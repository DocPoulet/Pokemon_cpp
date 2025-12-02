#pragma once
#include "Terrain.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <cmath>

// ==== Définitions complètes des énums ====
enum StatIndex {
    PV = 0,
    ATK,
    DEF,
    ATKSP,
    DEFSP,
    VIT,
    STAT_COUNT
};

enum StatPrecision {
    PRE,
    AVD,
    STAT_COUNT_PRE
};

enum TypeEnum {
    ACIER, COMBAT, DRAGON, EAU, ELECTRIQUE, FEE,
    FEU, GLACE, INSECTE, NORMAL, PLANTE, POISON,
    PSY, ROCHE, SOL, SPECTRE, TENEBRES, VOL,
    NEUTRE
};

// constantes globales utilisées par les classes
extern const int MAX_EQUIPE;
extern const int MIN_MOD;
extern const int MAX_MOD;

// prototypes de fonctions et externs définis dans le .cpp principal
std::string toString(StatIndex s);
double getEfficacite(TypeEnum attaque, const std::vector<TypeEnum>& DEFurs);

// forward pour variables définies dans le .cpp (déclaration externe)
extern const std::map<std::string, class Nature> NATURES;
extern const std::vector<std::string> nomsTypes;

// ==== Structure décrivant un effet de stat utilisé par Attaque ====
struct EffetStat {
    int estPrecision; // 0 = StatIndex, 1 = StatPrecision
    int stat;        // StatIndex ou StatPrecision
    int stages;     // variation en nombre de crans (+/-)
    bool cible;       //  0 = lanceur, 1 = cible
    int proba=100;      // probabilité en pourcentage d'application de l'effet
};

// ==== Classes complètes (implémentations inline pour éviter définitions multiples) ====

class Objet {
    std::string nom;

public:
    Objet(std::string nom):nom(nom){}
    std::string getNom(){return nom;}
};


// Nature
class Nature {
    std::string name;
    StatIndex increased;
    StatIndex decreased;

public:
    Nature(const std::string& n = "", StatIndex inc = ATK, StatIndex dec = ATK)
        : name(n), increased(inc), decreased(dec) {}

    const std::string& getName() const { return name; }
    StatIndex getIncreased() const { return increased; }
    StatIndex getDecreased() const { return decreased; }

    double getBoost(StatIndex s) const {
        if (increased == s && decreased == s)
            return 1.0;
        if (increased == s) return 1.1;
        if (decreased == s) return 0.9;
        return 1.0;
    }
};

class Attaque {
    std::string nom;
    int categorie; // 1=physique, 2=spécial, 0=status
    TypeEnum type;
    int puissance;
    int precision;
    int critique;
    int pp, pp_act, pp_max;
    std::vector<EffetStat> effets;
    bool contrecoup;

public:
    Attaque(std::string n = "", TypeEnum t = NEUTRE, int p = 0,  int c = 0, int pr = 100,
            int cr=0, int pp=5, int ppm = 8, std::vector<EffetStat> eff = {}, bool recul=0)
        : nom(std::move(n)), categorie(c), type(t), puissance(p), precision(pr),
          critique(cr), effets(eff), contrecoup(recul) {
        if (categorie < 0 || categorie > 2) categorie = 0;
        pp_max = ppm;
        if (pp < 0) pp = 0;
        if (pp > pp_max) pp = pp_max;
        this->pp = pp;
        pp_act = pp;
    }

    int getPP() const { return pp; }
    int getPP_act() const { return pp_act; }
    int getPP_max() const { return pp_max; }
    std::string getNom() const { return nom; }
    TypeEnum getType() const { return type; }
    int getPuissance() const { return puissance; }
    int getPrecision() const { return precision; }
    int getCategorie() const { return categorie; }
    int getCrit() const { return critique; }
    const std::vector<EffetStat>& getEffets() const { return effets; }
    bool getContrecoup() const { return contrecoup; }

    void setPP(int v) {
        pp = v;
        if (pp < 0) pp = 0;
        if (pp > pp_max) pp = pp_max;
    }
    void setPP_act(int v) {
        pp_act = v;
        if (pp_act < 0) pp_act = 0;
    }
    void setPP_max(int v) { pp_max = v; }
    void setNom(std::string n) { nom = std::move(n); }
    void setType(TypeEnum n) { type = n; }
    void setPuissance(int p) { puissance = p; }
    void setPrecision(int p) { precision = p; }
    void setCategorie(int c) { categorie = c; }
    void setCrit(int c) { critique = c; }
    void setEffets(std::vector<EffetStat> e) { effets = std::move(e); }

