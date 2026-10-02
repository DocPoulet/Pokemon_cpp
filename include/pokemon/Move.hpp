#pragma once

#include "pokemon/Mechanics.hpp"
#include "pokemon/Types.hpp"
#include <string>
#include <vector>

namespace pokemon {

/**
 * Décrit la nature d'un effet secondaire porté par une attaque.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   EffectKind : indique au moteur comment interpréter les autres champs de MoveEffect.
 */
enum class EffectKind {
    StatChange,       ///< Modifie une statistique principale.
    AccuracyChange,   ///< Modifie la précision ou l'esquive.
    SetWeather,       ///< Installe une météo.
    SetTerrain,       ///< Installe un terrain.
    RandomStatChange, ///< Modifie aléatoirement une statistique.
    ApplyStatus       ///< Applique un statut persistant à la cible.
};

/**
 * Décrit un effet secondaire associé à une attaque.
 *
 * Entrées:
 *   kind (EffectKind): Type d'effet à appliquer.
 *   target (Target): Pokémon qui reçoit l'effet.
 *   stat / accuracyStat: Statistique concernée selon le type d'effet.
 *   stages (int): Nombre de crans ajoutés ou retirés.
 *   chancePercent (int): Probabilité d'application en pourcentage.
 *   weather / terrain: Valeur à installer pour les effets correspondants.
 *   status (StatusCondition): Statut persistant à appliquer pour ApplyStatus.
 *
 * Sortie:
 *   MoveEffect : configuration déclarative lue par Battle lors de la résolution d'une attaque.
 */
struct MoveEffect {
    EffectKind kind = EffectKind::StatChange;
    Target target = Target::Opponent;
    Stat stat = Stat::Attack;
    AccuracyStat accuracyStat = AccuracyStat::Accuracy;
    int stages = 0;
    int chancePercent = 100;
    Weather weather = Weather::None;
    Terrain terrain = Terrain::None;
    StatusCondition status = StatusCondition::None;
};

/**
 * Décrit les données fixes et partagées d'une attaque.
 *
 * Entrées:
 *   name (std::string): Nom affiché de l'attaque.
 *   type (Type): Type élémentaire.
 *   power (int): Puissance de base, 0 pour une attaque de statut.
 *   category (MoveCategory): Physique, spéciale ou statut.
 *   accuracy (int): Précision en pourcentage ; une valeur négative signifie infaillible.
 *   criticalBonus (int): Bonus appliqué au taux de critique.
 *   pp (int): Nombre de PP de base.
 *   recoilThird (bool): Active un recul d'un tiers des dégâts infligés.
 *   struggle (bool): Indique qu'il s'agit de Lutte.
 *   effects (std::vector<MoveEffect>): Effets secondaires éventuels.
 *   priority (int): Priorité d'exécution avant comparaison de la Vitesse.
 *   recoilPercent (int): Pourcentage des dégâts infligés rendu en contrecoup.
 *   drainPercent (int): Pourcentage des dégâts infligés converti en soin.
 *   minHits / maxHits (int): Nombre minimal et maximal de coups pour une attaque multi-coups.
 *
 * Sortie:
 *   MoveData : définition réutilisable par plusieurs Pokémon sans partager leurs PP actuels.
 */
struct MoveData {
    std::string name;
    Type type = Type::Neutral;
    int power = 0;
    MoveCategory category = MoveCategory::Status;
    int accuracy = 100;
    int criticalBonus = 0;
    int pp = 5;
    bool recoilThird = false;
    bool struggle = false;
    std::vector<MoveEffect> effects;
    int priority = 0;
    int recoilPercent = 0;
    int drainPercent = 0;
    int minHits = 1;
    int maxHits = 1;

    // Métadonnées du catalogue Showdown. Elles n'affectent pas directement le calcul des dégâts.
    int generation = 0;
    int nationalNumber = 0;
    std::string nonStandard;
    std::string shortDescription;
    bool catalogMechanicsComplete = false;
};

/**
 * Représente une attaque équipée par un Pokémon avec ses PP propres.
 *
 * Entrées:
 *   data (const MoveData*): Définition partagée de l'attaque.
 *
 * Sortie:
 *   MoveInstance : instance locale capable de suivre et restaurer ses PP.
 */
class MoveInstance {
public:
    /**
     * Construit une instance d'attaque.
     *
     * Entrées:
     *   data (const MoveData*): Définition de l'attaque, ou nullptr pour un slot vide.
     *
     * Sortie:
     *   MoveInstance: Instance dont les PP actuels sont initialisés aux PP maximums.
     */
    explicit MoveInstance(const MoveData* data = nullptr);

    /**
     * Retourne la définition partagée de l'attaque.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const MoveData*: Pointeur vers la définition, ou nullptr si l'instance est vide.
     */
    const MoveData* data() const;

    /**
     * Retourne le nombre de PP actuellement disponibles.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Nombre de PP restants, toujours supérieur ou égal à 0.
     */
    int currentPP() const;

    /**
     * Retourne le nombre maximum de PP de l'attaque.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: PP maximums définis dans MoveData, ou 0 si aucune attaque n'est liée.
     */
    int maxPP() const;

    /**
     * Indique si l'attaque peut actuellement être utilisée.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   bool: true si une définition existe et qu'au moins un PP est disponible.
     */
    bool usable() const;

    /**
     * Consomme un PP de l'attaque.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Décrémente currentPP_ de 1 uniquement si un PP est encore disponible.
     */
    void consumePP();

    /**
     * Restaure tous les PP de l'attaque.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Replace currentPP_ à la valeur maximale définie par MoveData.
     */
    void restorePP();

private:
    const MoveData* data_ = nullptr; ///< Définition partagée de l'attaque.
    int currentPP_ = 0;             ///< Nombre de PP actuellement disponibles.
};

} // namespace pokemon
