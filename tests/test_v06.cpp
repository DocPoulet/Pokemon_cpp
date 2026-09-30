#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/DamageCalculator.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/GameMode.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/TeamIO.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace pokemon;

namespace {
PokemonConfig baseConfig(const std::string& species) {
    PokemonConfig config;
    config.species = species;
    config.level = 50;
    config.ivs.fill(31);
    config.moves = {"Griffe", "Vive-Attaque", "Aiguisage", ""};
    config.ability = Ability::Blaze;
    return config;
}

void testExpandedCatalog() {
    GameData data;
    assert(data.hasSpecies("Dracaufeu"));
    assert(data.hasSpecies("Pikachu"));
    assert(data.speciesNames().size() >= 11);
    assert(data.hasMove("Laser Glace"));
    assert(!data.moveNames().empty());
}

void testTeamBuilderValidation() {
    GameData data;
    TeamBuilder builder(data);
    auto config = baseConfig("Salameche");
    assert(builder.validate(config).empty());

    config.evs.fill(252);
    assert(!builder.validate(config).empty());

    config = baseConfig("Inconnu");
    assert(!builder.validate(config).empty());
}

void testTeamIORoundTrip() {
    GameData data;
    TeamBuilder builder(data);
    Trainer original("DocPoulet");

    auto config = baseConfig("Salameche");
    config.level = 73;
    config.nature = Nature("Timide", Stat::Speed, Stat::Attack);
    config.evs[static_cast<std::size_t>(Stat::Speed)] = 252;
    config.evs[static_cast<std::size_t>(Stat::SpecialAttack)] = 252;
    config.heldItem = HeldItem::LifeOrb;
    config.ability = Ability::Blaze;
    builder.addPokemon(original, config);

    std::stringstream buffer;
    TeamIO::save(original, buffer);
    Trainer loaded = TeamIO::load(buffer, data);

    assert(loaded.name() == "DocPoulet");
    assert(loaded.team().size() == 1);
    const Pokemon& pokemon = loaded.team()[0];
    assert(pokemon.name() == "Salameche");
    assert(pokemon.level() == 73);
    assert(pokemon.heldItem() == HeldItem::LifeOrb);
    assert(pokemon.ability() == Ability::Blaze);
    assert(pokemon.ev(Stat::Speed) == 252);
    assert(pokemon.moves()[0] && pokemon.moves()[0]->data()->name == "Griffe");
}

void testPrimalWeather() {
    GameData data;
    Trainer p1("Feu");
    Trainer p2("Pluie");
    Pokemon fire(&data.species("Salameche"), 50);
    Pokemon rain(&data.species("Carapuce"), 50);
    rain.setAbility(Ability::PrimordialSea);
    p1.addPokemon(fire);
    p2.addPokemon(rain);
    Battle battle(p1, p2, 42);

    assert(battle.weather() == Weather::HeavyRain);
    const double damage = DamageCalculator::rawDamage(
        *battle.active(0), *battle.active(1), data.move("Flammeche"), battle, 0, 1);
    assert(damage == 0.0);

    const double normal = DamageCalculator::effectiveness(
        Type::Rock, data.species("Dracaufeu"), Weather::None);
    const double winds = DamageCalculator::effectiveness(
        Type::Rock, data.species("Dracaufeu"), Weather::StrongWinds);
    assert(normal == 4.0);
    assert(winds == 2.0);
}

void testTacticalAISwitch() {
    GameData data;
    Trainer aiTrainer("IA");
    Trainer foe("Adversaire");

    Pokemon fire(&data.species("Salameche"), 50);
    fire.setMove(0, &data.move("Flammeche"));
    Pokemon grass(&data.species("Bulbizarre"), 50);
    grass.setMove(0, &data.move("Mega-Sangsue"));
    Pokemon water(&data.species("Carapuce"), 50);
    water.setMove(0, &data.move("Pistolet a O"));

    aiTrainer.addPokemon(fire);
    aiTrainer.addPokemon(grass);
    foe.addPokemon(water);
    Battle battle(aiTrainer, foe, 7);
    battle.active(0)->setHP(battle.active(0)->maxHP() / 4);

    TacticalAI ai;
    const BattleAction action = ai.chooseAction(battle, 0);
    assert(action.type == ActionType::Switch);
    assert(action.index == 1);
}

void testGameModeLabels() {
    assert(toString(GameMode::PlayerVsAI) == "Joueur vs IA");
    assert(toString(GameMode::PlayerVsPlayer) == "Joueur vs Joueur");
    assert(toString(GameMode::AIvsAI) == "IA vs IA");
}
}

int main() {
    testExpandedCatalog();
    testTeamBuilderValidation();
    testTeamIORoundTrip();
    testPrimalWeather();
    testTacticalAISwitch();
    testGameModeLabels();
    std::cout << "Tous les tests v0.6 sont passes.\n";
    return 0;
}
