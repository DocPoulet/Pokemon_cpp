#pragma once

#include "Terrain.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

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

extern const int MAX_EQUIPE;
extern const int MIN_MOD;
extern const int MAX_MOD;

std::string toString(StatIndex s);

class Nature;
extern const std::map<std::string, Nature> NATURES;
extern const std::vector<std::string> nomsTypes;

struct EffetStat {
    int estPrecision; // 0 = StatIndex, 1 = StatPrecision
    int stat;
    int stages;
    bool cible;       // false = lanceur, true = cible
    int proba = 100;
};

class Objet {
    std::string nom;

public:
    explicit Objet(std::string nom_) : nom(std::move(nom_)) {}
    const std::string& getNom() const { return nom; }
};

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
        if (increased == s && decreased == s) return 1.0;
        if (increased == s) return 1.1;
        if (decreased == s) return 0.9;
        return 1.0;
    }
};

class Attaque {
    std::string nom;
    int categorie; // 0 = statut, 1 = physique, 2 = special
    TypeEnum type;
    int puissance;
    int precision;
    int critique;
    int pp;
    int pp_act;
    int pp_max;
    std::vector<EffetStat> effets;
    bool contrecoup;

public:
    Attaque(std::string n = "", TypeEnum t = NEUTRE, int p = 0, int c = 0, int pr = 100,
            int cr = 0, int pp_ = 5, int ppm = 8, std::vector<EffetStat> eff = {},
            bool recul = false)
        : nom(std::move(n)),
          categorie(std::clamp(c, 0, 2)),
          type(t),
          puissance(std::max(0, p)),
          precision(pr),
          critique(std::max(0, cr)),
          pp(std::max(0, pp_)),
          pp_act(std::max(0, pp_)),
          pp_max(std::max(0, ppm)),
          effets(std::move(eff)),
          contrecoup(recul) {
        pp = std::min(pp, pp_max);
        pp_act = pp;
    }

    int getPP() const { return pp; }
    int getPP_act() const { return pp_act; }
    int getPP_max() const { return pp_max; }
    const std::string& getNom() const { return nom; }
    TypeEnum getType() const { return type; }
    int getPuissance() const { return puissance; }
    int getPrecision() const { return precision; }
    int getCategorie() const { return categorie; }
    int getCrit() const { return critique; }
    const std::vector<EffetStat>& getEffets() const { return effets; }
    bool getContrecoup() const { return contrecoup; }

    void setPP(int v) {
        pp = std::clamp(v, 0, pp_max);
        pp_act = std::min(pp_act, pp);
    }

    void setPP_act(int v) { pp_act = std::clamp(v, 0, pp_max); }

    void setPP_max(int v) {
        pp_max = std::max(0, v);
        pp = std::min(pp, pp_max);
        pp_act = std::min(pp_act, pp_max);
    }

    void setNom(std::string n) { nom = std::move(n); }
    void setType(TypeEnum n) { type = n; }
    void setPuissance(int p) { puissance = std::max(0, p); }
    void setPrecision(int p) { precision = p; }
    void setCategorie(int c) { categorie = std::clamp(c, 0, 2); }
    void setCrit(int c) { critique = std::max(0, c); }
    void setEffets(std::vector<EffetStat> e) { effets = std::move(e); }

    void utiliserPP() {
        if (pp_act > 0) --pp_act;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Attaque& a) {
    const char* categorie = "Statut";
    if (a.getCategorie() == 1) categorie = "Physique";
    if (a.getCategorie() == 2) categorie = "Special";

    const auto typeIndex = static_cast<std::size_t>(a.getType());
    const std::string typeNom =
        typeIndex < nomsTypes.size() ? nomsTypes[typeIndex] : "Inconnu";

    os << a.getNom()
       << " (Type: " << typeNom
       << ", Puissance: " << a.getPuissance()
       << ", Categorie: " << categorie
       << ", Precision: " << a.getPrecision()
       << ", Critique: +" << a.getCrit()
       << ")";
    return os;
}

extern Attaque lutte;

class CreatureBase {
    static int compteurID;
    int idEspece;

protected:
    std::string nom;
    std::array<int, STAT_COUNT> baseStat{};
    std::vector<TypeEnum> types;
    std::vector<Attaque> attaques;

public:
    CreatureBase(std::string n = "MissingNO", int hp = 33, int a = 136, int d = 0,
                 int as = 6, int ds = 6, int v = 28,
                 std::vector<TypeEnum> t = {NORMAL})
        : idEspece(++compteurID), nom(std::move(n)), types(std::move(t)) {
        baseStat[PV] = hp;
        baseStat[ATK] = a;
        baseStat[DEF] = d;
        baseStat[ATKSP] = as;
        baseStat[DEFSP] = ds;
        baseStat[VIT] = v;
    }

