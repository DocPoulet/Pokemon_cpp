#include "Workshop_POO_pt3.hpp"

#include "EffetsAttaques.hpp"
#include "Liste_Attaques.hpp"

#include <cctype>
#include <limits>
#include <random>

int tour = 1;

const int MAX_EQUIPE = 6;
const int MIN_MOD = -6;
const int MAX_MOD = 6;

namespace {
std::mt19937& rng() {
    static std::mt19937 engine{std::random_device{}()};
    return engine;
}
}

void seedRng(unsigned int seed) {
    rng().seed(seed);
}

int randomInt(int minInclusive, int maxInclusive) {
    std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
    return dist(rng());
}

std::string toString(StatIndex s) {
    switch (s) {
        case PV: return "PV";
        case ATK: return "Attaque";
        case DEF: return "Defense";
        case ATKSP: return "Attaque Speciale";
        case DEFSP: return "Defense Speciale";
        case VIT: return "Vitesse";
        default: return "Inconnu";
    }
}

const std::map<std::string, Nature> NATURES = {
    {"Hardi", Nature("Hardi", ATK, ATK)},
    {"Solo", Nature("Solo", ATK, DEF)},
    {"Rigide", Nature("Rigide", ATK, ATKSP)},
    {"Mauvais", Nature("Mauvais", ATK, DEFSP)},
    {"Brave", Nature("Brave", ATK, VIT)},

    {"Assure", Nature("Assure", DEF, ATK)},
    {"Assuré", Nature("Assuré", DEF, ATK)},
    {"Docile", Nature("Docile", DEF, DEF)},
    {"Malin", Nature("Malin", DEF, ATKSP)},
    {"Lache", Nature("Lache", DEF, DEFSP)},
    {"Lâche", Nature("Lâche", DEF, DEFSP)},
    {"Relax", Nature("Relax", DEF, VIT)},

    {"Modeste", Nature("Modeste", ATKSP, ATK)},
    {"Doux", Nature("Doux", ATKSP, DEF)},
    {"Pudique", Nature("Pudique", ATKSP, ATKSP)},
    {"Foufo", Nature("Foufo", ATKSP, DEFSP)},
    {"Discret", Nature("Discret", ATKSP, VIT)},

    {"Calme", Nature("Calme", DEFSP, ATK)},
    {"Gentil", Nature("Gentil", DEFSP, DEF)},
    {"Prudent", Nature("Prudent", DEFSP, ATKSP)},
    {"Bizarre", Nature("Bizarre", DEFSP, DEFSP)},
    {"Malpoli", Nature("Malpoli", DEFSP, VIT)},

    {"Timide", Nature("Timide", VIT, ATK)},
    {"Presse", Nature("Presse", VIT, DEF)},
    {"Pressé", Nature("Pressé", VIT, DEF)},
    {"Jovial", Nature("Jovial", VIT, ATKSP)},
    {"Naif", Nature("Naif", VIT, DEFSP)},
    {"Naïf", Nature("Naïf", VIT, DEFSP)},
    {"Serieux", Nature("Serieux", VIT, VIT)},
    {"Sérieux", Nature("Sérieux", VIT, VIT)}
};

const std::vector<std::string> nomsTypes = {
    "Acier", "Combat", "Dragon", "Eau", "Electrique", "Fee",
    "Feu", "Glace", "Insecte", "Normal", "Plante", "Poison",
    "Psy", "Roche", "Sol", "Spectre", "Tenebres", "Vol",
    "Neutre"
};

