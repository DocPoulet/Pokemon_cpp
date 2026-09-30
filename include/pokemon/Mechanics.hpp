#pragma once

#include <string_view>

namespace pokemon {

/**
 * Représente un problème de statut persistant appliqué à un Pokémon.
 *
 * Entrées:
 *   Aucune : les valeurs sont utilisées par Pokemon et Battle.
 *
 * Sortie:
 *   StatusCondition : état persistant influençant les actions ou les PV.
 */
enum class StatusCondition {
    None,       ///< Aucun statut persistant.
    Burn,       ///< Brûlure : réduit l'attaque physique et inflige des dégâts de fin de tour.
    Poison,     ///< Poison : inflige des dégâts de fin de tour.
    Paralysis,  ///< Paralysie : réduit la Vitesse et peut empêcher d'agir.
    Sleep,      ///< Sommeil : empêche d'agir pendant plusieurs tours.
    Freeze      ///< Gel : empêche d'agir jusqu'au dégel.
};

/**
 * Représente un objet tenu dont l'effet est géré par le moteur de combat.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   HeldItem : objet porté par une instance de Pokemon.
 */
enum class HeldItem {
    None,       ///< Aucun objet.
    Leftovers,  ///< Restes : soigne 1/16 des PV max en fin de tour.
    LifeOrb,    ///< Orbe Vie : augmente les dégâts de 30 % et inflige un recul après une attaque offensive.
    RedOrb,     ///< Orbe Rouge : objet réservé aux mécaniques de Primo-Groudon.
    BlueOrb     ///< Orbe Bleue : objet réservé aux mécaniques de Primo-Kyogre.
};

/**
 * Représente un talent passif supporté par la v0.5.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   Ability : talent consulté automatiquement par le moteur.
 */
enum class Ability {
    None,      ///< Aucun talent particulier.
    Blaze,     ///< Brasier : renforce le type Feu sous un tiers des PV.
    Torrent,   ///< Torrent : renforce le type Eau sous un tiers des PV.
    Overgrow,  ///< Engrais : renforce le type Plante sous un tiers des PV.
    Levitate,  ///< Lévitation : immunise contre les attaques Sol.
    Guts,          ///< Cran : augmente l'Attaque physique lorsqu'un statut est présent et ignore le malus de brûlure.
    PrimordialSea, ///< Mer Primaire : talent de météo primordiale.
    DesolateLand,  ///< Terre Finale : talent de météo primordiale.
    DeltaStream,   ///< Souffle Delta : talent de vents mystérieux.
    Static,        ///< Statik : talent possible de Pikachu ; effet de contact non implémenté.
    Adaptability   ///< Adaptabilité : talent possible d'Évoli ; effet mécanique non implémenté.
};

/**
 * Convertit un statut en texte lisible.
 *
 * Entrées:
 *   status (StatusCondition): Statut à convertir.
 *
 * Sortie:
 *   std::string_view: Nom français court du statut.
 */
std::string_view toString(StatusCondition status);

/**
 * Convertit un objet tenu en texte lisible.
 *
 * Entrées:
 *   item (HeldItem): Objet à convertir.
 *
 * Sortie:
 *   std::string_view: Nom lisible de l'objet.
 */
std::string_view toString(HeldItem item);

/**
 * Convertit un talent en texte lisible.
 *
 * Entrées:
 *   ability (Ability): Talent à convertir.
 *
 * Sortie:
 *   std::string_view: Nom lisible du talent.
 */
std::string_view toString(Ability ability);

} // namespace pokemon
