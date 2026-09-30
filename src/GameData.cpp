#include "pokemon/GameData.hpp"

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

GameData::GameData() {
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

    species_.emplace("Bulbizarre", PokemonSpecies{
        "Bulbizarre", {45,49,49,65,65,45}, {Type::Grass, Type::Poison}});
    species_.emplace("Salameche", PokemonSpecies{
        "Salameche", {39,52,43,60,50,65}, {Type::Fire}});
    species_.emplace("Carapuce", PokemonSpecies{
        "Carapuce", {44,48,65,50,64,43}, {Type::Water}});
}

const MoveData& GameData::move(const std::string& name) const {
    const auto it = moves_.find(name);
    if (it == moves_.end()) throw std::out_of_range("Attaque inconnue: " + name);
    return it->second;
}

const PokemonSpecies& GameData::species(const std::string& name) const {
    const auto it = species_.find(name);
    if (it == species_.end()) throw std::out_of_range("Espece inconnue: " + name);
    return it->second;
}

} // namespace pokemon
