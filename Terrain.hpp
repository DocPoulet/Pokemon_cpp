#pragma once

#include <string>

class Combat;

enum class Champ {
    Aucun,
    Electrifie,
    Brumeux,
    Herbu,
    Psychique
};

enum class Meteo {
    Aucune,
    Soleil,
    Pluie,
    TempeteDeSable,
    TempeteDeNeige,
    PluieBattante,
    VentMysterieux,
    SoleilIntense
};

std::string ChampString(Champ s);
std::string MeteoString(Meteo s);
void effetTerrain(Combat& combat);
