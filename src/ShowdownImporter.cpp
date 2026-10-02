#include "pokemon/ShowdownImporter.hpp"

#include "pokemon/DataCodec.hpp"

#include <fstream>
#include <algorithm>
#include <array>
#include <cctype>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace pokemon {
namespace {
std::string trim(std::string value) {
    const auto notSpace = [](unsigned char c) { return !std::isspace(c); };
    value.erase(value.begin(), std::find_if(value.begin(), value.end(), notSpace));
    value.erase(std::find_if(value.rbegin(), value.rend(), notSpace).base(), value.end());
    return value;
}

bool startsWith(const std::string& text, const std::string& prefix) {
    return text.rfind(prefix, 0) == 0;
}

std::vector<std::string> split(const std::string& text, char delimiter) {
    std::vector<std::string> result;
    std::stringstream stream(text);
    std::string part;
    while (std::getline(stream, part, delimiter)) result.push_back(trim(part));
    return result;
}

std::string speciesAlias(std::string name) {
    static const std::unordered_map<std::string, std::string> legacy{
        {"Bulbizarre", "Bulbasaur"}, {"Herbizarre", "Ivysaur"}, {"Florizarre", "Venusaur"},
        {"Salameche", "Charmander"}, {"Salamèche", "Charmander"}, {"Reptincel", "Charmeleon"}, {"Dracaufeu", "Charizard"},
        {"Carapuce", "Squirtle"}, {"Carabaffe", "Wartortle"}, {"Tortank", "Blastoise"}, {"Evoli", "Eevee"}, {"Évoli", "Eevee"},
        {"Farfetch'd", "Farfetch’d"}
    };
    const auto it = legacy.find(name);
    return it == legacy.end() ? name : it->second;
}

std::string moveAlias(std::string name) {
    static const std::unordered_map<std::string, std::string> legacy{
        {"Flammeche", "Ember"}, {"Flammèche", "Ember"}, {"Griffe", "Scratch"}, {"Pistolet a O", "Water Gun"},
        {"Fouet Lianes", "Vine Whip"}, {"Aiguisage", "Hone Claws"}, {"Psyko", "Psychic"},
        {"Acupression", "Acupressure"}, {"Zenith", "Sunny Day"}, {"Zénith", "Sunny Day"}, {"Danse-Pluie", "Rain Dance"},
        {"Champ Herbu", "Grassy Terrain"}, {"Vive-Attaque", "Quick Attack"}, {"Mega-Sangsue", "Mega Drain"},
        {"Méga-Sangsue", "Mega Drain"}, {"Belier", "Take Down"}, {"Bélier", "Take Down"}, {"Combo-Griffe", "Fury Swipes"},
        {"Feu Follet", "Will-O-Wisp"}, {"Toxik", "Toxic"}, {"Cage-Eclair", "Thunder Wave"}, {"Cage-Éclair", "Thunder Wave"},
        {"Poudre Dodo", "Sleep Powder"}, {"Laser Glace", "Ice Beam"}
    };
    const auto it = legacy.find(name);
    return it == legacy.end() ? name : it->second;
}

Stat showdownStat(const std::string& name) {
    if (name == "HP") return Stat::HP;
    if (name == "Atk") return Stat::Attack;
    if (name == "Def") return Stat::Defense;
    if (name == "SpA") return Stat::SpecialAttack;
    if (name == "SpD") return Stat::SpecialDefense;
    if (name == "Spe") return Stat::Speed;
    throw std::invalid_argument("Stat Showdown inconnue: " + name);
}

Nature natureFromName(const std::string& name) {
    using S = Stat;
    static const std::unordered_map<std::string, std::pair<S,S>> map{
        {"Hardy", {S::Attack,S::Attack}}, {"Lonely", {S::Attack,S::Defense}},
        {"Brave", {S::Attack,S::Speed}}, {"Adamant", {S::Attack,S::SpecialAttack}},
        {"Naughty", {S::Attack,S::SpecialDefense}}, {"Bold", {S::Defense,S::Attack}},
        {"Docile", {S::Defense,S::Defense}}, {"Relaxed", {S::Defense,S::Speed}},
        {"Impish", {S::Defense,S::SpecialAttack}}, {"Lax", {S::Defense,S::SpecialDefense}},
        {"Timid", {S::Speed,S::Attack}}, {"Hasty", {S::Speed,S::Defense}},
        {"Serious", {S::Speed,S::Speed}}, {"Jolly", {S::Speed,S::SpecialAttack}},
        {"Naive", {S::Speed,S::SpecialDefense}}, {"Modest", {S::SpecialAttack,S::Attack}},
        {"Mild", {S::SpecialAttack,S::Defense}}, {"Quiet", {S::SpecialAttack,S::Speed}},
        {"Bashful", {S::SpecialAttack,S::SpecialAttack}}, {"Rash", {S::SpecialAttack,S::SpecialDefense}},
        {"Calm", {S::SpecialDefense,S::Attack}}, {"Gentle", {S::SpecialDefense,S::Defense}},
        {"Sassy", {S::SpecialDefense,S::Speed}}, {"Careful", {S::SpecialDefense,S::SpecialAttack}},
        {"Quirky", {S::SpecialDefense,S::SpecialDefense}}
    };
    const auto it = map.find(name);
    if (it == map.end()) throw std::invalid_argument("Nature Showdown inconnue: " + name);
    return Nature(name, it->second.first, it->second.second);
}

std::string abilityFromShowdown(const std::string& text) {
    // Showdown utilise déjà les identifiants anglais canoniques du catalogue.
    // Ne pas retirer les espaces : "Lightning Rod" et "Keen Eye" sont des IDs valides.
    return trim(text);
}

std::string itemFromShowdown(const std::string& text) {
    return trim(text);
}

void parseSpread(const std::string& text, std::array<int, static_cast<std::size_t>(Stat::Count)>& values) {
    for (const auto& part : split(text, '/')) {
        std::stringstream ss(part);
        int value = 0;
        std::string stat;
        if (!(ss >> value >> stat)) throw std::invalid_argument("Spread Showdown invalide: " + part);
        values[static_cast<std::size_t>(showdownStat(stat))] = value;
    }
}

PokemonConfig parseBlock(const std::vector<std::string>& lines, std::vector<std::string>& warnings) {
    PokemonConfig config;
    config.level = 100;
    config.form = "Base";
    config.ivs.fill(31);
    config.evs.fill(0);
    config.dynamaxLevel = 10;
    config.gigantamax = false;
    if (lines.empty()) return config;

    std::string header = trim(lines.front());
    const std::size_t at = header.rfind(" @ ");
    if (at != std::string::npos) {
        config.heldItem = itemFromShowdown(trim(header.substr(at + 3)));
        header = trim(header.substr(0, at));
    }

    if (header.size() >= 4 && header.compare(header.size()-4, 4, " (M)") == 0) {
        config.gender = "M"; header = trim(header.substr(0, header.size()-4));
    } else if (header.size() >= 4 && header.compare(header.size()-4, 4, " (F)") == 0) {
        config.gender = "F"; header = trim(header.substr(0, header.size()-4));
    }

    if (!header.empty() && header.back() == ')') {
        const std::size_t open = header.rfind(" (");
        if (open != std::string::npos) {
            config.nickname = trim(header.substr(0, open));
            config.species = speciesAlias(header.substr(open + 2, header.size() - open - 3));
        } else config.species = speciesAlias(header);
    } else config.species = speciesAlias(header);

    std::size_t moveSlot = 0;
    for (std::size_t i = 1; i < lines.size(); ++i) {
        const std::string line = trim(lines[i]);
        try {
            if (startsWith(line, "Ability:")) config.ability = abilityFromShowdown(trim(line.substr(8)));
            else if (startsWith(line, "Level:")) config.level = std::stoi(trim(line.substr(6)));
            else if (startsWith(line, "EVs:")) parseSpread(trim(line.substr(4)), config.evs);
            else if (startsWith(line, "IVs:")) parseSpread(trim(line.substr(4)), config.ivs);
            else if (startsWith(line, "Shiny:")) config.shiny = trim(line.substr(6)) == "Yes";
            else if (startsWith(line, "Tera Type:")) config.teraType = typeFromDataId(trim(line.substr(10)));
            else if (startsWith(line, "Dynamax Level:")) config.dynamaxLevel = std::stoi(trim(line.substr(14)));
            else if (startsWith(line, "Gigantamax:")) config.gigantamax = trim(line.substr(11)) == "Yes";
            else if (line.size() > 7 && line.compare(line.size()-7, 7, " Nature") == 0) {
                config.nature = natureFromName(trim(line.substr(0, line.size()-7)));
            } else if (startsWith(line, "- ")) {
                if (moveSlot < config.moves.size()) config.moves[moveSlot++] = moveAlias(trim(line.substr(2)));
                else warnings.push_back("Plus de quatre attaques: ligne ignoree: " + line);
            } else if (!line.empty()) warnings.push_back("Ligne Showdown ignoree: " + line);
        } catch (const std::exception& e) {
            warnings.push_back(e.what());
        }
    }
    return config;
}
} // namespace

