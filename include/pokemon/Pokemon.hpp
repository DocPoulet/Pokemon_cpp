#pragma once

#include "pokemon/Catalog.hpp"
#include "pokemon/Mechanics.hpp"
#include "pokemon/Move.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <array>
#include <optional>
#include <string>

namespace pokemon {

/**
 * Représente un Pokémon individuel utilisable en combat.
 *
 * Entrées:
 *   species (const PokemonSpecies*): Espèce de référence.
 *   level (int): Niveau du Pokémon.
 *   nature (Nature): Nature influençant certaines statistiques.
 *   IV / EV / attaques: Données modifiables après construction via les méthodes dédiées.
 *
 * Sortie:
 *   Pokemon : instance possédant ses propres PV, IV, EV et PP d'attaques.
 */
class Pokemon {
public:
    /**
     * Construit un Pokémon à partir d'une espèce.
     *
     * Entrées:
     *   species (const PokemonSpecies*): Espèce utilisée pour les stats de base et les types.
     *   level (int): Niveau initial du Pokémon.
     *   nature (Nature): Nature initiale.
     *
     * Sortie:
     *   Pokemon: Instance initialisée avec ses PV au maximum.
     */
    Pokemon(const PokemonSpecies* species,
            int level = 50,
            Nature nature = Nature{},
            std::string nickname = "",
            std::string form = "Base");

    /**
     * Retourne l'espèce du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const PokemonSpecies&: Référence vers la définition d'espèce utilisée.
     */
    const PokemonSpecies& species() const;

    /**
     * Retourne le nom visible du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Surnom s'il est défini, sinon nom de l'espèce.
     */
    const std::string& name() const;


    /**
     * Retourne le surnom explicite du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Surnom stocké ; chaîne vide si le nom d'espèce doit être utilisé.
     */
    const std::string& nickname() const;

    /**
     * Modifie le surnom.
     *
     * Entrées:
     *   nickname (std::string): Nouveau surnom ; une chaîne vide rétablit le nom d'espèce à l'affichage.
     *
     * Sortie:
     *   Aucune.
     */
    void setNickname(std::string nickname);

    /**
     * Retourne l'identifiant de forme préparé pour les futures formes alternatives.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Identifiant de forme, actuellement informatif uniquement.
     */
    const std::string& form() const;

    /**
     * Modifie l'identifiant de forme sans changer les mécaniques de combat.
     *
     * Entrées:
     *   form (std::string): Identifiant non vide, par exemple "Base".
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Le champ est uniquement persistant en v0.7 ; il ne modifie ni stats, ni types, ni talent.
     */
    void setForm(std::string form);

    /**
     * Retourne le niveau actuel du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Niveau stocké dans l'instance.
     */

    /** Retourne le sexe importé ("M", "F" ou vide). Entrées: aucune. Sortie: chaîne stockée. */
    const std::string& gender() const;

    /** Modifie le sexe informatif. Entrée: "M", "F" ou vide. Sortie: aucune. */
    void setGender(std::string gender);

    /** Indique si le Pokémon est chromatique. Entrées: aucune. Sortie: bool. */
    bool shiny() const;

    /** Modifie le marqueur chromatique. Entrée: bool. Sortie: aucune. */
    void setShiny(bool shiny);

    /** Retourne le type Téracristal préparé pour une future mécanique. Sortie: type optionnel. */
    const std::optional<Type>& teraType() const;

    /** Modifie le type Téracristal informatif. Entrée: type optionnel. Sortie: aucune. */
    void setTeraType(std::optional<Type> type);

    /** Retourne le niveau Dynamax importé. Entrées: aucune. Sortie: entier entre 0 et 10. */
    int dynamaxLevel() const;

    /** Modifie le niveau Dynamax informatif. Entrée: entier entre 0 et 10. Sortie: aucune. */
    void setDynamaxLevel(int level);

    /** Indique si le preset demande la forme Gigamax. Entrées: aucune. Sortie: bool. */
    bool gigantamax() const;

    /** Modifie le marqueur Gigamax informatif. Entrée: bool. Sortie: aucune. */
    void setGigantamax(bool enabled);

