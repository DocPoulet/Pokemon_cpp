#include <ctime>
#include <climits>       // pour INT_MIN / INT_MAX
#include <limits>        // pour numeric_limits<streamsize>::max()
#include <optional>

#include "EffetsAttaques.hpp"
#include "Pokedex_1g.hpp"
#include "Liste_Attaques.hpp"

const int MAX_EQUIPE = 6;
const int MIN_MOD = -6;
const int MAX_MOD = 6;

std::string normString(const std::string& raw) {
    std::string s = raw;
    std::replace(s.begin(), s.end(), '_', ' ');
    bool capitalize = true;
    for (char& c : s) {
        if (capitalize && std::isalpha(c)) {
            c = std::toupper(c);
            capitalize = false;
        } else {
            c = std::tolower(c);
        }
        if (c == ' ')
            capitalize = true;
    }
    return s;
}

class Objet {
    std::string nom;
};

std::string toString(StatIndex s) {
    switch(s) {
        case ATK:   return "Attaque";
        case DEF:   return "Défense";
        case ATKSP: return "Attaque Spéciale";
        case DEFSP: return "Défense Spéciale";
        case VIT:   return "Vitesse";
    }
    return "Inconnu";
}


const std::map<std::string, Nature> NATURES = {
    {"Hardi",   Nature("Hardi",   ATK,   ATK)},
    {"Solo",    Nature("Solo",    ATK,   DEF)},
    {"Rigide",  Nature("Rigide",  ATK,   ATKSP)},
    {"Mauvais", Nature("Mauvais", ATK,   DEFSP)},
    {"Brave",   Nature("Brave",   ATK,   VIT)},

    {"Assuré",  Nature("Assuré",  DEF,   ATK)},
    {"Docile",  Nature("Docile",  DEF,   DEF)},
    {"Malin",   Nature("Malin",   DEF,   ATKSP)},
    {"Lâche",   Nature("Lâche",   DEF,   DEFSP)},
    {"Relax",   Nature("Relax",   DEF,   VIT)},

    {"Modeste", Nature("Modeste", ATKSP, ATK)},
    {"Doux",    Nature("Doux",    ATKSP, DEF)},
    {"Pudique", Nature("Pudique", ATKSP, ATKSP)},
    {"Foufo",   Nature("Foufo",   ATKSP, DEFSP)},
    {"Discret", Nature("Discret", ATKSP, VIT)},

    {"Calme",   Nature("Calme",   DEFSP, ATK)},
    {"Gentil",  Nature("Gentil",  DEFSP, DEF)},
    {"Prudent", Nature("Prudent", DEFSP, ATKSP)},
    {"Bizarre", Nature("Bizarre", DEFSP, DEFSP)},
    {"Malpoli", Nature("Malpoli", DEFSP, VIT)},

    {"Timide",  Nature("Timide",  VIT,   ATK)},
    {"Pressé",  Nature("Pressé",  VIT,   DEF)},
    {"Jovial",  Nature("Jovial",  VIT,   ATKSP)},
    {"Naïf",    Nature("Naïf",    VIT,   DEFSP)},
    {"Sérieux", Nature("Sérieux", VIT,   VIT)}
};


void afficherNature(const Nature& n) {
    std::cout << "Nature : " << n.getName() << "\n";
    std::cout << "Bonus : " << toString(n.getIncreased()) << "\n";
    std::cout << "Malus : " << toString(n.getDecreased()) << "\n";
}

const std::vector<std::string> nomsTypes = {
    "Acier", "Combat", "Dragon", "Eau", "Electrique", "Fée",
    "Feu", "Glace", "Insecte", "Normal", "Plante", "Poison",
    "Psy", "Roche", "Sol", "Spectre", "Ténèbres", "Vol",
    "Normal"
};

const double chart[18][18] = {
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
    {1.0, 0.5, 1.0, 1.0, 2.0, 1.0, 1.0, 2.0, 0.5, 1.0, 0.5, 1.0, 1.0, 2.0, 0.0, 1.0, 1.0, 1.0}};

