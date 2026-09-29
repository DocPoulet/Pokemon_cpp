#include "EffetsAttaques.hpp"

#include <cmath>
#include <iostream>

namespace {
void atkAcupression(Creature& attaquant, Combat& combat) {
    const auto stat = static_cast<StatIndex>(randomInt(ATK, VIT));

    const int joueurIdx = combat.getJoueurIndex(&attaquant);
    if (joueurIdx >= 0) {
        combat.changeStage(joueurIdx, stat, +2);
    }

    std::cout << attaquant.getNom() << " voit son "
              << toString(stat) << " fortement augmenter !\n";
}

void atkLutte(Creature& attaquant) {
    const int recul = std::max(1, attaquant.calculStat(PV) / 4);
    attaquant.setPV(attaquant.getPV() - recul);

    std::cout << attaquant.getNom()
              << " est blesse par le contrecoup de Lutte !\n";
}

bool verifMeteoBloquante(const Combat& combat) {
    return combat.meteoAct == Meteo::PluieBattante ||
           combat.meteoAct == Meteo::VentMysterieux ||
           combat.meteoAct == Meteo::SoleilIntense;
}

void appliquerSoleil(Combat& combat, Creature& attaquant) {
    if (verifMeteoBloquante(combat)) {
        std::cout << "Le Soleil ne peut pas remplacer cette meteo.\n";
        return;
    }

    combat.meteoAct = Meteo::Soleil;
    combat.dureeMeteo =
        attaquant.getObjet() && attaquant.getObjet()->getNom() == "Roche Chaude"
            ? 8
            : 5;

    std::cout << "Le soleil s'installe.\n";
}

void appliquerPluie(Combat& combat, Creature& attaquant) {
    if (verifMeteoBloquante(combat)) {
        std::cout << "La Pluie ne peut pas remplacer cette meteo.\n";
        return;
    }

    combat.meteoAct = Meteo::Pluie;
    combat.dureeMeteo =
        attaquant.getObjet() && attaquant.getObjet()->getNom() == "Roche Humide"
            ? 8
            : 5;

    std::cout << "Il commence a pleuvoir.\n";
}
}

void appliquerEffets(const Attaque& atk, Creature& attaquant,
                     Creature& defenseur, Combat& combat) {
    if (atk.getNom() == "Acupression") {
        atkAcupression(attaquant, combat);
        return;
    }

    if (atk.getNom() == "Lutte") {
        atkLutte(attaquant);
        return;
    }

    if (atk.getNom() == "Danse-Pluie") {
        appliquerPluie(combat, attaquant);
        return;
    }

    if (atk.getNom() == "Zenith") {
        appliquerSoleil(combat, attaquant);
        return;
    }

    if (atk.getNom() == "Champ Herbu") {
        combat.champAct = Champ::Herbu;
        combat.dureeChamp = 5;
        std::cout << "Le Champ Herbu s'installe.\n";
        return;
    }

    for (const auto& effet : atk.getEffets()) {
        if (effet.proba < 100 && randomInt(1, 100) > effet.proba) {
            continue;
        }

        Creature* cible = effet.cible ? &defenseur : &attaquant;
        const int joueurIdx = combat.getJoueurIndex(cible);
        if (joueurIdx < 0) continue;

        if (effet.estPrecision == 0) {
            const auto stat = static_cast<StatIndex>(effet.stat);
            combat.changeStage(joueurIdx, stat, effet.stages);

            std::cout << toString(stat) << " de " << cible->getNom()
                      << (effet.stages > 0 ? " augmente" : " diminue");

            if (std::abs(effet.stages) >= 2) std::cout << " beaucoup";
            std::cout << ".\n";
        } else {
            const auto stat = static_cast<StatPrecision>(effet.stat);
            combat.changePrecision(joueurIdx, stat, effet.stages);

            const char* nomStat = stat == PRE ? "Precision" : "Esquive";
            std::cout << nomStat << " de " << cible->getNom()
                      << (effet.stages > 0 ? " augmente" : " diminue")
                      << ".\n";
        }
    }
}
