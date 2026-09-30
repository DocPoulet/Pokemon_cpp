#include "pokemon/GameData.hpp"

#include "pokemon/DataCodec.hpp"
#include "pokemon/JsonLite.hpp"

#include <fstream>
#include <iostream>
#include <set>
#include <stdexcept>

namespace pokemon {
namespace {
MoveEffect statEffect(Target target, Stat stat, int stages, int chance = 100) {
    MoveEffect effect;
    effect.kind = EffectKind::StatChange;
    effect.target = target;
    effect.stat = stat;
    effect.stages = stages;
    effect.chancePercent = chance;
    return effect;
}

MoveEffect accuracyEffect(Target target, AccuracyStat stat, int stages) {
    MoveEffect effect;
    effect.kind = EffectKind::AccuracyChange;
    effect.target = target;
    effect.accuracyStat = stat;
    effect.stages = stages;
    return effect;
}

MoveEffect weatherEffect(Weather weather) {
    MoveEffect effect;
    effect.kind = EffectKind::SetWeather;
    effect.target = Target::Self;
    effect.weather = weather;
    return effect;
}

MoveEffect terrainEffect(Terrain terrain) {
    MoveEffect effect;
    effect.kind = EffectKind::SetTerrain;
    effect.target = Target::Self;
    effect.terrain = terrain;
    return effect;
}

MoveEffect randomBoost(int stages) {
    MoveEffect effect;
    effect.kind = EffectKind::RandomStatChange;
    effect.target = Target::Self;
    effect.stages = stages;
    return effect;
}

MoveEffect statusEffect(Target target, StatusCondition status, int chance = 100) {
    MoveEffect effect;
    effect.kind = EffectKind::ApplyStatus;
    effect.target = target;
    effect.status = status;
    effect.chancePercent = chance;
    return effect;
}
}


#ifndef POKEMON_DATA_DIR
#define POKEMON_DATA_DIR "data"
#endif

GameData::GameData() : GameData(std::string(POKEMON_DATA_DIR) + "/pokemon_species.json") {}

GameData::GameData(const std::string& speciesFile) {
    loadMoves();

    // MissingNo. est le filet de securite du catalogue. Il existe toujours, meme
    // si le JSON est absent ou invalide. Ses statistiques reprennent les valeurs
    // demandees pour le projet : 33 / 136 / 0 / 1 / 1 / 29.
    std::array<int, static_cast<std::size_t>(Stat::Count)> missingStats{33, 136, 0, 1, 1, 29};
    species_.emplace("MissingNo.", PokemonSpecies{
        "MissingNo.", missingStats, {Type::Flying, Type::Normal}, moveNames(), {}});

    loadSpeciesJson(speciesFile);
}

void GameData::loadMoves() {
    moves_.emplace("Flammeche", MoveData{
        "Flammeche", Type::Fire, 40, MoveCategory::Special, 100, 0, 25, false, false, {}});
    moves_.emplace("Griffe", MoveData{
        "Griffe", Type::Normal, 40, MoveCategory::Physical, 100, 0, 35, false, false, {}});
    moves_.emplace("Pistolet a O", MoveData{
        "Pistolet a O", Type::Water, 40, MoveCategory::Special, 100, 0, 25, false, false, {}});
    moves_.emplace("Fouet Lianes", MoveData{
        "Fouet Lianes", Type::Grass, 45, MoveCategory::Physical, 100, 0, 25, false, false, {}});
    moves_.emplace("Close Combat", MoveData{
        "Close Combat", Type::Fighting, 120, MoveCategory::Physical, 100, 0, 5, false, false,
        {statEffect(Target::Self, Stat::Defense, -1),
         statEffect(Target::Self, Stat::SpecialDefense, -1)}});
    moves_.emplace("Aiguisage", MoveData{
        "Aiguisage", Type::Steel, 0, MoveCategory::Status, -1, 0, 15, false, false,
        {statEffect(Target::Self, Stat::Attack, 1),
         accuracyEffect(Target::Self, AccuracyStat::Accuracy, 1)}});
    moves_.emplace("Psyko", MoveData{
        "Psyko", Type::Psychic, 90, MoveCategory::Special, 100, 0, 10, false, false,
        {statEffect(Target::Opponent, Stat::SpecialDefense, -1, 10)}});
    moves_.emplace("Acupression", MoveData{
        "Acupression", Type::Psychic, 0, MoveCategory::Status, -1, 0, 30, false, false,
        {randomBoost(2)}});
    moves_.emplace("Zenith", MoveData{
        "Zenith", Type::Fire, 0, MoveCategory::Status, -1, 0, 5, false, false,
        {weatherEffect(Weather::Sun)}});
    moves_.emplace("Danse-Pluie", MoveData{
        "Danse-Pluie", Type::Water, 0, MoveCategory::Status, -1, 0, 5, false, false,
        {weatherEffect(Weather::Rain)}});
    moves_.emplace("Champ Herbu", MoveData{
        "Champ Herbu", Type::Grass, 0, MoveCategory::Status, -1, 0, 10, false, false,
        {terrainEffect(Terrain::Grassy)}});

    moves_.emplace("Vive-Attaque", MoveData{
        "Vive-Attaque", Type::Normal, 40, MoveCategory::Physical, 100, 0, 30, false, false, {},
        1});
    moves_.emplace("Mega-Sangsue", MoveData{
        "Mega-Sangsue", Type::Grass, 40, MoveCategory::Special, 100, 0, 15, false, false, {},
        0, 0, 50});
    moves_.emplace("Belier", MoveData{
        "Belier", Type::Normal, 90, MoveCategory::Physical, 85, 0, 20, false, false, {},
        0, 25});
    moves_.emplace("Combo-Griffe", MoveData{
        "Combo-Griffe", Type::Normal, 18, MoveCategory::Physical, 80, 0, 15, false, false, {},
        0, 0, 0, 2, 5});
    moves_.emplace("Feu Follet", MoveData{
        "Feu Follet", Type::Fire, 0, MoveCategory::Status, 85, 0, 15, false, false,
        {statusEffect(Target::Opponent, StatusCondition::Burn)}});
    moves_.emplace("Toxik", MoveData{
        "Toxik", Type::Poison, 0, MoveCategory::Status, 90, 0, 10, false, false,
        {statusEffect(Target::Opponent, StatusCondition::Poison)}});
    moves_.emplace("Cage-Eclair", MoveData{
        "Cage-Eclair", Type::Electric, 0, MoveCategory::Status, 90, 0, 20, false, false,
        {statusEffect(Target::Opponent, StatusCondition::Paralysis)}});
    moves_.emplace("Poudre Dodo", MoveData{
        "Poudre Dodo", Type::Grass, 0, MoveCategory::Status, 75, 0, 15, false, false,
        {statusEffect(Target::Opponent, StatusCondition::Sleep)}});
    moves_.emplace("Laser Glace", MoveData{
        "Laser Glace", Type::Ice, 90, MoveCategory::Special, 100, 0, 10, false, false,
        {statusEffect(Target::Opponent, StatusCondition::Freeze, 10)}});

}

void GameData::loadSpeciesJson(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Catalogue d'especes introuvable: " << path
                  << ". MissingNo. sera utilise comme fallback.\n";
        return;
    }

    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() != 1) {
            std::cerr << "[Data] Version de pokemon_species.json non supportee. "
                      << "MissingNo. reste disponible.\n";
            return;
        }

        for (const auto& entry : root.at("species").asArray()) {
            try {
                const std::string name = entry.at("name").asString();
                if (name.empty()) throw std::runtime_error("nom vide");
                if (name == "MissingNo.") throw std::runtime_error("MissingNo. est reserve au fallback interne");
                if (species_.find(name) != species_.end()) throw std::runtime_error("espece dupliquee");

                const auto& stats = entry.at("base_stats");
                std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats{};
                baseStats[static_cast<std::size_t>(Stat::HP)] = stats.at("hp").asInt();
                baseStats[static_cast<std::size_t>(Stat::Attack)] = stats.at("attack").asInt();
                baseStats[static_cast<std::size_t>(Stat::Defense)] = stats.at("defense").asInt();
                baseStats[static_cast<std::size_t>(Stat::SpecialAttack)] = stats.at("special_attack").asInt();
                baseStats[static_cast<std::size_t>(Stat::SpecialDefense)] = stats.at("special_defense").asInt();
                baseStats[static_cast<std::size_t>(Stat::Speed)] = stats.at("speed").asInt();
                for (const int value : baseStats) if (value <= 0) throw std::runtime_error("stat de base invalide");

                std::vector<Type> types;
                for (const auto& value : entry.at("types").asArray()) types.push_back(typeFromDataId(value.asString()));
                if (types.empty() || types.size() > 2) throw std::runtime_error("il faut un ou deux types");

                std::vector<std::string> movePool;
                std::set<std::string> seenMoves;
                for (const auto& value : entry.at("move_pool").asArray()) {
                    const std::string moveName = value.asString();
                    if (!hasMove(moveName)) throw std::runtime_error("attaque inconnue: " + moveName);
                    if (!seenMoves.insert(moveName).second) throw std::runtime_error("attaque dupliquee: " + moveName);
                    movePool.push_back(moveName);
                }
                if (movePool.empty()) throw std::runtime_error("movepool vide");

                std::vector<Ability> abilities;
                std::set<int> seenAbilities;
                for (const auto& value : entry.at("abilities").asArray()) {
                    const Ability ability = abilityFromDataId(value.asString());
                    if (ability == Ability::None) throw std::runtime_error("talent None interdit");
                    if (!seenAbilities.insert(static_cast<int>(ability)).second) throw std::runtime_error("talent duplique");
                    abilities.push_back(ability);
                }
                if (abilities.empty()) throw std::runtime_error("aucun talent declare");

                species_.emplace(name, PokemonSpecies{name, baseStats, types, movePool, abilities});
            } catch (const std::exception& error) {
                std::cerr << "[Data] Espece JSON ignoree: " << error.what()
                          << ". Toute reference vers elle utilisera MissingNo.\n";
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "[Data] pokemon_species.json invalide: " << error.what()
                  << ". MissingNo. sera utilise comme fallback.\n";
    }
}

const MoveData& GameData::move(const std::string& name) const {
    const auto it = moves_.find(name);
    if (it == moves_.end()) throw std::out_of_range("Attaque inconnue: " + name);
    return it->second;
}

const PokemonSpecies& GameData::species(const std::string& name) const {
    const auto it = species_.find(name);
    if (it != species_.end()) return it->second;
    return species_.at("MissingNo.");
}

std::vector<std::string> GameData::moveNames() const {
    std::vector<std::string> result;
    result.reserve(moves_.size());
    for (const auto& [name, unused] : moves_) {
        (void)unused;
        result.push_back(name);
    }
    return result;
}

std::vector<std::string> GameData::speciesNames() const {
    std::vector<std::string> result;
    result.reserve(species_.size());
    for (const auto& [name, unused] : species_) {
        (void)unused;
        if (name == "MissingNo." && species_.size() > 1) continue;
        result.push_back(name);
    }
    return result;
}

bool GameData::hasMove(const std::string& name) const { return moves_.find(name) != moves_.end(); }
bool GameData::hasSpecies(const std::string& name) const { return species_.find(name) != species_.end(); }

} // namespace pokemon