    void utiliserPP() { if (pp_act > 0) pp_act--; }
};

inline std::ostream& operator<<(std::ostream& os, const Attaque& a) {
    std::string categorie;
    if (a.getCategorie() == 2) categorie = "Spécial";
    else if (a.getCategorie() == 1) categorie = "Physique";
    else categorie = "Statut";
    os << a.getNom()
       << " (Type: " << nomsTypes[a.getType()]
       << ", Puissance: " << a.getPuissance()
       << ", Catégorie: " << categorie
       << ", Précision: " << a.getPrecision()
       << ", Critique: +" << a.getCrit()
       << ")";
    return os;
}

extern Attaque lutte;

// CreatureBase
class CreatureBase {
    static int compteurID;
    int idEspece;

protected:
    std::string nom;
    std::array<int, STAT_COUNT> baseStat;
    std::vector<TypeEnum> types;
    std::vector<Attaque> attaques;

public:
    CreatureBase(std::string n = "MissingNO", int hp = 33, int a = 136, int d = 0,
                 int as = 6, int ds = 6, int v = 28,
                 std::vector<TypeEnum> t = { NORMAL })
        : nom(std::move(n)), types(std::move(t)) {
        baseStat[PV] = hp;
        baseStat[ATK] = a;
        baseStat[DEF] = d;
        baseStat[ATKSP] = as;
        baseStat[DEFSP] = ds;
        baseStat[VIT] = v;
        idEspece = ++compteurID;
    }

    int getBase(StatIndex s) const { return baseStat[s]; }
    const std::string& getNom() const { return nom; }
    const std::vector<TypeEnum>& getTypes() const { return types; }
    const std::vector<Attaque>& getAttaques() const { return attaques; }
    std::vector<Attaque>& getAttaques() { return attaques; }
    int getID(){return compteurID;}


    void ajouterAttaque(const Attaque& a) { attaques.push_back(a); }
};

inline int CreatureBase::compteurID = 0;

// Creature
class Creature {
    CreatureBase* base;
    int LVL;
    int PV_act;

    std::array<int, STAT_COUNT> IV;
    std::array<int, STAT_COUNT> EV;

    std::vector<Attaque*> slots;   // attaques équipées

    Nature nature;

    Objet* objet;

    void forceEV(StatIndex stat, int ev) { EV[stat] = ev; }

public:
    Creature(CreatureBase* b = nullptr, int lvl = 1, std::string nat = "Hardi", Objet* o=nullptr)
        : base(b), LVL(lvl), nature(NATURES.at(nat)), slots(4, nullptr), objet(o) {
        IV.fill(0);
        EV.fill(0);
        PV_act = calculStat(PV);
    }

    // getters de la base
    CreatureBase* getCreature() const { return base; }
    int getBase(StatIndex s) const { return base->getBase(s); }
    const std::string& getNom() const { return base->getNom(); }
    const std::vector<TypeEnum>& getTypes() const { return base->getTypes(); }
    const std::vector<Attaque>& getAttaques() const { return base->getAttaques(); }
    std::vector<Attaque>& getAttaques() { return base->getAttaques(); }
    int getID(){return base->getID();}

    // getters joueur
    int getPV() const { return PV_act; }
    int getLVL() const { return LVL; }
    const std::vector<Attaque*>& getSlots() const { return slots; }

    int getEV(StatIndex stat) const { return EV[stat]; }
    int getIV(StatIndex stat) const { return IV[stat]; }

    const Nature& getNature() const { return nature; }
    void setNature(const Nature& nat) { nature = nat; }
    Objet* getObjet(){return objet;}

    int totalEV() const {
        int sum = 0;
        for (int v : EV) sum += v;
        return sum;
    }

    void setIV(StatIndex stat, int iv) {
        if (iv >= 31) IV[stat] = 31;
        else IV[stat] = iv;
    }

    bool setEV(StatIndex stat, int newValue) {
        if (newValue < 0) newValue = 0;
        if (newValue > 252) newValue = 252;
        int current = EV[stat];
        if (newValue <= current) { EV[stat] = newValue; return true; }
        int amount = newValue - current;
        int roomStat  = 252 - current;
        int roomTotal = 510 - totalEV();
        if (roomStat <= 0 || roomTotal <= 0) return false;
        int addable = std::min({ amount, roomStat, roomTotal });
        EV[stat] += addable;
        return true;
    }

