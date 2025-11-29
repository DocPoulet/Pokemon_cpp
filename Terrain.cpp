#include "Terrain.hpp"

std::string ChampString(Champ s) {
    switch(s) {
        case Champ::Aucun:   return "Aucun";
        case Champ::Electrifie:   return "Electrifie";
        case Champ::Brumeux: return "Brumeux";
        case Champ::Herbu: return "Herbu";
        case Champ::Psychique:   return "Psychique";
    }
    return "Inconnu";
}

std::string MeteoString(Meteo s) {
    switch(s) {
        case Meteo::Aucune:   return "Aucune";
        case Meteo::Soleil:   return "Soleil";
        case Meteo::Pluie: return "Pluie";
        case Meteo::TempeteDeSable: return "Tempete de Sable";
        case Meteo::TempeteDeNeige:   return "Tempete de Neige";
        case Meteo::PluieBattante: return "Pluie Battante";
        case Meteo::VentMysterieux: return "Vent Mysterieux";
        case Meteo::SoleilIntense: return "Soleil Intense";
    }
    return "Inconnu";
}

void effetSable(Combat& combat){
    
    for (auto t : c1.getTypes())
        if (t != enumType::ROCHE || t != enumType::SOL || t != enumType::ACIER)
}

void effetTerrain(Combat& combat){
    switch (combat.meteo)
        case Meteo::TempeteDeSable:
            effetSable();
        
        case Meteo::TempeteDeNeige;
            effetNeige();
}