    int level() const;

    /**
     * Retourne les PV actuellement disponibles.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Valeur comprise entre 0 et maxHP().
     */
    int currentHP() const;

    /**
     * Calcule les PV maximums à partir de l'espèce, du niveau, des IV et des EV.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Nombre maximal de PV de ce Pokémon.
     */
    int maxHP() const;

    /**
     * Indique si le Pokémon est KO.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   bool: true lorsque currentHP() vaut 0.
     */
    bool fainted() const;

    /**
     * Remplace directement les PV actuels.
     *
     * Entrées:
     *   hp (int): Nouvelle valeur souhaitée.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   La valeur est automatiquement bornée entre 0 et maxHP().
     */
    void setHP(int hp);

    /**
     * Retire des PV au Pokémon.
     *
     * Entrées:
     *   amount (int): Quantité de dégâts à appliquer.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Les PV ne peuvent pas descendre sous 0.
     */
    void damage(int amount);

    /**
     * Rend des PV au Pokémon.
     *
     * Entrées:
     *   amount (int): Quantité de PV à restaurer.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Les PV ne peuvent pas dépasser maxHP().
     */
    void heal(int amount);

    /**
     * Calcule la valeur finale d'une statistique.
     *
     * Entrées:
     *   stat (Stat): Statistique à calculer.
     *
     * Sortie:
     *   int: Valeur obtenue avec les stats de base, le niveau, les IV, les EV et la nature.
     */
    int stat(Stat stat) const;

    /**
     * Retourne l'IV d'une statistique.
     *
     * Entrées:
     *   stat (Stat): Statistique à consulter.
     *
     * Sortie:
     *   int: IV compris entre 0 et 31.
     */
    int iv(Stat stat) const;

    /**
     * Retourne l'EV d'une statistique.
     *
     * Entrées:
     *   stat (Stat): Statistique à consulter.
     *
     * Sortie:
     *   int: EV compris entre 0 et 252.
     */
    int ev(Stat stat) const;

    /**
     * Modifie l'IV d'une statistique.
     *
     * Entrées:
     *   stat (Stat): Statistique à modifier.
     *   value (int): Nouvelle valeur souhaitée.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   La valeur est bornée automatiquement entre 0 et 31.
     */
    void setIV(Stat stat, int value);

    /**
     * Modifie l'EV d'une statistique en respectant les limites Pokémon.
     *
     * Entrées:
     *   stat (Stat): Statistique à modifier.
     *   value (int): Nouvelle valeur souhaitée pour cette statistique.
     *
     * Sortie:
     *   bool: true si la valeur demandée a pu être appliquée entièrement,
     *         false si la limite totale de 510 EV empêche de l'atteindre.
     *
     * Effets:
     *   Chaque statistique est limitée à 252 EV et le total à 510 EV.
     */
    bool setEV(Stat stat, int value);

    /**
     * Calcule la somme de tous les EV du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Total des EV, normalement compris entre 0 et 510.
     */
    int totalEV() const;

    /**
     * Retourne la nature actuelle du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const Nature&: Référence vers la nature stockée.
     */
    const Nature& nature() const;

    /**
     * Remplace la nature du Pokémon.
     *
     * Entrées:
     *   nature (Nature): Nouvelle nature à utiliser.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Les statistiques calculées avec stat() peuvent changer immédiatement.
     */
    void setNature(Nature nature);

    /**
     * Équipe une attaque dans l'un des quatre slots du Pokémon.
     *
     * Entrées:
     *   slot (std::size_t): Index du slot, entre 0 et 3.
     *   move (const MoveData*): Définition de l'attaque à équiper.
     *
     * Sortie:
     *   bool: true si le slot est valide et l'attaque a été équipée, sinon false.
     *
     * Effets:
     *   Une nouvelle MoveInstance est créée avec ses propres PP.
     */
    bool setMove(std::size_t slot, const MoveData* move);

    /**
     * Vide un slot d'attaque.
     *
     * Entrées:
     *   slot (std::size_t): Index du slot à vider, entre 0 et 3.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Le slot devient vide si l'index est valide.
     */
    void clearMove(std::size_t slot);

