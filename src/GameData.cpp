#include "pokemon/GameData.hpp"

#include "pokemon/DataCodec.hpp"
#include "pokemon/JsonLite.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

namespace pokemon {
namespace {

std::string canonicalAbilityId(const std::string& id) {
    if (id == "PrimordialSea") return "Primordial Sea";
    if (id == "DesolateLand") return "Desolate Land";
    if (id == "DeltaStream") return "Delta Stream";
    return id;
}

MoveCategory categoryFromId(const std::string& id) {
    if (id == "Status") return MoveCategory::Status;
    if (id == "Physical") return MoveCategory::Physical;
    if (id == "Special") return MoveCategory::Special;
    throw std::invalid_argument("Categorie d'attaque inconnue: " + id);
}

EffectKind effectKindFromId(const std::string& id) {
    if (id == "StatChange") return EffectKind::StatChange;
    if (id == "AccuracyChange") return EffectKind::AccuracyChange;
    if (id == "SetWeather") return EffectKind::SetWeather;
    if (id == "SetTerrain") return EffectKind::SetTerrain;
    if (id == "RandomStatChange") return EffectKind::RandomStatChange;
    if (id == "ApplyStatus") return EffectKind::ApplyStatus;
    throw std::invalid_argument("Effet d'attaque inconnu: " + id);
}

Target targetFromId(const std::string& id) {
    if (id == "Self") return Target::Self;
    if (id == "Opponent") return Target::Opponent;
    throw std::invalid_argument("Cible d'effet inconnue: " + id);
}

AccuracyStat accuracyStatFromId(const std::string& id) {
    if (id == "Accuracy") return AccuracyStat::Accuracy;
    if (id == "Evasion") return AccuracyStat::Evasion;
    throw std::invalid_argument("Stat de precision inconnue: " + id);
}

Weather weatherFromId(const std::string& id) {
    if (id == "None") return Weather::None;
    if (id == "Sun") return Weather::Sun;
    if (id == "Rain") return Weather::Rain;
    if (id == "Sandstorm") return Weather::Sandstorm;
    if (id == "Snow") return Weather::Snow;
    if (id == "ExtremelyHarshSunlight") return Weather::ExtremelyHarshSunlight;
    if (id == "HeavyRain") return Weather::HeavyRain;
    if (id == "StrongWinds") return Weather::StrongWinds;
    throw std::invalid_argument("Meteo inconnue: " + id);
}

Terrain terrainFromId(const std::string& id) {
    if (id == "None") return Terrain::None;
    if (id == "Electric") return Terrain::Electric;
    if (id == "Misty") return Terrain::Misty;
    if (id == "Grassy") return Terrain::Grassy;
    if (id == "Psychic") return Terrain::Psychic;
    throw std::invalid_argument("Terrain inconnu: " + id);
}

StatusCondition statusFromId(const std::string& id) {
    if (id == "None") return StatusCondition::None;
    if (id == "Burn") return StatusCondition::Burn;
    if (id == "Poison") return StatusCondition::Poison;
    if (id == "Paralysis") return StatusCondition::Paralysis;
    if (id == "Sleep") return StatusCondition::Sleep;
    if (id == "Freeze") return StatusCondition::Freeze;
    throw std::invalid_argument("Statut inconnu: " + id);
}

MoveEffect readEffect(const JsonValue& value) {
    MoveEffect effect;
    effect.kind = effectKindFromId(value.at("kind").asString());
    if (value.contains("target")) effect.target = targetFromId(value.at("target").asString());
    if (value.contains("stat")) effect.stat = statFromDataId(value.at("stat").asString());
    if (value.contains("accuracy_stat")) effect.accuracyStat = accuracyStatFromId(value.at("accuracy_stat").asString());
    if (value.contains("stages")) effect.stages = value.at("stages").asInt();
    if (value.contains("chance")) effect.chancePercent = value.at("chance").asInt();
    if (value.contains("weather")) effect.weather = weatherFromId(value.at("weather").asString());
    if (value.contains("terrain")) effect.terrain = terrainFromId(value.at("terrain").asString());
    if (value.contains("status")) effect.status = statusFromId(value.at("status").asString());
    return effect;
}

} // namespace

#ifndef POKEMON_DATA_DIR
#define POKEMON_DATA_DIR "data"
#endif

GameData::GameData() : GameData(std::string(POKEMON_DATA_DIR) + "/pokemon_species.json") {}

GameData::GameData(const std::string& speciesFile) {
    const std::string dataDir = POKEMON_DATA_DIR;
    loadMovesJson(dataDir + "/moves.json");
    loadAbilitiesJson(dataDir + "/abilities.json");
    loadItemsJson(dataDir + "/items.json");
    installFallbackCatalogs();

    std::array<int, static_cast<std::size_t>(Stat::Count)> missingStats{33, 136, 0, 1, 1, 29};
    std::vector<const MoveData*> missingMoves;
    for (const auto& [id, moveData] : moves_) {
        (void)id;
        missingMoves.push_back(&moveData);
    }
    species_.emplace("MissingNo.", PokemonSpecies{
        "MissingNo.", missingStats, {Type::Flying, Type::Normal}, std::move(missingMoves), {}});

    loadSpeciesJson(speciesFile);
}

void GameData::installFallbackCatalogs() {
    if (moves_.empty()) {
        moves_.emplace("Tackle", MoveData{"Tackle", Type::Normal, 40, MoveCategory::Physical, 100, 0, 35, false, false, {}, 0, 0, 0, 1, 1});
        moves_.emplace("Swift", MoveData{"Swift", Type::Normal, 60, MoveCategory::Special, -1, 0, 20, false, false, {}, 0, 0, 0, 1, 1});
        moves_.emplace("Aerial Ace", MoveData{"Aerial Ace", Type::Flying, 60, MoveCategory::Physical, -1, 0, 20, false, false, {}, 0, 0, 0, 1, 1});
        moves_.emplace("Splash", MoveData{"Splash", Type::Normal, 0, MoveCategory::Status, -1, 0, 40, false, false, {}, 0, 0, 0, 1, 1});
    }
    if (abilities_.empty()) {
        abilities_.emplace("Overgrow", AbilityData{"Overgrow", Ability::Overgrow, 3, "", "Powers up Grass-type moves in a pinch."});
        abilities_.emplace("Blaze", AbilityData{"Blaze", Ability::Blaze, 3, "", "Powers up Fire-type moves in a pinch."});
        abilities_.emplace("Torrent", AbilityData{"Torrent", Ability::Torrent, 3, "", "Powers up Water-type moves in a pinch."});
    }
    if (items_.empty()) {
        items_.emplace("Leftovers", ItemData{"Leftovers", HeldItem::Leftovers, 2, "", "Restores a little HP every turn."});
        items_.emplace("Life Orb", ItemData{"Life Orb", HeldItem::LifeOrb, 4, "", "Boosts damage at the cost of HP."});
        items_.emplace("Air Balloon", ItemData{"Air Balloon", HeldItem::AirBalloon, 5, "", "Grants Ground immunity until popped."});
        items_.emplace("Red Orb", ItemData{"Red Orb", HeldItem::RedOrb, 6, "Past", "Enables Primal Reversion for Groudon."});
        items_.emplace("Blue Orb", ItemData{"Blue Orb", HeldItem::BlueOrb, 6, "Past", "Enables Primal Reversion for Kyogre."});
    }
}

void GameData::loadMovesJson(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Catalogue d'attaques introuvable: " << path << ". Fallback minimal charge.\n";
        return;
    }
    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() < 1 || root.at("version").asInt() > 2) throw std::runtime_error("version non supportee");
        for (const auto& entry : root.at("moves").asArray()) {
            const std::string id = entry.at("id").asString();
            if (id.empty() || moves_.count(id)) throw std::runtime_error("attaque invalide ou dupliquee: " + id);
            MoveData move;
            move.name = id;
            move.type = typeFromDataId(entry.at("type").asString());
            move.power = entry.at("power").asInt();
            move.category = categoryFromId(entry.at("category").asString());
            move.accuracy = entry.at("accuracy").asInt();
            move.criticalBonus = entry.at("critical_bonus").asInt();
            move.pp = entry.at("pp").asInt();
            move.priority = entry.at("priority").asInt();
            move.recoilPercent = entry.at("recoil_percent").asInt();
            move.drainPercent = entry.at("drain_percent").asInt();
            move.minHits = entry.at("min_hits").asInt();
            move.maxHits = entry.at("max_hits").asInt();
            move.recoilThird = entry.contains("recoil_third") && entry.at("recoil_third").asBoolean();
            move.struggle = entry.contains("struggle") && entry.at("struggle").asBoolean();
            if (entry.contains("generation")) move.generation = entry.at("generation").asInt();
            if (entry.contains("number")) move.nationalNumber = entry.at("number").asInt();
            if (entry.contains("non_standard")) move.nonStandard = entry.at("non_standard").asString();
            if (entry.contains("short_description")) move.shortDescription = entry.at("short_description").asString();
            if (entry.contains("mechanics_complete")) move.catalogMechanicsComplete = entry.at("mechanics_complete").asBoolean();
            if (entry.contains("effects")) {
                for (const auto& effect : entry.at("effects").asArray()) move.effects.push_back(readEffect(effect));
            }
            moves_.emplace(id, std::move(move));
        }
    } catch (const std::exception& error) {
        moves_.clear();
        std::cerr << "[Data] moves.json invalide: " << error.what() << ". Fallback minimal charge.\n";
    }
}

