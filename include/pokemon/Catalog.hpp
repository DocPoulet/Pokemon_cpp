#pragma once

#include "pokemon/Mechanics.hpp"

#include <string>

namespace pokemon {

/**
 * Définition globale d'un talent disponible dans le catalogue.
 *
 * Entrées:
 *   id (std::string): Identifiant canonique anglais utilisé par Showdown.
 *   mechanic (Ability): Mécanique déjà comprise par le moteur, ou None si elle n'est pas encore simulée.
 *   generation (int): Génération d'introduction lorsque connue.
 *   nonStandard (std::string): Marqueur Showdown (Past, Future, LGPE...), vide pour une entrée standard.
 *   shortDescription (std::string): Résumé de l'effet destiné aux interfaces et outils.
 *
 * Sortie:
 *   AbilityData: Définition unique référencée par les espèces et Pokémon de combat.
 */
struct AbilityData {
    std::string id;
    Ability mechanic = Ability::None;
    int generation = 0;
    std::string nonStandard;
    std::string shortDescription;
    int nationalNumber = 0;

    /** Retourne true lorsque l'effet du talent est actuellement simulé par le moteur. */
    bool implemented() const { return mechanic != Ability::None; }
};

/**
 * Définition globale d'un objet disponible dans le catalogue.
 *
 * Entrées:
 *   id (std::string): Identifiant canonique anglais utilisé par Showdown.
 *   mechanic (HeldItem): Mécanique déjà comprise par le moteur, ou None si elle n'est pas encore simulée.
 *   generation (int): Génération d'introduction lorsque connue.
 *   nonStandard (std::string): Marqueur Showdown éventuel.
 *   shortDescription (std::string): Résumé de l'effet de l'objet.
 *
 * Sortie:
 *   ItemData: Définition unique pouvant être référencée par tous les Pokémon.
 */
struct ItemData {
    std::string id;
    HeldItem mechanic = HeldItem::None;
    int generation = 0;
    std::string nonStandard;
    std::string shortDescription;
    int nationalNumber = 0;

    /** Retourne true lorsque l'effet tenu de l'objet est actuellement simulé par le moteur. */
    bool implemented() const { return mechanic != HeldItem::None; }
};

} // namespace pokemon
