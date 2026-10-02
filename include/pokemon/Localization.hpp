#pragma once

#include <string>
#include <string_view>

namespace pokemon {
class Pokemon;

/**
 * Traduit uniquement les libellés affichés à l'utilisateur.
 *
 * Entrées:
 *   Identifiants canoniques anglais utilisés par le moteur et les fichiers de données.
 *
 * Sortie:
 *   Nom français destiné à l'interface. Si aucune traduction n'existe, l'identifiant anglais est conservé.
 *
 * Notes:
 *   Cette couche ne doit jamais être utilisée comme identifiant métier ou clé de catalogue.
 */
std::string_view frenchSpeciesName(std::string_view englishName);
std::string_view frenchMoveName(std::string_view englishName);
std::string_view frenchNatureName(std::string_view englishName);
std::string_view frenchAbilityName(std::string_view englishName);
std::string_view frenchItemName(std::string_view englishName);

/**
 * Retourne le nom à afficher en combat.
 *
 * Entrées:
 *   pokemon (const Pokemon&): Pokémon concerné.
 *
 * Sortie:
 *   std::string: Surnom s'il existe, sinon nom d'espèce traduit en français.
 */
std::string battleDisplayName(const Pokemon& pokemon);

} // namespace pokemon