// efficacité d'une attaque de type 'attaque' contre les types en 'DEFurs'
double getEfficacite(TypeEnum attaque, const std::vector<TypeEnum>& DEFurs) {
    if (attaque < 0 || attaque >= 18) return 1.0;
    double total = 1.0;
    for (TypeEnum def : DEFurs) {;
        total *= chart[static_cast<int>(def)][static_cast<int>(attaque)];
    }
    return total;
}

bool critique(int bonus = 0) {
    int prob = 24;
    switch (bonus){
        case 1:{prob=8;break;}
        case 2: {prob=2;break;}
        default: break;
    }
    if(bonus>=3) {prob=1;}
    return (std::rand() % prob) == 0;
}

bool precision(const Attaque& atk, Creature& attaquant, Creature& defenseur, Combat& combat) {
    bool aP1 = (&attaquant == combat.getActiveP1());

    double preMult = aP1 ? combat.getPREP1(PRE) : combat.getPREP2(PRE);

    double evaMult = aP1 ? combat.getPREP2(AVD) : combat.getPREP1(AVD);

    if (atk.getPrecision() == -1) return true;

    double chance = atk.getPrecision() * (preMult / evaMult);

    return ((std::rand() % 100) < chance);
}

int calculerDegatsPur(const Creature& attaquant,const Creature& defenseur,
                   Attaque& atk, Combat& combat)
{
    double eff = getEfficacite(atk.getType(), defenseur.getTypes());
    if(eff==0) return 0;
    if (atk.getCategorie() == 0 || atk.getPuissance() <= 0)
        return 0;

    // Détection : attaquant = joueur 1 ?
    bool aP1 = (&attaquant == combat.getActiveP1());

    // Multiplicateurs de stats
    double atkMod = 1.0;
    double defMod = 1.0;
    int atkstat;
    int defstat;
    if (atk.getCategorie() == 1) { // Physique
        if(aP1) {
            atkMod=combat.getStatMultiplierP1(ATK); 
            defMod=combat.getStatMultiplierP2(DEF);
            }
        else{
            atkMod=combat.getStatMultiplierP2(ATK); 
            defMod=combat.getStatMultiplierP1(DEF);
        }
        atkstat=attaquant.calculStat(ATK);
        defstat=defenseur.calculStat(DEF);
    }
    else { // Special
        if(aP1) {
            combat.getStatMultiplierP1(ATKSP); 
            combat.getStatMultiplierP2(DEFSP);
            }
        else{
            combat.getStatMultiplierP2(ATKSP); 
            combat.getStatMultiplierP1(DEFSP);
        }
        atkstat=attaquant.calculStat(ATKSP);
        defstat=defenseur.calculStat(DEFSP);
    }
    atkstat *= atkMod;
    defstat *= defMod;

    if (defstat < 1) defstat = 1;

    int Mod1=1;
    int Mod2;
    int Mod3;

    double stab = 1.0;
    for (auto t : attaquant.getTypes())
        if (t == atk.getType()) {stab = 1.5; break;}

    if(combat.meteoAct==Meteo::Soleil){
        if(atk.type == Type::FEU || atk.getNom()="Hydrovapeur") Mod1*=1.5;
        else if(atk.type == Type::EAU) Mod1*=0.5;
    }
    if(combat.meteoAct==Meteo::Pluie){
        if(atk.type==Type::EAU) Mod1*=1.5;
        else if(atk.type==Type::FEU) Mod1*=0.5;
    }

    int base1 =std::floor(attaquant.getLVL() * 0.4)+2;
    int base2 = base1*atk.getPuissance()*atkstat;
    int base3 = std::floor(base2/ defstat);
    int base4 = std::floor(base3/50)
    int base5 = std::floor(base4*Mod1+2);
    int base = base5*Mod2*Mod3;

    int degats = std::floor(base * stab * eff);

    return degats;
}


