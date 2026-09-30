#pragma once

#include <cstddef>
#include <string_view>

namespace pokemon {

/**
 * Représente une statistique principale d'un Pokémon.
 *
 * Entrées:
 *   Aucune : les valeurs de l'enum sont utilisées directement.
 *
 * Sortie:
 *   Stat : valeur servant notamment d'index dans les tableaux de statistiques.
 */
enum class Stat : std::size_t {
    HP = 0,          ///< Points de vie.
    Attack,          ///< Attaque physique.
    Defense,         ///< Défense physique.
    SpecialAttack,   ///< Attaque spéciale.
    SpecialDefense,  ///< Défense spéciale.
    Speed,           ///< Vitesse.
    Count            ///< Nombre total de statistiques ; ne représente pas une statistique jouable.
};

/**
 * Représente les statistiques liées à la précision et à l'esquive.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   AccuracyStat : valeur utilisée pour sélectionner le modificateur concerné.
 */
enum class AccuracyStat : std::size_t {
    Accuracy = 0, ///< Précision des attaques.
    Evasion,      ///< Esquive du Pokémon ciblé.
    Count         ///< Nombre de valeurs utilisables ; valeur technique.
};

/**
 * Représente les types élémentaires supportés par le moteur de combat.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   Type : valeur utilisée pour le STAB, les résistances et les faiblesses.
 */
enum class Type : std::size_t {
    Steel = 0, Fighting, Dragon, Water, Electric, Fairy,
    Fire, Ice, Bug, Normal, Grass, Poison, Psychic, Rock,
    Ground, Ghost, Dark, Flying,
    Neutral, ///< Type neutre utilisé notamment par Lutte.
    Count    ///< Nombre de types ; valeur technique.
};

/**
 * Définit la catégorie mécanique d'une attaque.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   MoveCategory : indique quelle formule de dégâts doit être utilisée.
 */
enum class MoveCategory {
    Status = 0, ///< Attaque sans dégâts directs.
    Physical,   ///< Attaque basée sur Attaque et Défense.
    Special     ///< Attaque basée sur Attaque Spéciale et Défense Spéciale.
};

/**
 * Représente la météo actuellement active pendant un combat.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   Weather : valeur utilisée par le moteur pour modifier certaines mécaniques.
 */
enum class Weather {
    None,      ///< Aucune météo particulière.
    Sun,       ///< Soleil.
    Rain,      ///< Pluie.
    Sandstorm, ///< Tempête de sable.
    Snow,       ///< Neige.
    ExtremelyHarshSunlight, ///< Soleil Intense.
    HeavyRain, ///< Pluie Battante.
    StrongWinds  ///< Vent Mysterieux.
};

/**
 * Représente le terrain actuellement actif pendant un combat.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   Terrain : valeur utilisée par le moteur pour les effets de terrain.
 */
enum class Terrain {
    None,     ///< Aucun terrain particulier.
    Electric, ///< Champ électrifié.
    Misty,    ///< Champ brumeux.
    Grassy,   ///< Champ herbu.
    Psychic   ///< Champ psychique.
};

/**
 * Indique la cible logique d'un effet secondaire d'attaque.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   Target : permet au moteur de savoir quel Pokémon reçoit l'effet.
 */
enum class Target {
    Self,     ///< Le lanceur de l'attaque.
    Opponent  ///< Le Pokémon adverse.
};

/**
 * Convertit une statistique en texte lisible.
 *
 * Entrées:
 *   stat (Stat): Statistique à convertir.
 *
 * Sortie:
 *   std::string_view: Nom français lisible de la statistique.
 */
std::string_view toString(Stat stat);

/**
 * Convertit un type Pokémon en texte lisible.
 *
 * Entrées:
 *   type (Type): Type à convertir.
 *
 * Sortie:
 *   std::string_view: Nom lisible du type.
 */
std::string_view toString(Type type);

/**
 * Convertit une catégorie d'attaque en texte lisible.
 *
 * Entrées:
 *   category (MoveCategory): Catégorie à convertir.
 *
 * Sortie:
 *   std::string_view: Libellé de la catégorie.
 */
std::string_view toString(MoveCategory category);

/**
 * Convertit une météo en texte lisible.
 *
 * Entrées:
 *   weather (Weather): Météo à convertir.
 *
 * Sortie:
 *   std::string_view: Nom lisible de la météo.
 */
std::string_view toString(Weather weather);

/**
 * Convertit un terrain en texte lisible.
 *
 * Entrées:
 *   terrain (Terrain): Terrain à convertir.
 *
 * Sortie:
 *   std::string_view: Nom lisible du terrain.
 */
std::string_view toString(Terrain terrain);

} // namespace pokemon
