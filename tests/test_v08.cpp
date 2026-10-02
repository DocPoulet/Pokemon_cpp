#include "pokemon/GameData.hpp"
#include "pokemon/TeamBuilder.hpp"

#include <cassert>
#include <iostream>

using namespace pokemon;

int main() {
    GameData data;

    // Snapshot officiel Showdown embarque dans la v0.8.
    assert(data.moveNames().size() == 951);
    assert(data.abilityNames().size() == 317);
    assert(data.itemNames().size() == 580);

    // Attaques representatives de plusieurs generations et statuts de catalogue.
    assert(data.hasMove("Earthquake"));
    assert(data.hasMove("Tera Blast"));
    assert(data.hasMove("Hidden Power Ice"));
    assert(data.hasMove("G-Max Wildfire"));
    assert(data.move("Earthquake").power == 100);
    assert(data.move("Tera Blast").type == Type::Normal);
    assert(data.move("Brave Bird").recoilPercent == 33);
    assert(data.move("Giga Drain").drainPercent == 50);
    assert(data.move("Bullet Seed").minHits == 2);
    assert(data.move("Bullet Seed").maxHits == 5);

    // Les entrees connues mais non encore simulees restent valides et referencables.
    assert(data.hasAbility("Intimidate"));
    assert(data.hasAbility("Magic Guard"));
    assert(!data.ability("Intimidate").implemented());
    assert(data.hasItem("Choice Scarf"));
    assert(data.hasItem("Booster Energy"));
    assert(!data.item("Choice Scarf").implemented());

    // Les mecanismes deja implementes restent relies aux definitions globales.
    assert(data.ability("Overgrow").implemented());
    assert(data.item("Life Orb").implemented());
    assert(data.ability("PrimordialSea").id == "Primordial Sea");

    // Les especes existantes resolvent toujours leur movepool vers le catalogue global.
    const auto& bulbasaur = data.species("Bulbasaur");
    bool foundEnergyBall = false;
    for (const MoveData* move : bulbasaur.movePool()) {
        if (move && move->name == "Energy Ball") {
            assert(move == &data.move("Energy Ball"));
            foundEnergyBall = true;
        }
    }
    assert(foundEnergyBall);

    std::cout << "Tous les tests v0.8 sont passes.\n";
    return 0;
}
