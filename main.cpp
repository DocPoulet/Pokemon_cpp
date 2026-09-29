#include "Liste_Attaques.hpp"
#include "Pokedex_1g.hpp"
#include "Workshop_POO_pt3.hpp"

int main() {
    Joueur docPoulet("DocPoulet");
    Joueur frodon("Frodon");
    Combat combat(docPoulet, frodon);

    Creature salameche1(&salameche, 5, "Hardi");
    Creature carapuce1(&carapuce, 5, "Modeste");
    Creature salameche2(&salameche, 5, "Timide");
    Creature carapuce2(&carapuce, 5, "Assuré");

    salameche.ajouterAttaque(flammeche);
    salameche.ajouterAttaque(griffe);
    salameche.ajouterAttaque(pistolet_a_O);
    salameche.ajouterAttaque(fouet_lianes);
    salameche.ajouterAttaque(close_combat);
    salameche.ajouterAttaque(aiguisage);
    salameche.ajouterAttaque(psyko);
    salameche.ajouterAttaque(acupression);
    salameche.ajouterAttaque(zenith);

    carapuce.ajouterAttaque(pistolet_a_O);
    carapuce.ajouterAttaque(fouet_lianes);
    carapuce.ajouterAttaque(acupression);

    salameche1.assignerSlot(zenith, 0);
    salameche1.assignerSlot(flammeche, 1);
    salameche1.assignerSlot(aiguisage, 2);
    salameche1.assignerSlot(close_combat, 3);

    salameche2.assignerSlot(flammeche, 0);
    salameche2.assignerSlot(griffe, 1);
    salameche2.assignerSlot(fouet_lianes, 2);

    carapuce1.assignerSlot(fouet_lianes, 0);
    carapuce1.assignerSlot(pistolet_a_O, 1);

    carapuce2.assignerSlot(fouet_lianes, 0);
    carapuce2.assignerSlot(pistolet_a_O, 1);

    docPoulet.ajouterCreature(salameche1);
    docPoulet.ajouterCreature(carapuce2);

    frodon.ajouterCreature(salameche2);
    frodon.ajouterCreature(carapuce1);

    combat.setActiveP1(0);
    combat.setActiveP2(0);

    menuCombat(combat);
    return 0;
}
