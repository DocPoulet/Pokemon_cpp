#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/DamageCalculator.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/Mechanics.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/Pokemon.hpp"
#include "pokemon/Trainer.hpp"

#include <cassert>
#include <iostream>
#include <string>

using namespace pokemon;

namespace {
Battle makeBattle(GameData& data, Trainer& p1, Trainer& p2) {
    p1.addPokemon(Pokemon(&data.species("Salameche"), 50));
    p2.addPokemon(Pokemon(&data.species("Carapuce"), 50));
    return Battle(p1, p2, 42);
}

void testIndependentMovePP() {
    GameData data;
    Pokemon a(&data.species("Salameche"), 50);
    Pokemon b(&data.species("Salameche"), 50);
    a.setMove(0, &data.move("Flammeche"));
    b.setMove(0, &data.move("Flammeche"));
    a.moves()[0]->consumePP();
    assert(a.moves()[0]->currentPP() == 24);
    assert(b.moves()[0]->currentPP() == 25);
}

void testPriorityBeforeSpeed() {
    GameData data;
    Trainer p1("Lent");
    Trainer p2("Rapide");
    Pokemon slow(&data.species("Carapuce"), 50);
    Pokemon fast(&data.species("Salameche"), 50);
    slow.setMove(0, &data.move("Vive-Attaque"));
    fast.setMove(0, &data.move("Griffe"));
    p1.addPokemon(slow);
    p2.addPokemon(fast);
    Battle battle(p1, p2, 3);

    const auto events = battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    for (const auto& event : events) {
        if (event.type == EventType::MoveUsed) {
            assert(event.player == 0);
            return;
        }
    }
    assert(false);
}

void testBurnAndPoisonResidualDamage() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    auto battle = makeBattle(data, p1, p2);
    battle.active(0)->setMove(0, &data.move("Aiguisage"));
    battle.active(1)->setMove(0, &data.move("Aiguisage"));

    assert(battle.active(0)->setStatus(StatusCondition::Burn));
    assert(battle.active(1)->setStatus(StatusCondition::Poison));
    const int hp0 = battle.active(0)->currentHP();
    const int hp1 = battle.active(1)->currentHP();
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->currentHP() < hp0);
    assert(battle.active(1)->currentHP() < hp1);
}

void testBurnReducesPhysicalDamageAndGutsOverridesIt() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon attacker(&data.species("Salameche"), 50);
    Pokemon defender(&data.species("Carapuce"), 50);
    p1.addPokemon(attacker);
    p2.addPokemon(defender);
    Battle battle(p1, p2, 10);
    const auto& move = data.move("Griffe");

    const double normal = DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), move, battle, 0, 1);
    battle.active(0)->setStatus(StatusCondition::Burn);
    const double burned = DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), move, battle, 0, 1);
    battle.active(0)->setAbility(Ability::Guts);
    const double guts = DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), move, battle, 0, 1);
    assert(burned < normal);
    assert(guts > normal);
}

void testLevitateImmunity() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon attacker(&data.species("Salameche"), 50);
    Pokemon defender(&data.species("Carapuce"), 50);
    defender.setAbility(Ability::Levitate);
    MoveData groundMove{"Test Sol", Type::Ground, 80, MoveCategory::Physical, 100, 0, 10};
    p1.addPokemon(attacker);
    p2.addPokemon(defender);
    Battle battle(p1, p2, 2);
    assert(DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), groundMove, battle, 0, 1) == 0.0);
}

void testDrainAndRecoil() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon grass(&data.species("Bulbizarre"), 50);
    Pokemon water(&data.species("Carapuce"), 50);
    grass.setMove(0, &data.move("Mega-Sangsue"));
    grass.damage(40);
    water.setMove(0, &data.move("Aiguisage"));
    p1.addPokemon(grass);
    p2.addPokemon(water);
    Battle battle(p1, p2, 6);
    const int beforeDrain = battle.active(0)->currentHP();
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->currentHP() > beforeDrain);

    Trainer r1("R1");
    Trainer r2("R2");
    Pokemon recoilUser(&data.species("Salameche"), 50);
    Pokemon target(&data.species("Carapuce"), 50);
    MoveData recoilMove{"Recul Test", Type::Normal, 90, MoveCategory::Physical, -1, 0, 20};
    recoilMove.recoilPercent = 25;
    recoilUser.setMove(0, &recoilMove);
    target.setMove(0, &data.move("Aiguisage"));
    r1.addPokemon(recoilUser);
    r2.addPokemon(target);
    Battle recoilBattle(r1, r2, 6);
    const int beforeRecoil = recoilBattle.active(0)->currentHP();
    recoilBattle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(recoilBattle.active(0)->currentHP() < beforeRecoil);
}

void testMultiHitConsumesOnePP() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon attacker(&data.species("Salameche"), 50);
    Pokemon defender(&data.species("Carapuce"), 50);
    MoveData multiMove{"Multi Test", Type::Normal, 18, MoveCategory::Physical, -1, 0, 15};
    multiMove.minHits = 2;
    multiMove.maxHits = 5;
    attacker.setMove(0, &multiMove);
    defender.setMove(0, &data.move("Aiguisage"));
    p1.addPokemon(attacker);
    p2.addPokemon(defender);
    Battle battle(p1, p2, 4);
    const int pp = battle.active(0)->moves()[0]->currentPP();
    const auto events = battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->moves()[0]->currentPP() == pp - 1);
    bool sawMultiHit = false;
    for (const auto& event : events) {
        if (event.type == EventType::Text && event.value >= 2 && event.value <= 5) sawMultiHit = true;
    }
    assert(sawMultiHit);
}