int calculerDegats(int degatsPur,const Creature& defenseur,
                   Attaque& atk, Combat& combat) {
    if(atk.getCategorie()==0) {
        atk.utiliserPP();
        return 0;}
    
    int random = std::rand() % 16 + 85;
    int crit = critique() ? 1.5 : 1;
    if (crit==1.5) std::cout<<"Coup critique !\n"<<std::endl;
    if (getEfficacite(atk.getType(), defenseur.getTypes())==0) {
        std::cout<<defenseur.getNom()<<" est immunisé contre l'attaque !\n"<<std::endl;
        atk.utiliserPP();
        return 0;
    }
    else if(getEfficacite(atk.getType(), defenseur.getTypes())<1) {
        std::cout<<"Ce n'est pas très efficace...\n"<<std::endl;
    }
    else if(getEfficacite(atk.getType(), defenseur.getTypes())>1) {
        std::cout<<"C'est super efficace !\n" <<std::endl;
    }
    atk.utiliserPP();
    if(atk.getCategorie()==0) return 0;
    return std::max(1, degatsPur * random * crit/100);
}

int priorite(Combat& combat) {
    Joueur& j1=combat.getP1();
    Joueur& j2=combat.getP2();

    Creature& c1=j1.getEquipe()[j1.getIndex()];
    Creature& c2=j2.getEquipe()[j2.getIndex()];

    int v1=c1.calculStat(VIT);
    int v2=c2.calculStat(VIT);
    
    if(v1==v2) {
        int r=rand()%2;
        return (r==0) ? 1:2;
    } else if(v1>v2) return 1;
    else return 2;
}

Attaque& attaqueAleatoire(Creature& attaquant, const Creature& DEFur, Combat& combat) {
    auto& slots = attaquant.getSlots();

    std::vector<Attaque*> valids;
    for (auto i:slots) if (i) valids.push_back(i);

    if (valids.empty()) {
        int size=attaquant.getCreature()->getAttaques().size()-1;
        attaquant.getCreature()->ajouterAttaque(lutte);

        attaquant.assignerSlot(0, size);
        attaquant.assignerSlot(1, size);
        attaquant.assignerSlot(2, size);
        attaquant.assignerSlot(3, size);
        return lutte;}

    int totalWeight = 0.0;
    std::vector<int> weights;
    for (const Attaque* a : valids) {
        double poids=calculerDegatsPur(attaquant, DEFur, *a, combat);
        weights.push_back(poids);
        totalWeight += poids;
    }
    
    /*
    std::cout<<"totalWeight: "<<totalWeight<<std::endl;
    std::cout<<"weights: ";
    for(auto w:weights) std::cout<<w<<" ";
    */
   
    int r = rand() % totalWeight;
    for (size_t i = 0; i < valids.size(); ++i) {
        r -= weights[i];
        if (r < 0) {
            return *(valids[i]);
        }
    }
    return *(valids[rand()%valids.size()]);
}

void afficherAttaques(std::vector<Attaque*> slots) {
    std::vector<const Attaque*> valids;
    for (auto i:slots) if (i) valids.push_back(i);
    int cpt=0;
    for (auto i: valids) {
        cpt++;
        std::cout << cpt << ". ";
        if (i)
            std::cout << i->getNom();
        else
            std::cout << "[vide]";
        std::cout << "\n";
        std::cout<<"PP: "<<i->getPP_act()<<"/"<<i->getPP()<<std::endl;
    }
    std::cout << "> ";
}