    void resetEV() { EV.fill(0); }
    void IV6() { IV.fill(31); }
    void IV5() { IV.fill(31); IV[ATK] = 0; }
    void IV0() { IV.fill(0); }

    int calculStat(StatIndex stat) const {
        if (stat == PV) {
            if (getNom() == "Munja") return 1;
            return (((2 * getBase(PV) + IV[PV] + EV[PV] / 4) * LVL) / 100) + LVL + 10;
        }
        double mult = nature.getBoost(stat);
        return static_cast<int>(((((2 * getBase(stat) + IV[stat] + EV[stat] / 4) * LVL) / 100) + 5) * mult);
    }

    void afficherStats() const {
        std::cout << "PV: " << calculStat(PV) << "\n";
        std::cout << "Attaque: " << calculStat(ATK) << "\n";
        std::cout << "Défense: " << calculStat(DEF) << "\n";
        std::cout << "Attaque Spéciale: " << calculStat(ATKSP) << "\n";
        std::cout << "Défense Spéciale: " << calculStat(DEFSP) << "\n";
        std::cout << "Vitesse: " << calculStat(VIT) << "\n";
    }

    bool assignerSlot(Attaque& atk, int slot) {
    if (!base) return false;
    // Vérifie si le slot est valide (entre 0 et 3)
    if (slot < 0 || slot >= 4) return false;

    // Assigner le pointeur vers l'objet Attaque directement au slot
    // L'adresse de l'objet est prise avec l'opérateur &
    slots[slot] = &atk;
    return true;
}

    void effacerSlot(int slot) {
        if (slot >= 0 && slot < 4) slots[slot] = nullptr;
    }

    bool estKO() const { return PV_act <= 0; }

    void setPV(int newPV) {
        PV_act = std::max(0, std::min(calculStat(PV), newPV));
    }

    void afficherEtat() {
        std::cout << getNom() << " - " << PV_act << "/" << calculStat(PV) << " PV (VIT " << calculStat(VIT) << ")\n Vitesse théorique: ( ";
        int evtmp = getEV(VIT);
        forceEV(VIT, 0);
        std::cout << calculStat(VIT) << " - ";
        forceEV(VIT, 252);
        std::cout << calculStat(VIT) << " )\n";
        forceEV(VIT, evtmp);
    }
};

inline std::ostream& operator<<(std::ostream& os, const Creature& c) {
    os << c.getNom()
       << " (PV : " << c.getPV()
       << "/" << c.calculStat(PV)
       << ", Niveau : " << c.getLVL()
       << ")";
    return os;
}

// Joueur
class Joueur {
    std::string nom;
    std::vector<Creature> equipe;
    int indexActif = 0;

public:
    Joueur(std::string n = "") : nom(std::move(n)) {}

    void ajouterCreature(const Creature& c) {
        if (equipe.size() >= static_cast<size_t>(MAX_EQUIPE)) {
            std::cout << nom << " ne peut pas avoir plus de 6 créatures !\n";
            return;
        }
        equipe.push_back(c);
    }

    std::string getNom() const { return nom; }
    std::vector<Creature>& getEquipe() { return equipe; }
    int getIndex() const { return indexActif; }

    void afficherEquipe()  {
        std::cout << "\nÉquipe de " << nom << " :\n";
        for (auto& c : equipe) c.afficherEtat();
    }

    bool toutesCreaturesKO() const {
        for (const auto& c : equipe)
            if (c.getPV() > 0) return false;
        return true;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Joueur& j) {
    os << j.getNom();
    return os;
}

// Stages
struct Stages {
    std::array<int, STAT_COUNT> stats{};
    std::array<int, STAT_COUNT_PRE> precisionEva{};
    Stages() { stats.fill(0); precisionEva.fill(0); }
};

// Combat
class Combat {
    std::array<Joueur*, 2> joueurs;      // [0] = P1, [1] = P2
    std::array<int, 2> activeIndex;      // Index des créatures actives
    std::array<Stages, 2> mods;          // Modificateurs de stats

public:
    Meteo meteoAct;
    int dureeMeteo;

    Champ champAct;
    int dureeChamp;

    Combat(Joueur& j1, Joueur& j2)
        : joueurs{&j1, &j2}, activeIndex{0, 0},
        meteoAct(Meteo::Aucune), dureeMeteo(0), 
        champAct(Champ::Aucun), dureeChamp(0) {}