void testStatusEffectAndImmunity() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon attacker(&data.species("Carapuce"), 50);
    Pokemon defender(&data.species("Bulbizarre"), 50);
    MoveData burnMove{"Brulure Test", Type::Neutral, 0, MoveCategory::Status, -1, 0, 10};
    MoveEffect burn;
    burn.kind = EffectKind::ApplyStatus;
    burn.target = Target::Opponent;
    burn.status = StatusCondition::Burn;
    burnMove.effects.push_back(burn);
    attacker.setMove(0, &burnMove);
    defender.setMove(0, &data.move("Aiguisage"));
    p1.addPokemon(attacker);
    p2.addPokemon(defender);
    Battle battle(p1, p2, 11);
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(1)->status() == StatusCondition::Burn);

    Trainer f1("F1");
    Trainer f2("F2");
    Pokemon source(&data.species("Carapuce"), 50);
    Pokemon fire(&data.species("Salameche"), 50);
    source.setMove(0, &burnMove);
    fire.setMove(0, &data.move("Aiguisage"));
    f1.addPokemon(source);
    f2.addPokemon(fire);
    Battle immunityBattle(f1, f2, 11);
    immunityBattle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(immunityBattle.active(1)->status() == StatusCondition::None);
}

void testSleepBlocksOneTurn() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon sleeper(&data.species("Salameche"), 50);
    Pokemon other(&data.species("Carapuce"), 50);
    sleeper.setMove(0, &data.move("Griffe"));
    other.setMove(0, &data.move("Aiguisage"));
    sleeper.setStatus(StatusCondition::Sleep, 1);
    p1.addPokemon(sleeper);
    p2.addPokemon(other);
    Battle battle(p1, p2, 8);
    const int pp = battle.active(0)->moves()[0]->currentPP();
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->moves()[0]->currentPP() == pp);
    assert(battle.active(0)->status() == StatusCondition::None);
}

void testParalysisSlowsPokemon() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon water(&data.species("Carapuce"), 50);
    Pokemon fire(&data.species("Salameche"), 50);
    water.setMove(0, &data.move("Griffe"));
    fire.setMove(0, &data.move("Griffe"));
    fire.setStatus(StatusCondition::Paralysis);
    p1.addPokemon(water);
    p2.addPokemon(fire);
    Battle battle(p1, p2, 9);
    const auto events = battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    for (const auto& event : events) {
        if (event.type == EventType::MoveUsed) {
            assert(event.player == 0);
            return;
        }
    }
    assert(false);
}

void testLifeOrbBoostAndRecoil() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon attacker(&data.species("Salameche"), 50);
    Pokemon defender(&data.species("Carapuce"), 50);
    MoveData move{"Orbe Test", Type::Normal, 60, MoveCategory::Physical, -1, 0, 10};
    attacker.setMove(0, &move);
    defender.setMove(0, &data.move("Aiguisage"));
    p1.addPokemon(attacker);
    p2.addPokemon(defender);
    Battle battle(p1, p2, 13);
    const double normal = DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), move, battle, 0, 1);
    battle.active(0)->setHeldItem(HeldItem::LifeOrb);
    const double boosted = DamageCalculator::rawDamage(*battle.active(0), *battle.active(1), move, battle, 0, 1);
    assert(boosted > normal);
    const int hp = battle.active(0)->currentHP();
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->currentHP() < hp);
}

void testHeldItemsAndAbilities() {
    GameData data;
    Trainer p1("P1");
    Trainer p2("P2");
    Pokemon left(&data.species("Carapuce"), 50);
    Pokemon other(&data.species("Bulbizarre"), 50);
    left.setHeldItem(HeldItem::Leftovers);
    left.damage(30);
    left.setMove(0, &data.move("Aiguisage"));
    other.setMove(0, &data.move("Aiguisage"));
    p1.addPokemon(left);
    p2.addPokemon(other);
    Battle battle(p1, p2, 12);
    const int hp = battle.active(0)->currentHP();
    battle.resolveTurn({ActionType::Move, 0}, {ActionType::Move, 0});
    assert(battle.active(0)->currentHP() > hp);

    Pokemon blaze(&data.species("Salameche"), 50);
    Pokemon target(&data.species("Bulbizarre"), 50);
    Trainer a("A");
    Trainer b("B");
    a.addPokemon(blaze);
    b.addPokemon(target);
    Battle abilityBattle(a, b, 5);
    const double normal = DamageCalculator::rawDamage(
        *abilityBattle.active(0), *abilityBattle.active(1), data.move("Flammeche"), abilityBattle, 0, 1);
    abilityBattle.active(0)->setAbility(Ability::Blaze);
    abilityBattle.active(0)->setHP(abilityBattle.active(0)->maxHP() / 3);
    const double boosted = DamageCalculator::rawDamage(
        *abilityBattle.active(0), *abilityBattle.active(1), data.move("Flammeche"), abilityBattle, 0, 1);
    assert(boosted > normal);
}
}

int main() {
    testIndependentMovePP();
    testPriorityBeforeSpeed();
    testBurnAndPoisonResidualDamage();
    testBurnReducesPhysicalDamageAndGutsOverridesIt();
    testLevitateImmunity();
    testDrainAndRecoil();
    testMultiHitConsumesOnePP();
    testStatusEffectAndImmunity();
    testSleepBlocksOneTurn();
    testParalysisSlowsPokemon();
    testLifeOrbBoostAndRecoil();
    testHeldItemsAndAbilities();

    std::cout << "Tous les tests v0.5 sont passes.\n";
    return 0;
}