    int getBase(StatIndex s) const { return baseStat[static_cast<std::size_t>(s)]; }
    const std::string& getNom() const { return nom; }
    const std::vector<TypeEnum>& getTypes() const { return types; }
    const std::vector<Attaque>& getAttaques() const { return attaques; }
    std::vector<Attaque>& getAttaques() { return attaques; }
    int getID() const { return idEspece; }

    void ajouterAttaque(const Attaque& a) { attaques.push_back(a); }
};

inline int CreatureBase::compteurID = 0;

class Creature {
    CreatureBase* base;
    int LVL;
    int PV_act = 0;

    std::array<int, STAT_COUNT> IV{};
    std::array<int, STAT_COUNT> EV{};

    // v0.2: chaque Creature possede ses propres copies d'attaques.
    // Les PP ne sont donc plus partages entre deux Pokemon.
    std::array<std::optional<Attaque>, 4> slots{};

    Nature nature;
    Objet* objet;

    void forceEV(StatIndex stat, int ev) {
        EV[static_cast<std::size_t>(stat)] = ev;
    }

public:
    Creature(CreatureBase* b = nullptr, int lvl = 1, std::string nat = "Hardi",
             Objet* o = nullptr)
        : base(b),
          LVL(std::max(1, lvl)),
          nature(NATURES.at(nat)),
          objet(o) {
        IV.fill(0);
        EV.fill(0);
        if (base) PV_act = calculStat(PV);
    }

    CreatureBase* getCreature() const { return base; }

    int getBase(StatIndex s) const {
        return base ? base->getBase(s) : 0;
    }

    const std::string& getNom() const {
        static const std::string missing = "MissingNO";
        return base ? base->getNom() : missing;
    }

    const std::vector<TypeEnum>& getTypes() const {
        static const std::vector<TypeEnum> empty;
        return base ? base->getTypes() : empty;
    }

    const std::vector<Attaque>& getAttaques() const {
        static const std::vector<Attaque> empty;
        return base ? base->getAttaques() : empty;
    }

    std::vector<Attaque>& getAttaques() {
        static std::vector<Attaque> empty;
        return base ? base->getAttaques() : empty;
    }

    int getID() const { return base ? base->getID() : -1; }

    int getPV() const { return PV_act; }
    int getLVL() const { return LVL; }

    std::array<std::optional<Attaque>, 4>& getSlots() { return slots; }
    const std::array<std::optional<Attaque>, 4>& getSlots() const { return slots; }

    int getEV(StatIndex stat) const { return EV[static_cast<std::size_t>(stat)]; }
    int getIV(StatIndex stat) const { return IV[static_cast<std::size_t>(stat)]; }

    const Nature& getNature() const { return nature; }
    void setNature(const Nature& nat) { nature = nat; }

    Objet* getObjet() { return objet; }
    const Objet* getObjet() const { return objet; }

    int totalEV() const {
        int sum = 0;
        for (int v : EV) sum += v;
        return sum;
    }

    void setIV(StatIndex stat, int iv) {
        IV[static_cast<std::size_t>(stat)] = std::clamp(iv, 0, 31);
    }

    bool setEV(StatIndex stat, int newValue) {
        newValue = std::clamp(newValue, 0, 252);
        auto& current = EV[static_cast<std::size_t>(stat)];

        if (newValue <= current) {
            current = newValue;
            return true;
        }

        const int roomTotal = 510 - totalEV();
        if (roomTotal <= 0) return false;

        const int requested = newValue - current;
        const int addable = std::min(requested, roomTotal);
        current += addable;
        return current == newValue;
    }

    void resetEV() { EV.fill(0); }
    void IV6() { IV.fill(31); }
    void IV5() {
        IV.fill(31);
        IV[ATK] = 0;
    }
    void IV0() { IV.fill(0); }

    int calculStat(StatIndex stat) const {
        if (!base) return 0;

        if (stat == PV) {
            if (getNom() == "Munja") return 1;
            return (((2 * getBase(PV) + IV[PV] + EV[PV] / 4) * LVL) / 100) + LVL + 10;
        }

        const double mult = nature.getBoost(stat);
        return static_cast<int>(
            (((2 * getBase(stat) + IV[static_cast<std::size_t>(stat)] +
               EV[static_cast<std::size_t>(stat)] / 4) *
                  LVL) /
                 100 +
             5) *
            mult);
    }

