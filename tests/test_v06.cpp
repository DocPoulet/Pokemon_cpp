#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/DamageCalculator.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/GameMode.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/ShowdownExporter.hpp"
#include "pokemon/ShowdownImporter.hpp"

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
    config.moves = {"Scratch", "Quick Attack", "Hone Claws", ""};
    config.ability = "Blaze";
    return config;
}

void testExpandedCatalog() {
    GameData data;
    assert(data.hasSpecies("Charizard"));
    assert(data.hasSpecies("Pikachu"));
    assert(data.speciesNames().size() >= 11);
    assert(data.hasMove("Ice Beam"));
    assert(!data.moveNames().empty());
}

void testTeamBuilderValidation() {
    GameData data;
    TeamBuilder builder(data);
    auto config = baseConfig("Charmander");
    assert(builder.validate(config).empty());

    config.evs.fill(252);
    assert(!builder.validate(config).empty());

    config = baseConfig("Inconnu");
    assert(!builder.validate(config).empty());
}

void testShowdownRoundTrip() {
    GameData data;
    TeamBuilder builder(data);
    Trainer original("DocPoulet");

    auto config = baseConfig("Charmander");
    config.level = 73;
    config.nature = Nature("Timid", Stat::Speed, Stat::Attack);
    config.evs[static_cast<std::size_t>(Stat::Speed)] = 252;
    config.evs[static_cast<std::size_t>(Stat::SpecialAttack)] = 252;
    config.heldItem = "Life Orb";
    config.ability = "Blaze";
    builder.addPokemon(original, config);

    const std::string saved = ShowdownExporter::toText(original);
    Trainer loaded = ShowdownImporter::buildTeam(saved, "DocPoulet", data);

    assert(loaded.name() == "DocPoulet");
    assert(loaded.team().size() == 1);
    const Pokemon& pokemon = loaded.team()[0];
    assert(pokemon.name() == "Charmander");
    assert(pokemon.level() == 73);
    assert(pokemon.heldItem() && pokemon.heldItem()->id == "Life Orb");
    assert(pokemon.ability() && pokemon.ability()->id == "Blaze");
    assert(pokemon.ev(Stat::Speed) == 252);
    assert(pokemon.moves()[0] && pokemon.moves()[0]->data()->name == "Scratch");
}

void testPrimalWeather() {
    GameData data;
    Trainer p1("Feu");
    Trainer p2("Pluie");
    Pokemon fire(&data.species("Charmander"), 50);
    Pokemon rain(&data.species("Squirtle"), 50);
    rain.setAbility(&data.ability("PrimordialSea"));
    p1.addPokemon(fire);
    p2.addPokemon(rain);
    Battle battle(p1, p2, 42);

    assert(battle.weather() == Weather::HeavyRain);
    const double damage = DamageCalculator::rawDamage(
        *battle.active(0), *battle.active(1), data.move("Ember"), battle, 0, 1);
    assert(damage == 0.0);

    const double normal = DamageCalculator::effectiveness(
        Type::Rock, data.species("Charizard"), Weather::None);
    const double winds = DamageCalculator::effectiveness(
        Type::Rock, data.species("Charizard"), Weather::StrongWinds);
    assert(normal == 4.0);
    assert(winds == 2.0);
}

void testTacticalAISwitch() {
    GameData data;
    Trainer aiTrainer("IA");
    Trainer foe("Adversaire");

    Pokemon fire(&data.species("Charmander"), 50);
    fire.setMove(0, &data.move("Ember"));
    Pokemon grass(&data.species("Bulbasaur"), 50);
    grass.setMove(0, &data.move("Mega Drain"));
    Pokemon water(&data.species("Squirtle"), 50);
    water.setMove(0, &data.move("Water Gun"));

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
    testShowdownRoundTrip();
    testPrimalWeather();
    testTacticalAISwitch();
    testGameModeLabels();
    std::cout << "Tous les tests v0.6 sont passes.\n";
    return 0;
}
