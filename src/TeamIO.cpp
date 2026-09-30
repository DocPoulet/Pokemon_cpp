#include "pokemon/TeamIO.hpp"

#include "pokemon/TeamBuilder.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace pokemon {
namespace {
std::vector<std::string> split(const std::string& text, char delimiter) {
    std::vector<std::string> parts;
    std::stringstream stream(text);
    std::string part;
    while (std::getline(stream, part, delimiter)) parts.push_back(part);
    return parts;
}

std::string joinMoves(const Pokemon& pokemon) {
    std::string result;
    for (std::size_t i = 0; i < pokemon.moves().size(); ++i) {
        if (i > 0) result += '|';
        const auto& move = pokemon.moves()[i];
        if (move && move->data()) result += move->data()->name;
        else result += '-';
    }
    return result;
}

std::string joinStats(const Pokemon& pokemon, bool iv) {
    std::string result;
    for (std::size_t i = 0; i < static_cast<std::size_t>(Stat::Count); ++i) {
        if (i > 0) result += ',';
        const auto stat = static_cast<Stat>(i);
        result += std::to_string(iv ? pokemon.iv(stat) : pokemon.ev(stat));
    }
    return result;
}

std::array<int, static_cast<std::size_t>(Stat::Count)> parseStats(const std::string& text) {
    const auto values = split(text, ',');
    if (values.size() != static_cast<std::size_t>(Stat::Count)) {
        throw std::runtime_error("Une ligne IV/EV doit contenir exactement six valeurs.");
    }
    std::array<int, static_cast<std::size_t>(Stat::Count)> result{};
    for (std::size_t i = 0; i < values.size(); ++i) result[i] = std::stoi(values[i]);
    return result;
}

void readTeamHeader(std::istream& input, std::string& line, Trainer& trainer, int& count) {
    if (!std::getline(input, line) || line.rfind("trainer=", 0) != 0) {
        throw std::runtime_error("Nom de dresseur manquant.");
    }
    trainer = Trainer(line.substr(8));
    if (!std::getline(input, line) || line.rfind("count=", 0) != 0) {
        throw std::runtime_error("Nombre de Pokemon manquant.");
    }
    count = std::stoi(line.substr(6));
    if (count < 0 || count > 6) throw std::runtime_error("Une equipe doit contenir entre 0 et 6 Pokemon.");
}
}

void TeamIO::save(const Trainer& trainer, std::ostream& output) {
    output << "POKEMON_TEAM_V2\n";
    output << "trainer=" << trainer.name() << '\n';
    output << "count=" << trainer.team().size() << '\n';
    for (const auto& pokemon : trainer.team()) {
        output << "pokemon=" << pokemon.species().name() << ';'
               << pokemon.nickname() << ';'
               << pokemon.form() << ';'
               << pokemon.level() << ';'
               << pokemon.nature().name() << ';'
               << static_cast<int>(pokemon.nature().increased()) << ';'
               << static_cast<int>(pokemon.nature().decreased()) << ';'
               << static_cast<int>(pokemon.heldItem()) << ';'
               << static_cast<int>(pokemon.ability()) << ';'
               << joinStats(pokemon, true) << ';'
               << joinStats(pokemon, false) << ';'
               << joinMoves(pokemon) << '\n';
    }
}

Trainer TeamIO::load(std::istream& input, const GameData& data) {
    std::string line;
    if (!std::getline(input, line)) throw std::runtime_error("Sauvegarde d'equipe vide.");
    const bool v1 = line == "POKEMON_TEAM_V1";
    const bool v2 = line == "POKEMON_TEAM_V2";
    if (!v1 && !v2) throw std::runtime_error("Format d'equipe inconnu ou version non supportee.");

    Trainer trainer;
    int count = 0;
    readTeamHeader(input, line, trainer, count);
    TeamBuilder builder(data);

    for (int n = 0; n < count; ++n) {
        if (!std::getline(input, line) || line.rfind("pokemon=", 0) != 0) {
            throw std::runtime_error("Ligne Pokemon manquante dans la sauvegarde.");
        }
        try {
        const auto fields = split(line.substr(8), ';');
        if ((v1 && fields.size() != 10) || (v2 && fields.size() != 12)) {
            throw std::runtime_error("Ligne Pokemon invalide.");
        }

        PokemonConfig config;
        std::size_t offset = 0;
        config.species = fields[offset++];
        if (v2) {
            config.nickname = fields[offset++];
            config.form = fields[offset++].empty() ? "Base" : fields[offset - 1];
        }
        config.level = std::stoi(fields[offset++]);
        config.nature = Nature(fields[offset], static_cast<Stat>(std::stoi(fields[offset + 1])),
                               static_cast<Stat>(std::stoi(fields[offset + 2])));
        offset += 3;
        config.heldItem = static_cast<HeldItem>(std::stoi(fields[offset++]));
        config.ability = static_cast<Ability>(std::stoi(fields[offset++]));
        config.ivs = parseStats(fields[offset++]);
        config.evs = parseStats(fields[offset++]);
        const auto moveNames = split(fields[offset], '|');
        if (moveNames.size() != 4) throw std::runtime_error("Une equipe sauvegardee doit definir quatre slots d'attaque.");
        for (std::size_t i = 0; i < moveNames.size(); ++i) {
            if (moveNames[i] != "-") config.moves[i] = moveNames[i];
        }

        // Compatibilite v0.6 : les anciennes sauvegardes pouvaient stocker Ability::None.
        if (v1 && config.ability == Ability::None) {
            const auto& abilities = data.species(config.species).abilities();
            if (!abilities.empty()) config.ability = abilities.front();
        }

        if (!builder.addPokemon(trainer, config)) throw std::runtime_error("Equipe trop grande.");
        } catch (const std::exception&) {
            PokemonConfig invalid;
            invalid.species = "__invalid_saved_pokemon__";
            if (!builder.addPokemon(trainer, invalid)) throw std::runtime_error("Equipe trop grande.");
        }
    }
    return trainer;
}

bool TeamIO::saveFile(const Trainer& trainer, const std::string& path) {
    std::ofstream output(path);
    if (!output) return false;
    save(trainer, output);
    return static_cast<bool>(output);
}

Trainer TeamIO::loadFile(const std::string& path, const GameData& data) {
    std::ifstream input(path);
    if (!input) throw std::runtime_error("Impossible d'ouvrir le fichier d'equipe: " + path);
    return load(input, data);
}

} // namespace pokemon