namespace {
// Lignes = type defenseur, colonnes = type attaque.
constexpr double chart[18][18] = {
//   Aci  Com  Dra  Eau  Ele  Fee  Feu  Gla  Ins  Nor  Pla  Poi  Psy  Roc  Sol  Spe  Ten  Vol
    {0.5, 2.0, 0.5, 1.0, 1.0, 0.5, 2.0, 0.5, 0.5, 0.5, 0.5, 0.0, 0.5, 0.5, 2.0, 1.0, 1.0, 0.5},
    {1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 0.5, 2.0},
    {1.0, 1.0, 2.0, 0.5, 0.5, 2.0, 0.5, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {0.5, 1.0, 1.0, 0.5, 2.0, 1.0, 0.5, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
    {0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5},
    {2.0, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 0.5, 1.0},
    {0.5, 1.0, 1.0, 2.0, 1.0, 0.5, 0.5, 0.5, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 2.0, 1.0, 1.0, 1.0},
    {2.0, 2.0, 1.0, 1.0, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 1.0},
    {1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 2.0, 0.5, 1.0, 1.0, 2.0},
    {1.0, 2.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0},
    {1.0, 1.0, 1.0, 0.5, 0.5, 1.0, 2.0, 2.0, 2.0, 1.0, 0.5, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 2.0},
    {1.0, 0.5, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 0.5, 1.0, 0.5, 0.5, 2.0, 1.0, 2.0, 1.0, 1.0, 1.0},
    {1.0, 0.5, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.5, 1.0, 1.0, 2.0, 2.0, 1.0},
    {2.0, 2.0, 1.0, 2.0, 1.0, 1.0, 0.5, 1.0, 1.0, 0.5, 2.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 0.5},
    {1.0, 1.0, 1.0, 2.0, 0.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 1.0, 1.0},
    {1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.5, 0.0, 1.0, 0.5, 1.0, 1.0, 1.0, 2.0, 2.0, 1.0},
    {1.0, 2.0, 1.0, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.5, 0.5, 1.0},
    {1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 0.0, 1.0, 1.0, 1.0}
};

bool critique(int bonus) {
    int denominateur = 24;
    if (bonus == 1) denominateur = 8;
    else if (bonus == 2) denominateur = 2;
    else if (bonus >= 3) denominateur = 1;

    return randomInt(1, denominateur) == 1;
}

bool touche(const Attaque& atk, Creature& attaquant, Combat& combat) {
    if (atk.getPrecision() < 0) return true;

    const int attaquantIdx = combat.getJoueurIndex(&attaquant);
    if (attaquantIdx < 0) return true;

    const int defenseurIdx = 1 - attaquantIdx;
    const double preMult = combat.getPRE(attaquantIdx, PRE);
    const double evaMult = combat.getPRE(defenseurIdx, AVD);

    const double chance =
        std::clamp(atk.getPrecision() * (preMult / evaMult), 0.0, 100.0);

    return randomInt(1, 100) <= chance;
}

bool aUneAttaqueUtilisable(const Creature& c) {
    for (const auto& slot : c.getSlots()) {
        if (slot && slot->getPP_act() > 0) return true;
    }
    return false;
}

void consommerPP(Attaque& atk) {
    if (atk.getNom() != "Lutte") atk.utiliserPP();
}
}

double getEfficacite(TypeEnum attaque, const std::vector<TypeEnum>& defenseurs,
                     const Combat& combat) {
    const int atkType = static_cast<int>(attaque);
    if (atkType < 0 || atkType >= 18) return 1.0;

    double total = 1.0;

    for (TypeEnum def : defenseurs) {
        const int defType = static_cast<int>(def);
        if (defType < 0 || defType >= 18) continue;

        double mult = chart[defType][atkType];

        if (combat.meteoAct == Meteo::VentMysterieux &&
            def == VOL && mult > 1.0) {
            mult = 1.0;
        }

        total *= mult;
    }

    return total;
}

double calculerDegatsPur(const Creature& attaquant, const Creature& defenseur,
                         const Attaque& atk, Combat& combat) {
    const double efficacite = getEfficacite(atk.getType(), defenseur.getTypes(), combat);

    if (efficacite == 0.0 || atk.getCategorie() == 0 || atk.getPuissance() <= 0) {
        return 0.0;
    }

    const int attaquantIdx = combat.getJoueurIndex(&attaquant);
    if (attaquantIdx < 0) return 0.0;

    const int defenseurIdx = 1 - attaquantIdx;

    double atkMod = 1.0;
    double defMod = 1.0;
    double atkStat = 1.0;
    double defStat = 1.0;

    if (atk.getCategorie() == 1) {
        atkMod = combat.getStatMultiplier(attaquantIdx, ATK);
        defMod = combat.getStatMultiplier(defenseurIdx, DEF);
        atkStat = attaquant.calculStat(ATK);
        defStat = defenseur.calculStat(DEF);

        if (combat.meteoAct == Meteo::TempeteDeNeige) {
            for (TypeEnum t : defenseur.getTypes()) {
                if (t == GLACE) {
                    defMod *= 1.5;
                    break;
                }
            }
        }
    } else {
        atkMod = combat.getStatMultiplier(attaquantIdx, ATKSP);
        defMod = combat.getStatMultiplier(defenseurIdx, DEFSP);
        atkStat = attaquant.calculStat(ATKSP);
        defStat = defenseur.calculStat(DEFSP);
    }

    atkStat *= atkMod;
    defStat *= defMod;
    defStat = std::max(1.0, defStat);

    double meteo = 1.0;
    if (combat.meteoAct == Meteo::Soleil) {
        if (atk.getType() == FEU || atk.getNom() == "Hydrovapeur") meteo *= 1.5;
        else if (atk.getType() == EAU) meteo *= 0.5;
    } else if (combat.meteoAct == Meteo::Pluie) {
        if (atk.getType() == EAU) meteo *= 1.5;
        else if (atk.getType() == FEU) meteo *= 0.5;
    } else if (combat.meteoAct == Meteo::SoleilIntense) {
        if (atk.getType() == FEU) meteo *= 1.5;
        else if (atk.getType() == EAU) meteo = 0.0;
    } else if (combat.meteoAct == Meteo::PluieBattante) {
        if (atk.getType() == EAU) meteo *= 1.5;
        else if (atk.getType() == FEU) meteo = 0.0;
    }

    double stab = 1.0;
    for (TypeEnum t : attaquant.getTypes()) {
        if (t == atk.getType()) {
            stab = 1.5;
            break;
        }
    }

    const double base =
        ((((attaquant.getLVL() * 0.4) + 2.0) *
          atk.getPuissance() * atkStat / defStat) /
         50.0) +
        2.0;

    return base * meteo * stab * efficacite;
}

void effectuerAttaque(Creature& attaquant, Creature& defenseur,
                      Attaque& atk, Combat& combat) {
    if (attaquant.estKO() || defenseur.estKO()) return;

    std::cout << attaquant.getNom() << " utilise " << atk.getNom() << " !\n";

    if (atk.getNom() != "Lutte" && atk.getPP_act() <= 0) {
        std::cout << "Mais " << atk.getNom() << " n'a plus de PP !\n";
        return;
    }

    if (!touche(atk, attaquant, combat)) {
        consommerPP(atk);
        std::cout << "L'attaque echoue !\n";
        return;
    }

    const int pvAvant = defenseur.getPV();
    const double degatsPurs = calculerDegatsPur(attaquant, defenseur, atk, combat);

    consommerPP(atk);

    int degats = 0;

    if (atk.getCategorie() != 0 && atk.getPuissance() > 0) {
        const double multType = getEfficacite(atk.getType(), defenseur.getTypes(), combat);

        if (multType == 0.0) {
            std::cout << defenseur.getNom() << " est immunise contre l'attaque !\n";
        } else {
            if (multType < 1.0) std::cout << "Ce n'est pas tres efficace...\n";
            if (multType > 1.0) std::cout << "C'est super efficace !\n";

            const double crit = critique(atk.getCrit()) ? 1.5 : 1.0;
            if (crit > 1.0) std::cout << "Coup critique !\n";

            const double aleatoire = randomInt(85, 100) / 100.0;
            degats = std::max(1, static_cast<int>(degatsPurs * crit * aleatoire));
            defenseur.setPV(defenseur.getPV() - degats);

            std::cout << defenseur.getNom() << " perd " << degats << " PV.\n";
        }
    }

    if (degats > 0 || atk.getCategorie() == 0) {
        appliquerEffets(atk, attaquant, defenseur, combat);
    }

    if (atk.getContrecoup() && degats > 0) {
        const int recul = std::max(1, degats / 3);
        attaquant.setPV(attaquant.getPV() - recul);
        std::cout << attaquant.getNom() << " subit " << recul
                  << " PV de contrecoup.\n";
    }

    if (pvAvant > 0 && defenseur.estKO()) {
        std::cout << defenseur.getNom() << " est KO !\n";
    }
}

namespace {
int priorite(Combat& combat) {
    Creature* c1 = combat.getActive(0);
    Creature* c2 = combat.getActive(1);

    if (!c1 || !c2) return 0;

    const int v1 = c1->calculStat(VIT);
    const int v2 = c2->calculStat(VIT);

    if (v1 == v2) return randomInt(0, 1);
    return v1 > v2 ? 0 : 1;
}

Attaque& attaqueAleatoire(Creature& attaquant,
                          const Creature& defenseur,
                          Combat& combat) {
    std::vector<Attaque*> utilisables;

    for (auto& slot : attaquant.getSlots()) {
        if (slot && slot->getPP_act() > 0) {
            utilisables.push_back(&*slot);
        }
    }

    if (utilisables.empty()) return lutte;

    std::vector<int> poids;
    int total = 0;

    for (Attaque* attaque : utilisables) {
        int p = static_cast<int>(
            std::max(0.0, calculerDegatsPur(attaquant, defenseur, *attaque, combat)));

        // Une attaque de statut reste choisissable.
        if (p == 0 && attaque->getCategorie() == 0) p = 5;

        poids.push_back(p);
        total += p;
    }

    if (total <= 0) {
        return *utilisables[static_cast<std::size_t>(
            randomInt(0, static_cast<int>(utilisables.size()) - 1))];
    }

    int tirage = randomInt(1, total);

    for (std::size_t i = 0; i < utilisables.size(); ++i) {
        tirage -= poids[i];
        if (tirage <= 0) return *utilisables[i];
    }

    return *utilisables.back();
}

void afficherAttaques(const Creature& c) {
    const auto& slots = c.getSlots();

    for (std::size_t i = 0; i < slots.size(); ++i) {
        std::cout << (i + 1) << ". ";
        if (!slots[i]) {
            std::cout << "[vide]\n";
            continue;
        }

        std::cout << slots[i]->getNom()
                  << " - PP " << slots[i]->getPP_act()
                  << "/" << slots[i]->getPP() << '\n';
    }

    std::cout << "> ";
}

Attaque& choixAttaque(Creature& c) {
    if (!aUneAttaqueUtilisable(c)) {
        std::cout << c.getNom()
                  << " n'a plus d'attaque utilisable : Lutte est utilisee.\n";
        return lutte;
    }

    while (true) {
        std::cout << "=== Attaques de " << c.getNom() << " ===\n";
        afficherAttaques(c);

        int n = 0;
        if (!(std::cin >> n)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entree invalide.\n";
            continue;
        }

        if (n < 1 || n > 4) {
            std::cout << "Choix invalide.\n";
            continue;
        }

        auto& slot = c.getSlots()[static_cast<std::size_t>(n - 1)];

        if (!slot) {
            std::cout << "Ce slot est vide.\n";
            continue;
        }

        if (slot->getPP_act() <= 0) {
            std::cout << "Cette attaque n'a plus de PP.\n";
            continue;
        }

        return *slot;
    }
}

void resoudreTour(Combat& combat) {
    Creature* p1 = combat.getActiveP1();
    Creature* p2 = combat.getActiveP2();
    if (!p1 || !p2) return;

    Attaque& atkP1 = choixAttaque(*p1);
    Attaque& atkP2 = attaqueAleatoire(*p2, *p1, combat);

    const int premierIdx = priorite(combat);
    const int secondIdx = 1 - premierIdx;

    Creature* premier = combat.getActive(premierIdx);
    Creature* second = combat.getActive(secondIdx);
    if (!premier || !second) return;

    Attaque& premiereAttaque = premierIdx == 0 ? atkP1 : atkP2;
    Attaque& secondeAttaque = secondIdx == 0 ? atkP1 : atkP2;

    effectuerAttaque(*premier, *second, premiereAttaque, combat);

    if (!premier->estKO() && !second->estKO()) {
        effectuerAttaque(*second, *premier, secondeAttaque, combat);
    }

    ++tour;
    effetTerrain(combat);

    if (combat.dureeMeteo > 0) {
        --combat.dureeMeteo;
        if (combat.dureeMeteo == 0) {
            std::cout << "La meteo " << MeteoString(combat.meteoAct)
                      << " se dissipe.\n";
            combat.meteoAct = Meteo::Aucune;
        }
    }

    if (combat.dureeChamp > 0) {
        --combat.dureeChamp;
        if (combat.dureeChamp == 0) {
            std::cout << "Le champ " << ChampString(combat.champAct)
                      << " se dissipe.\n";
            combat.champAct = Champ::Aucun;
        }
    }
}

void afficherMenuCombat() {
    std::cout << "\n=== Combat ===\n"
              << "1. Attaquer\n"
              << "2. Equipe\n"
              << "3. Sac\n"
              << "4. Fuite\n"
              << "> ";
}
}

bool combatTermine(const Combat& combat) {
    return combat.getP1().toutesCreaturesKO() ||
           combat.getP2().toutesCreaturesKO();
}

void switchCombat(Combat& combat) {
    Joueur& joueur = combat.getP1();

    if (joueur.toutesCreaturesKO()) return;

    while (true) {
        std::cout << "Choisissez une creature pour " << joueur.getNom() << " :\n";
        joueur.afficherEquipe();

        int n = 0;
        if (!(std::cin >> n)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entree invalide.\n";
            continue;
        }

        if (n < 1 || n > static_cast<int>(joueur.getEquipe().size())) {
            std::cout << "Choix invalide.\n";
            continue;
        }

        Creature& choisie = joueur.getEquipe()[static_cast<std::size_t>(n - 1)];
        if (choisie.estKO()) {
            std::cout << "Cette creature est KO.\n";
            continue;
        }

        combat.setActiveP1(n - 1);
        std::cout << joueur.getNom() << " envoie "
                  << combat.getActiveP1()->getNom() << " !\n";
        return;
    }
}

void switchBot(Combat& combat, int index) {
    Joueur& bot = combat.getP2();
    if (bot.toutesCreaturesKO()) return;

    if (index >= 0 && index < static_cast<int>(bot.getEquipe().size()) &&
        !bot.getEquipe()[static_cast<std::size_t>(index)].estKO()) {
        combat.setActiveP2(index);
        std::cout << bot.getNom() << " envoie "
                  << combat.getActiveP2()->getNom() << " !\n";
        return;
    }

    Creature* ennemi = combat.getActiveP1();

    int bestIndex = -1;
    double bestScore = -1.0;

    for (std::size_t i = 0; i < bot.getEquipe().size(); ++i) {
        Creature& candidate = bot.getEquipe()[i];
        if (candidate.estKO()) continue;

        double score = 0.0;

        if (ennemi) {
            for (auto& slot : candidate.getSlots()) {
                if (!slot || slot->getPP_act() <= 0) continue;
                score = std::max(
                    score,
                    calculerDegatsPur(candidate, *ennemi, *slot, combat));
            }
        }

        // Un Pokemon sans PP reste valide : il pourra utiliser Lutte.
        if (score >= bestScore) {
            bestScore = score;
            bestIndex = static_cast<int>(i);
        }
    }

    if (bestIndex >= 0) {
        combat.setActiveP2(bestIndex);
        std::cout << bot.getNom() << " envoie "
                  << combat.getActiveP2()->getNom() << " !\n";
    }
}

void menuCombat(Combat& combat) {
    switchBot(combat, 0);

    while (!combatTermine(combat)) {
        Creature* p1 = combat.getActiveP1();
        Creature* p2 = combat.getActiveP2();

        if (!p1 || p1->estKO()) {
            switchCombat(combat);
            if (combatTermine(combat)) break;
        }

        if (!p2 || p2->estKO()) {
            switchBot(combat);
            if (combatTermine(combat)) break;
        }

        p1 = combat.getActiveP1();
        p2 = combat.getActiveP2();

        std::cout << "\n==========================\n"
                  << "=== Tour " << tour << " ===\n"
                  << *p1 << " // " << *p2 << '\n';

        afficherMenuCombat();

        int choix = 0;
        if (!(std::cin >> choix)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entree invalide.\n";
            continue;
        }

        switch (choix) {
            case 1:
                resoudreTour(combat);
                break;

            case 2:
                switchCombat(combat);
                break;

            case 3:
                std::cout << "Le sac n'est pas encore implemente.\n";
                break;

            case 4:
                std::cout << "Vous prenez la fuite.\n";
                return;

            default:
                std::cout << "Choix invalide.\n";
                break;
        }
    }

    if (combat.getP1().toutesCreaturesKO()) {
        std::cout << combat.getP2().getNom() << " a gagne le combat !\n";
    } else if (combat.getP2().toutesCreaturesKO()) {
        std::cout << combat.getP1().getNom() << " a gagne le combat !\n";
    }
}