    Joueur& getJoueur(int index) const { 
        return *joueurs[index]; 
    }

    Joueur& getP1() const { return *joueurs[0]; }
    Joueur& getP2() const { return *joueurs[1]; }

    void setActive(int joueurIndex, int creatureIndex) {
        if (creatureIndex < 0 || creatureIndex >= static_cast<int>(joueurs[joueurIndex]->getEquipe().size())) 
            return;
        if (joueurs[joueurIndex]->getEquipe()[creatureIndex].estKO()) 
            return;
        activeIndex[joueurIndex] = creatureIndex;
    }

    void setActiveP1(int i) { setActive(0, i); }
    void setActiveP2(int i) { setActive(1, i); }

    Creature* getActive(int joueurIndex) {
        int idx = activeIndex[joueurIndex];
        auto& equipe = joueurs[joueurIndex]->getEquipe();
        if (idx < 0 || idx >= static_cast<int>(equipe.size())) 
            return nullptr;
        return &equipe[idx];
    }

    Creature* getActiveP1() { return getActive(0); }
    Creature* getActiveP2() { return getActive(1); }

    int getJoueurIndex(const Creature* crea) {
        if (getActive(0) == crea) return 0;
        if (getActive(1) == crea) return 1;
        return -1;
    }
    // ---- RESET ----
    void resetStages(int joueurIndex) { 
        mods[joueurIndex] = Stages(); 
    }

    void resetStagesP1() { resetStages(0); }
    void resetStagesP2() { resetStages(1); }

    // ---- MODIFICATION ----
    void changeStage(int joueurIndex, StatIndex stat, int var) {
        mods[joueurIndex].stats[stat] = std::clamp(
            mods[joueurIndex].stats[stat] + var, MIN_MOD, MAX_MOD);
    }
    
    void changeStageP1(StatIndex stat, int var) { changeStage(0, stat, var); }
    void changeStageP2(StatIndex stat, int var) { changeStage(1, stat, var); }

    void changePrecision(int joueurIndex, StatPrecision stat, int var) {
        mods[joueurIndex].precisionEva[stat] = std::clamp(
            mods[joueurIndex].precisionEva[stat] + var, MIN_MOD, MAX_MOD);
    }
    
    void changePrecisionP1(StatPrecision stat, int var) { changePrecision(0, stat, var); }
    void changePrecisionP2(StatPrecision stat, int var) { changePrecision(1, stat, var); }

    // ---- GETTERS ----
    int getStage(int joueurIndex, StatIndex stat) const { 
        return mods[joueurIndex].stats[stat]; 
    }
    
    int getStageP1(StatIndex stat) const { return getStage(0, stat); }
    int getStageP2(StatIndex stat) const { return getStage(1, stat); }
    
    int getPrecisionStage(int joueurIndex, StatPrecision stat) const { 
        return mods[joueurIndex].precisionEva[stat]; 
    }
    
    int getPrecisionStageP1(StatPrecision stat) const { return getPrecisionStage(0, stat); }
    int getPrecisionStageP2(StatPrecision stat) const { return getPrecisionStage(1, stat); }

    // ---- CONVERSION EN MULTIPLICATEURS ----
    static double stageMultiplier(int stage) {
        if (stage > 0) return 1.0 + 0.5 * stage;
        if (stage < 0) return 1.0 / (1.0 + 0.5 * (-stage));
        return 1.0;
    }

    static double stageMultiplierPrecision(int stage) {
        if (stage > 0) return 1.0 + (stage / 3.0);
        if (stage < 0) return 1.0 / (1.0 + (-stage / 3.0));
        return 1.0;
    }

    double getStatMultiplier(int joueurIndex, StatIndex s) const { 
        return stageMultiplier(mods[joueurIndex].stats[s]); 
    }
    
    double getStatMultiplierP1(StatIndex s) const { return getStatMultiplier(0, s); }
    double getStatMultiplierP2(StatIndex s) const { return getStatMultiplier(1, s); }

    double getPRE(int joueurIndex, StatPrecision stat) const { 
        return stageMultiplierPrecision(mods[joueurIndex].precisionEva[stat]); 
    }
    
    double getPREP1(StatPrecision stat) const { return getPRE(0, stat); }
    double getPREP2(StatPrecision stat) const { return getPRE(1, stat); }
};

// Fonction gérant les effets (définie dans le .cpp)
void appliquerEffets(const Attaque& atk, Creature& attaquant, Creature& defenseur, Combat& combat);