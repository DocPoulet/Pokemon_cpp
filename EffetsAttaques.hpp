#pragma once
#include <cstdlib>
#include <iostream>

#include "Workshop_POO_pt3.hpp"

void appliquerAcupression(Creature& attaquant, Combat& combat);
void appliquerEffets(const Attaque& atk, Creature& attaquant, Creature& defenseur, Combat& combat);