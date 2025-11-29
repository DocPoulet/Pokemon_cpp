#include "EffetsAttaques.hpp"

// Comme EffetStat est défini dans le .cpp principal, on l’inclut après Workshop_POO_pt3.hpp
// Toutes les autres fonctions/méthodes sont appelées dynamiquement via leurs déclarations

void atkAcupression(Creature& attaquant, Combat& combat)
{
    StatIndex r = (StatIndex)(rand() % 5 + 1); // ATK -> VIT

    bool aP1 = (&attaquant == combat.getActiveP1());
    if (aP1)
        combat.changeStageP1(r, +2);
    else
        combat.changeStageP2(r, +2);

    std::cout << attaquant.getNom() << " voit son "
              << toString(r) << " fortement augmenter !" << std::endl;
}

void atkLutte(Creature& attaquant)
{
    attaquant.setPV(attaquant.getPV() - attaquant.calculStat(PV) / 4);
    std::cout << attaquant.getNom() << " est blessé par le contrecoup !";
}

void atkPsyko(Creature& defenseur, Combat& combat)
{
    bool aP1 = (&defenseur == combat.getActiveP1());
    if (aP1)
        combat.changeStageP1(DEFSP, -1);
    else
        combat.changeStageP2(DEFSP, -1);

    std::cout << defenseur.getNom() << " voit sa Défense Spéciale diminuer !" << std::endl;
}

bool verifMeteo(Combat& combat){
    if(combat.meteoAct==Meteo::PluieBattante || 
       combat.meteoAct==Meteo::VentMysterieux ||
       combat.meteoAct==Meteo::SoleilIntense) return 1;
    return 0;
}

void appliquerSoleil(Combat& combat, Creature& attaquant){
    if(verifMeteo(combat)){
        std::cout<<"Vous ne pouvez pas installer le Soleil"<<std::endl;
        return;}

    combat.meteoAct=Meteo::Soleil;
    std::cout<<"Le soleil s'installe"<<std::endl;
    Creature *act = (combat.getActiveP1() == &attaquant) ? &attaquant : combat.getActiveP2();
    Objet* obj = act ? act->getObjet() : nullptr;
    if (obj && obj->getNom() == "Roche Chaude") {
        combat.dureeMeteo = 8;
    } else {
        combat.dureeMeteo = 5;
    }
}

void appliquerPluie(Combat& combat, Creature& attaquant){
    if(verifMeteo(combat)) {
        std::cout<<"Vous ne pouvez pas installer la Pluie"<<std::endl;
        return;}

    combat.meteoAct=Meteo::Pluie;
    Creature *act=(combat.getActiveP1()==&attaquant) ? &attaquant : combat.getActiveP2();
    if(act->getObjet()->getNom()=="Roche Humide")
    combat.dureeMeteo=8;
    else combat.dureeMeteo=5;
}

void appliquerEffets(const Attaque& atk, Creature& attaquant, Creature& defenseur, Combat& combat)
{
    for (auto& e : atk.getEffets())
    {
        if(e.proba >= 100) continue;
        int roll = std::rand() % 100;
        if (roll >= e.proba)
            return;
    }
    
    if (atk.getNom() == "Acupression") {
        atkAcupression(attaquant, combat);
        return;
    }
    if (atk.getNom() == "Lutte") {
        atkLutte(attaquant);
        return;
    }

    if(atk.getNom()=="Psyko") {
        atkPsyko(defenseur, combat);
        return;
    }
    
    if (atk.getNom() == "Danse-Pluie") {
        combat.meteoAct = Meteo::Pluie;
        combat.dureeMeteo = 5;
        return;
    }

    if (atk.getNom() == "Zenith") {
        appliquerSoleil(combat, attaquant);
        return;
    }

    if (atk.getNom() == "Champ Herbu") {
        combat.champAct = Champ::Herbu;
        combat.dureeChamp = 5;
        return;
    }

    if(atk.getEffets().empty()) return;
    // Effets généraux
    for (auto& e : atk.getEffets())
    {
        Creature* cible = e.cible ? &defenseur : &attaquant;
        
        bool p1 = (cible == combat.getActiveP1());

        if (e.estPrecision == 0)
        {
            // ---- Effet sur stat normale ----
            StatIndex s = static_cast<StatIndex>(e.stat);

            if (p1)
                combat.changeStageP1(s, e.stages);
            else
                combat.changeStageP2(s, e.stages);
            std::string det = (s == ATK || s == ATKSP) ? "l" : "la ";
            std::cout <<det<<toString(s)<<" de "<< cible->getNom()
                      << (e.stages > 0 ? " augmente" : " diminue")
                      << (std::abs(e.stages) > 2 ?  "énormément" : (std::abs(e.stages) > 1 ? "beaucoup" : "") ) << std::endl;
        }
        else
        {
            // ---- Effet sur précision / esquive ----
            StatPrecision sp = static_cast<StatPrecision>(e.stat);

            if (p1)
                combat.changePrecisionP1(sp, e.stages);
            else
                combat.changePrecisionP2(sp, e.stages);

            std::cout <<"L'attaque de "<< cible->getNom()
                      << (e.stages > 0 ? " augmente" : " diminue")
                      << (std::abs(e.stages) > 2 ?  "énormément" : (std::abs(e.stages) > 1 ? "beaucoup" : "") ) << std::endl;
        }
    }
}