    /**
     * Retourne les quatre slots d'attaque avec accès en modification.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   std::array<std::optional<MoveInstance>, 4>&: Référence vers les slots internes.
     */
    std::array<std::optional<MoveInstance>, 4>& moves();

    /**
     * Retourne les quatre slots d'attaque en lecture seule.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::array<std::optional<MoveInstance>, 4>&: Référence constante vers les slots.
     */
    const std::array<std::optional<MoveInstance>, 4>& moves() const;

    /**
     * Vérifie si le Pokémon possède au moins une attaque encore utilisable.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   bool: true si au moins un slot contient une attaque avec des PP restants.
     */
    bool hasUsableMove() const;

    /**
     * Retourne le statut persistant actuellement appliqué.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   StatusCondition: Statut courant du Pokémon.
     */
    StatusCondition status() const;

    /**
     * Applique un statut persistant au Pokémon.
     *
     * Entrées:
     *   status (StatusCondition): Nouveau statut.
     *   turns (int): Durée initiale utilisée pour le sommeil.
     *
     * Sortie:
     *   bool: true si le statut a été appliqué, false si un autre statut était déjà présent.
     */
    bool setStatus(StatusCondition status, int turns = 0);

    /**
     * Retire le statut persistant du Pokémon.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Aucune.
     */
    void cureStatus();

    /**
     * Retourne le compteur interne associé au statut temporaire.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: Nombre de tours restants, principalement utilisé pour le sommeil.
     */
    int statusTurns() const;

    /**
     * Modifie le compteur interne du statut temporaire.
     *
     * Entrées:
     *   turns (int): Nouvelle durée, automatiquement bornée à 0 minimum.
     *
     * Sortie:
     *   Aucune.
     */
    void setStatusTurns(int turns);

    /**
     * Retourne la définition globale de l'objet tenu.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const ItemData*: Référence globale vers l'objet, ou nullptr si aucun objet.
     */
    const ItemData* heldItem() const;

    /** Équipe un objet global. Entrée: pointeur de catalogue ou nullptr. Sortie: aucune. */
    void setHeldItem(const ItemData* item);

    /**
     * Retourne la définition globale du talent actif.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const AbilityData*: Référence globale vers le talent, ou nullptr pour MissingNo.
     */
    const AbilityData* ability() const;

    /** Active un talent global. Entrée: pointeur de catalogue ou nullptr. Sortie: aucune. */
    void setAbility(const AbilityData* ability);

private:
    const PokemonSpecies* species_ = nullptr;
    std::string nickname_; ///< Surnom optionnel ; vide signifie utiliser le nom de l'espèce.
    std::string form_ = "Base"; ///< Forme persistée mais sans effet mécanique.
    std::string gender_; ///< Sexe importé depuis Showdown : M, F ou vide.
    bool shiny_ = false; ///< Marqueur chromatique sans effet mécanique.
    std::optional<Type> teraType_; ///< Type Téracristal conservé pour une future mécanique.
    int dynamaxLevel_ = 10; ///< Niveau Dynamax conservé pour une future mécanique.
    bool gigantamax_ = false; ///< Marqueur Gigamax conservé pour une future mécanique.
    int level_ = 1; ///< Niveau actuel.
    int currentHP_ = 0; ///< PV actuellement disponibles.
    std::array<int, static_cast<std::size_t>(Stat::Count)> ivs_{}; ///< IV par statistique.
    std::array<int, static_cast<std::size_t>(Stat::Count)> evs_{}; ///< EV par statistique.
    Nature nature_; ///< Nature actuelle.
    std::array<std::optional<MoveInstance>, 4> moves_{}; ///< Quatre attaques équipées au maximum.
    StatusCondition status_ = StatusCondition::None; ///< Statut persistant actuel.
    int statusTurns_ = 0; ///< Compteur utilisé par les statuts temporaires comme le sommeil.
    const ItemData* heldItem_ = nullptr; ///< Référence vers l'objet global du catalogue.
    const AbilityData* ability_ = nullptr; ///< Référence vers le talent global du catalogue.
};

} // namespace pokemon