void GameData::loadAbilitiesJson(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Catalogue de talents introuvable: " << path << ". Fallback minimal charge.\n";
        return;
    }
    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() < 1 || root.at("version").asInt() > 2) throw std::runtime_error("version non supportee");
        for (const auto& entry : root.at("abilities").asArray()) {
            const std::string id = entry.at("id").asString();
            const Ability mechanic = entry.contains("mechanic")
                ? abilityFromDataId(entry.at("mechanic").asString()) : Ability::None;
            if (id.empty() || abilities_.count(id)) {
                throw std::runtime_error("talent invalide ou duplique: " + id);
            }
            AbilityData data;
            data.id = id;
            data.mechanic = mechanic;
            if (entry.contains("generation")) data.generation = entry.at("generation").asInt();
            if (entry.contains("non_standard")) data.nonStandard = entry.at("non_standard").asString();
            if (entry.contains("short_description")) data.shortDescription = entry.at("short_description").asString();
            if (entry.contains("number")) data.nationalNumber = entry.at("number").asInt();
            abilities_.emplace(id, std::move(data));
        }
    } catch (const std::exception& error) {
        abilities_.clear();
        std::cerr << "[Data] abilities.json invalide: " << error.what() << ". Fallback minimal charge.\n";
    }
}

void GameData::loadItemsJson(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Catalogue d'objets introuvable: " << path << ". Fallback minimal charge.\n";
        return;
    }
    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() < 1 || root.at("version").asInt() > 2) throw std::runtime_error("version non supportee");
        for (const auto& entry : root.at("items").asArray()) {
            const std::string id = entry.at("id").asString();
            const HeldItem mechanic = entry.contains("mechanic")
                ? heldItemFromDataId(entry.at("mechanic").asString()) : HeldItem::None;
            if (id.empty() || items_.count(id)) {
                throw std::runtime_error("objet invalide ou duplique: " + id);
            }
            ItemData data;
            data.id = id;
            data.mechanic = mechanic;
            if (entry.contains("generation")) data.generation = entry.at("generation").asInt();
            if (entry.contains("non_standard")) data.nonStandard = entry.at("non_standard").asString();
            if (entry.contains("short_description")) data.shortDescription = entry.at("short_description").asString();
            if (entry.contains("number")) data.nationalNumber = entry.at("number").asInt();
            items_.emplace(id, std::move(data));
        }
    } catch (const std::exception& error) {
        items_.clear();
        std::cerr << "[Data] items.json invalide: " << error.what() << ". Fallback minimal charge.\n";
    }
}

