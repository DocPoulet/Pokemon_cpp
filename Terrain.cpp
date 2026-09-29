#include "Terrain.hpp"
#include "Workshop_POO_pt3.hpp"

#include <algorithm>
#include <iostream>

std::string ChampString(Champ s) {
    switch (s) {
        case Champ::Aucun: return "Aucun";
        case Champ::Electrifie: return "Electrifie";
        case Champ::Brumeux: return "Brumeux";
        case Champ::Herbu: return "Herbu";
        case Champ::Psychique: return "Psychique";
    }
    return "Inconnu";
}

std::string MeteoString(Meteo s) {
    switch (s) {
        case Meteo::Aucune: return "Aucune";
        case Meteo::Soleil: return "Soleil";
        case Meteo::Pluie: return "Pluie";
        case Meteo::TempeteDeSable: return "Tempete de Sable";
        case Meteo::TempeteDeNeige: return "Tempete de Neige";
        case Meteo::PluieBattante: return "Pluie Battante";
        case Meteo::VentMysterieux: return "Vent Mysterieux";
        case Meteo::SoleilIntense: return "Soleil Intense";
    }
    return "Inconnu";
}

namespace {
void effetSable(Combat& combat) {
    for (int i = 0; i < 2; ++i) {
        Creature* c = combat.getActive(i);
        if (!c || c->estKO()) continue;

        const Objet* obj = c->getObjet();
        if (obj && obj->getNom() == "Lunettes Filtre") continue;

        bool immune = false;
        for (TypeEnum t : c->getTypes()) {
            if (t == ROCHE || t == SOL || t == ACIER) {
                immune = true;
                break;
            }
        }

        if (!immune) {
            const int degats = std::max(1, c->calculStat(PV) / 16);
            c->setPV(c->getPV() - degats);
            std::cout << c->getNom()
                      << " est endommage par la tempete de sable !\n";
        }
    }
}
}

void effetTerrain(Combat& combat) {
    if (combat.meteoAct == Meteo::TempeteDeSable) {
        effetSable(combat);
    }
}
