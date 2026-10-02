/**
 * @file test_v04.cpp
 * @brief Tests de non-regression de la release v0.4 Documentation.
 *
 * La v0.4 ne doit pas modifier les mecanismes valides de la v0.3. Ces tests
 * verrouillent notamment les PP independants, les switches, IV/EV, types,
 * evenements, meteo et la baseline d IA.
 */

#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/DamageCalculator.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/Pokemon.hpp"
#include "pokemon/Trainer.hpp"

#include <cassert>
#include <iostream>

using namespace pokemon;

namespace {
void testIndependentMovePP() {
    GameData data;
    Pokemon a(&data.species("Charmander"), 50);
    Pokemon b(&data.species("Charmander"), 50);

    a.setMove(0, &data.move("Ember"));
    b.setMove(0, &data.move("Ember"));

    a.moves()[0]->consumePP();
    assert(a.moves()[0]->currentPP() == 24);
    assert(b.moves()[0]->currentPP() == 25);
}

void testSwitchAndStagesReset() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    p1.addPokemon(Pokemon(&data.species("Charmander"), 50));
    p1.addPokemon(Pokemon(&data.species("Squirtle"), 50));
    p2.addPokemon(Pokemon(&data.species("Bulbasaur"), 50));

    Battle battle(p1, p2, 42);
    battle.changeStage(0, Stat::Attack, 4);
    assert(battle.stage(0, Stat::Attack) == 4);
    assert(battle.switchPokemon(0, 1));
    assert(battle.activeIndex(0) == 1);
    assert(battle.active(0)->name() == "Squirtle");
    assert(battle.stage(0, Stat::Attack) == 0);
}

void testIvEvBounds() {
    GameData data;
    Pokemon pokemon(&data.species("Charmander"), 50);
    pokemon.setIV(Stat::Attack, 99);
    pokemon.setIV(Stat::Defense, -4);
    assert(pokemon.iv(Stat::Attack) == 31);
    assert(pokemon.iv(Stat::Defense) == 0);

    assert(pokemon.setEV(Stat::Attack, 252));
    assert(pokemon.setEV(Stat::Defense, 252));
    assert(!pokemon.setEV(Stat::Speed, 252));
    assert(pokemon.totalEV() == 510);
    assert(pokemon.ev(Stat::Speed) == 6);
}

void testTypeEffectiveness() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    p1.addPokemon(Pokemon(&data.species("Charmander"), 50));
    p2.addPokemon(Pokemon(&data.species("Bulbasaur"), 50));
    Battle battle(p1, p2, 42);

    assert(DamageCalculator::effectiveness(
        Type::Fire, data.species("Bulbasaur"), battle.weather()) == 2.0);
}

void testBattleEventsAndDamage() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");

    Pokemon fire(&data.species("Charmander"), 50);
    Pokemon grass(&data.species("Bulbasaur"), 50);
    fire.setMove(0, &data.move("Ember"));
    grass.setMove(0, &data.move("Vine Whip"));
    p1.addPokemon(fire);
    p2.addPokemon(grass);

    Battle battle(p1, p2, 42);
    const int hpBefore = battle.active(1)->currentHP();
    const auto events = battle.resolveTurn(
        {ActionType::Move, 0}, {ActionType::Move, 0});

    assert(!events.empty());
    assert(battle.active(1)->currentHP() < hpBefore);
    assert(battle.active(0)->moves()[0]->currentPP() == 24);
}

void testWeatherEffect() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon fire(&data.species("Charmander"), 50);
    Pokemon water(&data.species("Squirtle"), 50);
    fire.setMove(0, &data.move("Sunny Day"));
    water.setMove(0, &data.move("Water Gun"));
    p1.addPokemon(fire);
    p2.addPokemon(water);

    Battle battle(p1, p2, 7);
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.weather() == Weather::Sun);
    assert(battle.weatherTurns() == 4);
}

void testGreedyAI() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon grass(&data.species("Bulbasaur"), 50);
    Pokemon fire(&data.species("Charmander"), 50);
    grass.setMove(0, &data.move("Vine Whip"));
    fire.setMove(0, &data.move("Scratch"));
    fire.setMove(1, &data.move("Ember"));
    p1.addPokemon(grass);
    p2.addPokemon(fire);

    Battle battle(p1, p2, 42);
    GreedyAI ai;
    const auto action = ai.chooseAction(battle, 1);
    assert(action.type == ActionType::Move);
    assert(action.index == 1);
}
}

int main() {
    testIndependentMovePP();
    testSwitchAndStagesReset();
    testIvEvBounds();
    testTypeEffectiveness();
    testBattleEventsAndDamage();
    testWeatherEffect();
    testGreedyAI();

    std::cout << "Tous les tests v0.4 sont passes.\n";
    return 0;
}