void GameData::loadSpeciesJson(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Catalogue d'especes introuvable: " << path << ". MissingNo. sera utilise.\n";
        return;
    }
    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() < 1 || root.at("version").asInt() > 2) throw std::runtime_error("version non supportee");
        for (const auto& entry : root.at("species").asArray()) {
            try {
                const std::string name = entry.at("name").asString();
                if (name.empty() || name == "MissingNo." || species_.count(name)) throw std::runtime_error("nom reserve, vide ou duplique");
                const auto& stats = entry.at("base_stats");
                std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats{};
                baseStats[0] = stats.at("hp").asInt();
                baseStats[1] = stats.at("attack").asInt();
                baseStats[2] = stats.at("defense").asInt();
                baseStats[3] = stats.at("special_attack").asInt();
                baseStats[4] = stats.at("special_defense").asInt();
                baseStats[5] = stats.at("speed").asInt();
                for (const int value : baseStats) if (value <= 0) throw std::runtime_error("stat de base invalide");

                std::vector<Type> types;
                for (const auto& value : entry.at("types").asArray()) types.push_back(typeFromDataId(value.asString()));
                if (types.empty() || types.size() > 2) throw std::runtime_error("il faut un ou deux types");

                std::vector<const MoveData*> movePool;
                std::set<std::string> seenMoves;
                for (const auto& value : entry.at("move_pool").asArray()) {
                    const std::string id = value.asString();
                    if (!seenMoves.insert(id).second) throw std::runtime_error("reference d'attaque dupliquee: " + id);
                    if (!hasMove(id)) throw std::runtime_error("reference d'attaque inconnue: " + id);
                    movePool.push_back(&move(id));
                }
                if (movePool.empty()) throw std::runtime_error("movepool vide");

                std::vector<const AbilityData*> abilities;
                std::set<std::string> seenAbilities;
                for (const auto& value : entry.at("abilities").asArray()) {
                    const std::string id = value.asString();
                    if (!seenAbilities.insert(id).second) throw std::runtime_error("reference de talent dupliquee: " + id);
                    if (!hasAbility(id)) throw std::runtime_error("reference de talent inconnue: " + id);
                    abilities.push_back(&ability(id));
                }
                if (abilities.empty()) throw std::runtime_error("aucun talent possible");

                species_.emplace(name, PokemonSpecies{name, baseStats, std::move(types), std::move(movePool), std::move(abilities)});
            } catch (const std::exception& error) {
                std::cerr << "[Data] Espece ignoree: " << error.what() << ".\n";
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "[Data] pokemon_species.json invalide: " << error.what() << ". MissingNo. reste disponible.\n";
    }
}

