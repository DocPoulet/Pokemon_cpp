#include "Liste_Attaques.hpp"

Attaque lutte("Lutte", NEUTRE, 50, 1, -1, 0, 1, 1);

Attaque flammeche("Flammèche", FEU, 40, 2, 100, 0, 40, 48);
Attaque griffe("Griffe", NORMAL, 35, 1);
Attaque pistolet_a_O("Pistolet à O", EAU, 40, 2);
Attaque fouet_lianes("Fouet Lianes", PLANTE, 45, 1);
Attaque close_combat("Close Combat", COMBAT, 120, 1, 100, 0, 5, 8, {
    {0, DEF, -1, 0},
    {0, DEFSP, -1, 0}
});
Attaque aiguisage("Aiguisage", ACIER, 0, 0, -1, 0, 15, 24, {
    {0, ATK, 1, 0},
    {1, PRE, 1, 0}
});
Attaque psyko("Psyko", PSY, 90, 2, 100, 0, 10, 16, {
    {0, DEFSP, -1, 1}
});
Attaque acupression("Acupression", PSY, 0, 0, -1, 0, 30, 48);