Attaque& choixAttaque(Creature& c) {
    std::cout << "=== Attaques de " << c.getNom() << " ===\n";
    auto& slots = c.getSlots();
    afficherAttaques(slots);
    int n;
    while (true) {
        if (!(std::cin >> n)) { std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (n < 1 || n > 4) {
            std::cout << "Choix invalide\n";
            continue;
        }

        if (!slots[n-1]) {
            std::cout << "Ce slot est vide.\n";
            continue;
        }

        std::cout << "Vous avez choisi : " 
                  << slots[n-1]->getNom() << "\n";
        return *slots[n-1];
    }
}

void effectuerAttaque(Creature& attaquant, Creature& defenseur, 
                      Attaque& atk, Combat& combat)
{
    std::cout << attaquant.getNom() << " utilise " << atk.getNom() << std::endl;

    int degats = calculerDegats(calculerDegatsPur(attaquant, defenseur, atk, combat), defenseur, atk, combat);
    defenseur.setPV(defenseur.getPV() - degats);

    if(degats>0)
        std::cout << defenseur.getNom() << " perd " << degats << " PV\n";
    appliquerEffets(atk, attaquant, defenseur, combat);
    if(atk.getContrecoup()){
        int recul = std::max(1, degats/3);
        attaquant.setPV(attaquant.getPV() - recul);
    }
    if (defenseur.estKO()) {
        std::cout << defenseur.getNom() << " est KO!\n";
    }
}

void resoudreTour(Combat& combat)
{
    Attaque& atkP1 = choixAttaque(*combat.getActiveP1());
    Attaque& atkP2 = attaqueAleatoire(*combat.getActiveP2(), *combat.getActiveP1(), combat);
    
    int prio = priorite(combat);

    if (prio == 1) {
        effectuerAttaque(*combat.getActiveP1(), *combat.getActiveP2(), atkP1, combat);

        if (!combat.getActiveP2()->estKO()) {
            effectuerAttaque(*combat.getActiveP2(), *combat.getActiveP1(), atkP2, combat);
        }
    } 
    else {
        effectuerAttaque(*combat.getActiveP2(), *combat.getActiveP1(), atkP2, combat);

        if (!combat.getActiveP1()->estKO()) {
            effectuerAttaque(*combat.getActiveP1(), *combat.getActiveP2(), atkP1, combat);
        }
    }
}

void afficherMenuCombat()
{
    std::cout << "=== Combat ===\n"
              << "1. Combat\n"
              << "2. Equipe\n"
              << "3. Sac\n"
              << "4. Fuite\n"
              << "> ";
}

void finCombat(Combat& combat){
    if(combat.getP1().toutesCreaturesKO()){
        std::cout<<combat.getP2().getNom()<<" a gagné le combat !"<<std::endl;
        exit(-1);
    } else if(combat.getP2().toutesCreaturesKO()){
        std::cout<<combat.getP1().getNom()<<" a gagné le combat !"<<std::endl;
        exit(-1);
    }
}

void switchCombat(Combat& combat){
    Joueur& j1=combat.getP1();

    std::cout<<"Choisissez une créature pour "<<j1.getNom()<<":"<<std::endl;

    j1.afficherEquipe();
    int n;
    while (true) {
        if (!(std::cin >> n)) { std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (n < 1 || n > (int)j1.getEquipe().size()) {
            std::cout << "Choix invalide\n";
            continue;
        }

        if (j1.getEquipe()[n-1].estKO()) {
            std::cout << "Cette créature est KO.\n";
            continue;
        }

        combat.setActiveP1(n-1);
        std::cout << j1.getNom() << " a choisi : " 
                  << combat.getActiveP1()->getNom() << "\n";
        break;
    }
}

int switchBot(Combat& combat){
    Joueur& bot = combat.getP2();

    Creature* ennemi = combat.getActiveP1();
    if (!ennemi) return -1;

    int bestIndex = -1;
    int bestScore = -1;

    for (int i = 0; i < (int)bot.getEquipe().size(); ++i)
    {
        Creature& candidate = bot.getEquipe()[i];
        if (candidate.estKO()) continue;
        int bestDamageThisPokemon = 0;
        for (Attaque* a : candidate.getSlots())
        {
            if (!a) continue;
            if (a->getPP_act() <= 0) continue;
            int dmg = calculerDegatsPur(candidate, *ennemi, *a, combat);
            if (dmg > bestDamageThisPokemon)
                bestDamageThisPokemon = dmg;
        }

        if (bestDamageThisPokemon > bestScore)
        {
            bestScore = bestDamageThisPokemon;
            bestIndex = i;
        }
    }

    return bestIndex;
}

int initSwitchBot(Combat& combat){
    Joueur& bot = combat.getP2();
    Joueur& player = combat.getP1();

    std::vector<Creature>& equipe = bot.getEquipe();
    std::vector<Creature>& equipePlayer = player.getEquipe();

    int bestIndex = -1;
    double bestScore = -1;

    // Poids pour la fonction de score (à ajuster si besoin)
    const double W_INCOMING = 1.0;   // plus haut = on privilégie la minimisation des dégâts reçus
    const double W_OUTGOING = 1.2;   // plus haut = on valorise l'offense
    const double W_SPEED = 2.0;      // bonus par adversaire plus lent
    const double W_HP = 0.02;        // petit bonus pour PV max

    for (int i = 0; i < (int)equipe.size(); ++i) {
        Creature& cand = equipe[i];
        if (cand.estKO()) continue;

        // Nombre d'adversaires encore valides
        int opponents = 0;
        double sumBestIncoming = 0.0; // dégâts attendus que l'adversaire peut infliger au meilleur de ses attacks
        double sumBestOutgoing = 0.0; // dégâts que le candidat peut infliger (meilleure attaque par adversaire)
        int speedAdvantageCount = 0;
        int candHP = cand.calculStat(PV);

        // Vérifier si le candidat a au moins une attaque utilisable
        int usableMoves = 0;
        for (Attaque* sa : cand.getSlots()) if (sa && sa->getPP_act() > 0) usableMoves++;
        if (usableMoves == 0) {
            // pénalité très forte si pas de moves utilisables
            continue;
        }

        for (Creature& opp : equipePlayer) {
            if (opp.estKO()) continue;
            opponents++;

            // Meilleure attaque de l'adversaire contre cand
            int bestIncoming = 0;
            for (Attaque* a : opp.getSlots()) {
                if (!a || a->getPP_act() <= 0) continue;
                int d = calculerDegatsPur(opp, cand, *a, combat);
                bestIncoming = std::max(bestIncoming, d);
            }
            if (bestIncoming <= 0) bestIncoming = 1; // éviter div / 0 et trop faibles valeurs
            sumBestIncoming += bestIncoming;

            // Meilleure attaque du candidat contre cet adversaire
            int bestOutgoing = 0;
            for (Attaque* a : cand.getSlots()) {
                if (!a || a->getPP_act() <= 0) continue;
                int d = calculerDegatsPur(cand, opp, *a, combat);
                bestOutgoing = std::max(bestOutgoing, d);
            }
            sumBestOutgoing += bestOutgoing;

            // Avantage vitesse
            if (cand.calculStat(VIT) > opp.calculStat(VIT)) speedAdvantageCount++;
        }

        if (opponents == 0) return i; // pas d'adversaires: choix trivial

        double avgIncoming = sumBestIncoming / opponents / candHP;
        double avgOutgoing = sumBestOutgoing / opponents;

        // Score : plus petit = meilleur choix
        double score = W_OUTGOING * avgOutgoing - W_INCOMING * avgIncoming + W_SPEED * speedAdvantageCount + W_HP * candHP;

        // Petites protections numériques
        if (std::isnan(score) || std::isinf(score)) continue;

        // Debug (décommenter si besoin)
        std::cout <<cand.getNom() << " : in="<<avgIncoming<<" out="<<avgOutgoing<<" spAdv="<<speedAdvantageCount<<" hp="<<candHP<<" -> score="<<score<<"\n";
        std::cout <<"score: "<<score<<" bestScore: "<<bestScore<<std::endl;
        if (score > bestScore) {
            bestScore = score;
            bestIndex = i;
        }
    }

    if (bestIndex < 0) {
        // fallback minimal : choisir le premier non-KO
        for (int i = 0; i < (int)equipe.size(); ++i) {
            if (!equipe[i].estKO()) { bestIndex = i; break; }
        }
    }

    if (bestIndex >= 0)
        std::cout << "Bot choisit " << equipe[bestIndex].getNom() << std::endl;
    return bestIndex;
}

void menuCombat(Combat& combat){
    combat.setActiveP2(initSwitchBot(combat));
    int n;
    bool menu=true;
    static int tour=0 ;
    while(menu){
        finCombat(combat);
        tour++;

        
        if(combat.getActiveP1()->estKO()) switchCombat(combat);
        if(combat.getActiveP2()->estKO()) {
            int index = switchBot(combat);
            combat.setActiveP2(index);
            std::cout<<combat.getP2()<<" a choisi "<<*combat.getActiveP2()<<std::endl;
            
        }
        std::cout<<"\n=========================="<<std::endl;
        std::cout<<"\n=== Tour "<<tour<<" ===\n"<<std::endl;
        std::cout<<*combat.getActiveP1()<<"//"<<*combat.getActiveP2()<<std::endl;
        afficherMenuCombat();

        if (!(std::cin>>n)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        switch (n) {
        case 1:
            resoudreTour(combat);
            break;

        case 2:
            switchCombat(combat);
            break;

        case 3:
            std::cout<<"T'es dans le sac\n";
            break;

        case 4:
            std::cout<<"Vous prenez la fuite comme une merde!";
            return;

        default:
        std::cout<<"T'es une merde"<<std::endl;
            break;
        }
    }
}


int main() {
    std::srand(time(NULL));

    Joueur DocPoulet("DocPoulet");
    Joueur Frodon("Frodon");
    Combat combat(DocPoulet, Frodon);

    Creature salameche1(&salameche, 5, "Hardi");
    Creature carapuce1(&carapuce, 5, "Modeste");
    Creature salameche2(&salameche, 5, "Timide");
    Creature carapuce2(&carapuce, 5, "Assuré");

//Attaque("Nom", TYPE, puissance,  status0/physique1/special2, precision%, critique bonus, pp, pp max, effets, contrecoup)
// effets = {Stat(0=base stat, 1=Precision), stat, variation(+/-), cible(0=attaquant/1=defenseur)}

    salameche.ajouterAttaque(flammeche);
    salameche.ajouterAttaque(griffe);
    salameche.ajouterAttaque(pistolet_a_O);
    salameche.ajouterAttaque(fouet_lianes);
    salameche.ajouterAttaque(close_combat);
    salameche.ajouterAttaque(aiguisage);
    salameche.ajouterAttaque(psyko);
    salameche.ajouterAttaque(acupression);

    carapuce.ajouterAttaque(pistolet_a_O);
    carapuce.ajouterAttaque(fouet_lianes);
    carapuce.ajouterAttaque(acupression);
    

    salameche1.assignerSlot(7,0);
    salameche1.assignerSlot(6,1);
    salameche1.assignerSlot(5,2);
    salameche1.assignerSlot(3,3);
    salameche1.assignerSlot(4,3);

    salameche2.assignerSlot(0,0);
    salameche2.assignerSlot(1,1);
    salameche2.assignerSlot(3,3);

    //carapuce1.assignerSlot(0,0);
    carapuce1.assignerSlot(1,1);

    carapuce2.assignerSlot(1,0);
    carapuce2.assignerSlot(0,1);

    DocPoulet.ajouterCreature(salameche1);
    Frodon.ajouterCreature(salameche2);
    Frodon.ajouterCreature(carapuce1);
    DocPoulet.ajouterCreature(carapuce2);

 
    
    DocPoulet.afficherEquipe();
    combat.setActiveP1(0);  
    combat.setActiveP2(0); 
/*
    

    std::cout<<"\n=== Meilleure Attaque de "<<combat.getActiveP1()->getNom()<<" contre "<<combat.getActiveP2()->getNom()<<" ===\n"<<std::endl;
    const Attaque& meilleure = attaqueAleatoire(*combat.getActiveP1(), *combat.getActiveP2(), combat);
    std::cout<<"\n"<<meilleure<<std::endl;
*/


    menuCombat(combat);

    return 0;
}