ShowdownImportResult ShowdownImporter::parse(std::istream& input) {
    ShowdownImportResult result;
    std::vector<std::string> block;
    std::string line;
    auto flush = [&]() {
        if (block.empty()) return;
        result.pokemon.push_back(parseBlock(block, result.warnings));
        block.clear();
    };
    while (std::getline(input, line)) {
        if (trim(line).empty()) flush();
        else block.push_back(line);
    }
    flush();
    return result;
}

ShowdownImportResult ShowdownImporter::parseText(const std::string& text) {
    std::istringstream input(text);
    return parse(input);
}

Trainer ShowdownImporter::buildTeam(const std::string& text, const std::string& trainerName,
                                    const GameData& data, std::vector<std::string>* warnings) {
    auto parsed = parseText(text);
    if (warnings) *warnings = parsed.warnings;
    Trainer trainer(trainerName);
    TeamBuilder builder(data);
    const std::size_t count = std::min<std::size_t>(6, parsed.pokemon.size());
    for (std::size_t i = 0; i < count; ++i) builder.addPokemon(trainer, parsed.pokemon[i]);
    if (parsed.pokemon.size() > 6 && warnings) warnings->push_back("Showdown: seuls les six premiers Pokemon ont ete importes.");
    return trainer;
}

Trainer ShowdownImporter::loadFile(const std::string& path, const std::string& trainerName,
                                   const GameData& data, std::vector<std::string>* warnings) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("Impossible d'ouvrir le fichier Showdown: " + path);
    std::ostringstream buffer;
    buffer << input.rdbuf();
    return buildTeam(buffer.str(), trainerName, data, warnings);
}

} // namespace pokemon