    void afficherStats() const {
        std::cout << "PV: " << calculStat(PV) << '\n'
                  << "Attaque: " << calculStat(ATK) << '\n'
                  << "Defense: " << calculStat(DEF) << '\n'
                  << "Attaque Speciale: " << calculStat(ATKSP) << '\n'
                  << "Defense Speciale: " << calculStat(DEFSP) << '\n'
                  << "Vitesse: " << calculStat(VIT) << '\n';
    }

    bool assignerSlot(const Attaque& atk, int slot) {
        if (!base) return false;
        if (slot < 0 || slot >= static_cast<int>(slots.size())) return false;
        slots[static_cast<std::size_t>(slot)] = atk;
        return true;
    }

    void effacerSlot(int slot) {
        if (slot >= 0 && slot < static_cast<int>(slots.size())) {
            slots[static_cast<std::size_t>(slot)].reset();
        }
    }

    bool estKO() const { return PV_act <= 0; }

    void setPV(int newPV) {
        PV_act = std::clamp(newPV, 0, calculStat(PV));
    }

    void afficherEtat() {
        std::cout << getNom() << " - " << PV_act << "/" << calculStat(PV)
                  << " PV (VIT " << calculStat(VIT) << ")\n Vitesse theorique: ( ";

        const int evtmp = getEV(VIT);
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

class Joueur {
    std::string nom;
    std::vector<Creature> equipe;

public:
    explicit Joueur(std::string n = "") : nom(std::move(n)) {}

    void ajouterCreature(const Creature& c) {
        if (equipe.size() >= static_cast<std::size_t>(MAX_EQUIPE)) {
            std::cout << nom << " ne peut pas avoir plus de 6 creatures !\n";
            return;
        }
        equipe.push_back(c);
    }

    const std::string& getNom() const { return nom; }
    std::vector<Creature>& getEquipe() { return equipe; }
    const std::vector<Creature>& getEquipe() const { return equipe; }

    void afficherEquipe() {
        std::cout << "\nEquipe de " << nom << " :\n";
        for (std::size_t i = 0; i < equipe.size(); ++i) {
            std::cout << (i + 1) << ". ";
            equipe[i].afficherEtat();
        }
    }

    bool toutesCreaturesKO() const {
        if (equipe.empty()) return true;
        for (const auto& c : equipe) {
            if (!c.estKO()) return false;
        }
        return true;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Joueur& j) {
    os << j.getNom();
    return os;
}

struct Stages {
    std::array<int, STAT_COUNT> stats{};
    std::array<int, STAT_COUNT_PRE> precisionEva{};

    Stages() {
        stats.fill(0);
        precisionEva.fill(0);
    }
};

class Combat {
    std::array<Joueur*, 2> joueurs;
    std::array<int, 2> activeIndex;
    std::array<Stages, 2> mods;

public:
    Meteo meteoAct;
    int dureeMeteo;

    Champ champAct;
    int dureeChamp;

    Combat(Joueur& j1, Joueur& j2)
        : joueurs{&j1, &j2},
          activeIndex{0, 0},
          meteoAct(Meteo::Aucune),
          dureeMeteo(0),
          champAct(Champ::Aucun),
          dureeChamp(0) {}

    Joueur& getJoueur(int index) const { return *joueurs.at(static_cast<std::size_t>(index)); }
    Joueur& getP1() const { return *joueurs[0]; }
    Joueur& getP2() const { return *joueurs[1]; }

    void setActive(int joueurIndex, int creatureIndex) {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(joueurs.size())) return;

        auto& equipe = joueurs[static_cast<std::size_t>(joueurIndex)]->getEquipe();
        if (creatureIndex < 0 || creatureIndex >= static_cast<int>(equipe.size())) return;
        if (equipe[static_cast<std::size_t>(creatureIndex)].estKO()) return;

        activeIndex[static_cast<std::size_t>(joueurIndex)] = creatureIndex;
        resetStages(joueurIndex);
    }

    void setActiveP1(int i) { setActive(0, i); }
    void setActiveP2(int i) { setActive(1, i); }

    int getActiveIndex(int joueurIndex) const {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(activeIndex.size())) return -1;
        return activeIndex[static_cast<std::size_t>(joueurIndex)];
    }

    Creature* getActive(int joueurIndex) {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(joueurs.size())) return nullptr;

        auto& equipe = joueurs[static_cast<std::size_t>(joueurIndex)]->getEquipe();
        if (equipe.empty()) return nullptr;

        const int index = activeIndex[static_cast<std::size_t>(joueurIndex)];
        if (index < 0 || index >= static_cast<int>(equipe.size())) return nullptr;

        return &equipe[static_cast<std::size_t>(index)];
    }

    const Creature* getActive(int joueurIndex) const {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(joueurs.size())) return nullptr;

