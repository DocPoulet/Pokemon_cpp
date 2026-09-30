#pragma once

#include "pokemon/Mechanics.hpp"
#include "pokemon/Types.hpp"

#include <string>
#include <string_view>

namespace pokemon {

/**
 * Convertit les enums du moteur vers des identifiants stables pour les fichiers JSON.
 *
 * Entrées:
 *   Une valeur de type Stat, Type, Ability ou HeldItem.
 *
 * Sortie:
 *   std::string_view: identifiant ASCII stable, indépendant du texte affiché à l'utilisateur.
 */
std::string_view dataId(Stat value);
std::string_view dataId(Type value);
std::string_view dataId(Ability value);
std::string_view dataId(HeldItem value);

/**
 * Reconstruit les enums du moteur depuis les identifiants JSON.
 *
 * Entrées:
 *   id (std::string_view): identifiant exact présent dans les fichiers de données.
 *
 * Sortie:
 *   Valeur enum correspondante.
 *
 * Erreurs:
 *   Lance std::invalid_argument si l'identifiant est inconnu.
 */
Stat statFromDataId(std::string_view id);
Type typeFromDataId(std::string_view id);
Ability abilityFromDataId(std::string_view id);
HeldItem heldItemFromDataId(std::string_view id);

} // namespace pokemon
