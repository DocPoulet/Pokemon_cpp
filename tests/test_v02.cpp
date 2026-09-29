#include "Liste_Attaques.hpp"
#include "Workshop_POO_pt3.hpp"

#include <cassert>
#include <iostream>

namespace {
void testActivePokemon() {
    CreatureBase especeA("A", 50, 50, 50, 50, 50, 50, {NORMAL});
    CreatureBase especeB("B", 50, 50, 50, 50, 50, 50, {NORMAL});

    Joueur j1("J1");
    Joueur j2("J2");

    j1.ajouterCreature(Creature(&especeA, 50));
    j1.ajouterCreature(Creature(&especeB, 50));
    j2.ajouterCreature(Creature(&especeA, 50));

    Combat combat(j1, j2);

    assert(combat.getActiveP1() == &j1.getEquipe()[0]);
    combat.setActiveP1(1);
    assert(combat.getActiveP1() == &j1.getEquipe()[1]);
    assert(combat.getActiveIndex(0) == 1);
}

void testPpAreIndependent() {
    CreatureBase espece("Test", 50, 50, 50, 50, 50, 50, {FEU});
    Attaque move("Test Move", FEU, 40, 2, 100, 0, 10, 10);

    Creature a(&espece, 50);
    Creature b(&espece, 50);

    assert(a.assignerSlot(move, 0));
    assert(b.assignerSlot(move, 0));

    a.getSlots()[0]->utiliserPP();

    assert(a.getSlots()[0]->getPP_act() == 9);
    assert(b.getSlots()[0]->getPP_act() == 10);
    assert(move.getPP_act() == 10);
}

void testIvEvBounds() {
    CreatureBase espece("Test", 50, 50, 50, 50, 50, 50, {NORMAL});
    Creature c(&espece, 50);

    c.setIV(ATK, 99);
    c.setIV(DEF, -4);

    assert(c.getIV(ATK) == 31);
    assert(c.getIV(DEF) == 0);

    assert(c.setEV(ATK, 252));
    assert(c.setEV(DEF, 252));

    const bool fullRequest = c.setEV(VIT, 252);
    assert(!fullRequest);
    assert(c.totalEV() == 510);
    assert(c.getEV(VIT) == 6);
}

void testTypeChart() {
    CreatureBase attaquantBase("Feu", 50, 50, 50, 50, 50, 50, {FEU});
    CreatureBase planteBase("Plante", 50, 50, 50, 50, 50, 50, {PLANTE});
    CreatureBase spectreBase("Spectre", 50, 50, 50, 50, 50, 50, {SPECTRE});

    Joueur j1("J1");
    Joueur j2("J2");
    j1.ajouterCreature(Creature(&attaquantBase, 50));
    j2.ajouterCreature(Creature(&planteBase, 50));

    Combat combat(j1, j2);

    assert(getEfficacite(FEU, planteBase.getTypes(), combat) == 2.0);
    assert(getEfficacite(NORMAL, spectreBase.getTypes(), combat) == 0.0);
}

void testStagesAndPvClamp() {
    CreatureBase espece("Test", 50, 50, 50, 50, 50, 50, {NORMAL});
    Joueur j1("J1");
    Joueur j2("J2");

    j1.ajouterCreature(Creature(&espece, 50));
    j2.ajouterCreature(Creature(&espece, 50));

    Combat combat(j1, j2);

    for (int i = 0; i < 20; ++i) combat.changeStageP1(ATK, 1);
    assert(combat.getStageP1(ATK) == 6);

    for (int i = 0; i < 20; ++i) combat.changeStageP1(ATK, -1);
    assert(combat.getStageP1(ATK) == -6);

    Creature& c = j1.getEquipe()[0];
    c.setPV(-999);
    assert(c.getPV() == 0);

    c.setPV(999999);
    assert(c.getPV() == c.calculStat(PV));
}

void testCombatEnd() {
    CreatureBase espece("Test", 50, 50, 50, 50, 50, 50, {NORMAL});

    Joueur j1("J1");
    Joueur j2("J2");
    j1.ajouterCreature(Creature(&espece, 50));
    j2.ajouterCreature(Creature(&espece, 50));

    Combat combat(j1, j2);
    assert(!combatTermine(combat));

    j2.getEquipe()[0].setPV(0);
    assert(combatTermine(combat));
}
}

int main() {
    seedRng(42);

    testActivePokemon();
    testPpAreIndependent();
    testIvEvBounds();
    testTypeChart();
    testStagesAndPvClamp();
    testCombatEnd();

    std::cout << "Tous les tests v0.2 sont passes.\n";
    return 0;
}
