#include "Terrain.hpp"
#include "Workshop_POO_pt3.hpp"

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
    for(int i=0; i<2; i++){
        Creature* c= combat.getActive(i);
        Objet* obj = c ? c->getObjet() : nullptr;
        if (obj && obj->getNom() == "Lunettes Filtre")
            continue;
        int t1=0;
        for (auto t : c->getTypes()){
            if (t != TypeEnum::ROCHE && t != TypeEnum::SOL && t != TypeEnum::ACIER)
                t1+=1;
            }
        if(t1==c->getTypes().size())
            c->setPV(c->getPV() - c->calculStat(PV)/16);
    }
}

void effetNeige(Combat& combat){

}

void effetTerrain(Combat& combat){
    switch (combat.meteoAct){
        case Meteo::TempeteDeSable:
            effetSable(combat);
            break;
        
        case Meteo::TempeteDeNeige:
            effetNeige(combat);
            break;
    }
}