const MoveData& GameData::move(const std::string& id) const {
    const auto it = moves_.find(id);
    if (it == moves_.end()) throw std::out_of_range("Attaque inconnue: " + id);
    return it->second;
}
const AbilityData& GameData::ability(const std::string& id) const {
    const std::string canonical = canonicalAbilityId(id);
    const auto it = abilities_.find(canonical);
    if (it == abilities_.end()) throw std::out_of_range("Talent inconnu: " + id);
    return it->second;
}
const ItemData& GameData::item(const std::string& id) const {
    const auto it = items_.find(id);
    if (it == items_.end()) throw std::out_of_range("Objet inconnu: " + id);
    return it->second;
}
const PokemonSpecies& GameData::species(const std::string& id) const {
    const auto it = species_.find(id);
    if (it != species_.end()) return it->second;
    const auto fallback = species_.find("MissingNo.");
    if (fallback != species_.end()) return fallback->second;
    throw std::out_of_range("Espece inconnue: " + id);
}

std::vector<std::string> GameData::moveNames() const { std::vector<std::string> r; for (const auto& [id,v]:moves_){(void)v;r.push_back(id);} return r; }
std::vector<std::string> GameData::abilityNames() const { std::vector<std::string> r; for (const auto& [id,v]:abilities_){(void)v;r.push_back(id);} return r; }
std::vector<std::string> GameData::itemNames() const { std::vector<std::string> r; for (const auto& [id,v]:items_){(void)v;r.push_back(id);} return r; }
std::vector<std::string> GameData::speciesNames() const { std::vector<std::string> r; for (const auto& [id,v]:species_){(void)v;if(id!="MissingNo."||species_.size()==1)r.push_back(id);} return r; }
bool GameData::hasMove(const std::string& id) const { return moves_.count(id) != 0; }
bool GameData::hasAbility(const std::string& id) const { return abilities_.count(canonicalAbilityId(id)) != 0; }
bool GameData::hasItem(const std::string& id) const { return items_.count(id) != 0; }
bool GameData::hasSpecies(const std::string& id) const { return species_.count(id) != 0; }

} // namespace pokemon