        const auto& equipe = joueurs[static_cast<std::size_t>(joueurIndex)]->getEquipe();
        if (equipe.empty()) return nullptr;

        const int index = activeIndex[static_cast<std::size_t>(joueurIndex)];
        if (index < 0 || index >= static_cast<int>(equipe.size())) return nullptr;

        return &equipe[static_cast<std::size_t>(index)];
    }

    Creature* getActiveP1() { return getActive(0); }
    Creature* getActiveP2() { return getActive(1); }

    int getJoueurIndex(const Creature* crea) const {
        if (!crea) return -1;
        if (getActive(0) == crea) return 0;
        if (getActive(1) == crea) return 1;
        return -1;
    }

    void resetStages(int joueurIndex) {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(mods.size())) return;
        mods[static_cast<std::size_t>(joueurIndex)] = Stages();
    }

    void resetStagesP1() { resetStages(0); }
    void resetStagesP2() { resetStages(1); }

    void changeStage(int joueurIndex, StatIndex stat, int var) {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(mods.size())) return;
        auto& value = mods[static_cast<std::size_t>(joueurIndex)]
                          .stats[static_cast<std::size_t>(stat)];
        value = std::clamp(value + var, MIN_MOD, MAX_MOD);
    }

    void changeStageP1(StatIndex stat, int var) { changeStage(0, stat, var); }
    void changeStageP2(StatIndex stat, int var) { changeStage(1, stat, var); }

    void changePrecision(int joueurIndex, StatPrecision stat, int var) {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(mods.size())) return;
        auto& value = mods[static_cast<std::size_t>(joueurIndex)]
                          .precisionEva[static_cast<std::size_t>(stat)];
        value = std::clamp(value + var, MIN_MOD, MAX_MOD);
    }

    void changePrecisionP1(StatPrecision stat, int var) { changePrecision(0, stat, var); }
    void changePrecisionP2(StatPrecision stat, int var) { changePrecision(1, stat, var); }

    int getStage(int joueurIndex, StatIndex stat) const {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(mods.size())) return 0;
        return mods[static_cast<std::size_t>(joueurIndex)]
            .stats[static_cast<std::size_t>(stat)];
    }

    int getStageP1(StatIndex stat) const { return getStage(0, stat); }
    int getStageP2(StatIndex stat) const { return getStage(1, stat); }

    int getPrecisionStage(int joueurIndex, StatPrecision stat) const {
        if (joueurIndex < 0 || joueurIndex >= static_cast<int>(mods.size())) return 0;
        return mods[static_cast<std::size_t>(joueurIndex)]
            .precisionEva[static_cast<std::size_t>(stat)];
    }

    int getPrecisionStageP1(StatPrecision stat) const { return getPrecisionStage(0, stat); }
    int getPrecisionStageP2(StatPrecision stat) const { return getPrecisionStage(1, stat); }

    static double stageMultiplier(int stage) {
        if (stage > 0) return (2.0 + stage) / 2.0;
        if (stage < 0) return 2.0 / (2.0 - stage);
        return 1.0;
    }

    static double stageMultiplierPrecision(int stage) {
        if (stage > 0) return (3.0 + stage) / 3.0;
        if (stage < 0) return 3.0 / (3.0 - stage);
        return 1.0;
    }

    double getStatMultiplier(int joueurIndex, StatIndex s) const {
        return stageMultiplier(getStage(joueurIndex, s));
    }

    double getStatMultiplierP1(StatIndex s) const { return getStatMultiplier(0, s); }
    double getStatMultiplierP2(StatIndex s) const { return getStatMultiplier(1, s); }

    double getPRE(int joueurIndex, StatPrecision stat) const {
        return stageMultiplierPrecision(getPrecisionStage(joueurIndex, stat));
    }

    double getPREP1(StatPrecision stat) const { return getPRE(0, stat); }
    double getPREP2(StatPrecision stat) const { return getPRE(1, stat); }
};

double getEfficacite(TypeEnum attaque, const std::vector<TypeEnum>& defenseurs,
                     const Combat& combat);

void seedRng(unsigned int seed);
int randomInt(int minInclusive, int maxInclusive);

double calculerDegatsPur(const Creature& attaquant, const Creature& defenseur,
                         const Attaque& atk, Combat& combat);
void effectuerAttaque(Creature& attaquant, Creature& defenseur,
                      Attaque& atk, Combat& combat);

void switchCombat(Combat& combat);
void switchBot(Combat& combat, int index = -1);
bool combatTermine(const Combat& combat);
void menuCombat(Combat& combat);

void appliquerEffets(const Attaque& atk, Creature& attaquant,
                     Creature& defenseur, Combat& combat);
