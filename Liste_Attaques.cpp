#include "Liste_Attaques.hpp"

// Attaque("Nom", TYPE, puissance, categorie, precision, critique, pp, pp max, effets, contrecoup)
// categorie : 0 = statut, 1 = physique, 2 = special
// effet : {0=stat/1=precision, stat, stages, cible, probabilite}

Attaque lutte("Lutte", NEUTRE, 50, 1, -1, 0, 1, 1);

Attaque flammeche("Flammeche", FEU, 40, 2, 100, 0, 25, 40);
Attaque griffe("Griffe", NORMAL, 40, 1, 100, 0, 35, 56);
Attaque pistolet_a_O("Pistolet a O", EAU, 40, 2, 100, 0, 25, 40);
Attaque fouet_lianes("Fouet Lianes", PLANTE, 45, 1, 100, 0, 25, 40);

Attaque close_combat("Close Combat", COMBAT, 120, 1, 100, 0, 5, 8, {
    {0, DEF, -1, false, 100},
    {0, DEFSP, -1, false, 100}
});

Attaque aiguisage("Aiguisage", ACIER, 0, 0, -1, 0, 15, 24, {
    {0, ATK, 1, false, 100},
    {1, PRE, 1, false, 100}
});

Attaque psyko("Psyko", PSY, 90, 2, 100, 0, 10, 16, {
    {0, DEFSP, -1, true, 10}
});

Attaque acupression("Acupression", PSY, 0, 0, -1, 0, 30, 48);
Attaque zenith("Zenith", FEU, 0, 0, -1, 0, 5, 8);
Attaque dansepluie("Danse-Pluie", EAU, 0, 0, -1, 0, 5, 8);
