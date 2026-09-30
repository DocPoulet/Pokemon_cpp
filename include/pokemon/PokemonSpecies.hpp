#pragma once

#include "pokemon/Types.hpp"
#include <array>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Décrit les données communes à tous les Pokémon d'une même espèce.
 *
 * Entrées:
 *   name (std::string): Nom de l'espèce.
 *   baseStats (std::array<int, ...>): Statistiques de base de l'espèce.
 *   types (std::vector<Type>): Un ou plusieurs types élémentaires.
 *
 * Sortie:
 *   PokemonSpecies : définition immuable consultée par les instances de Pokemon.
 */
class PokemonSpecies {
public:
    /**
     * Construit une définition d'espèce.
     *
     * Entrées:
     *   name (std::string): Nom de l'espèce.
     *   baseStats (std::array<int, ...>): Valeurs de base pour HP, Attaque, Défense, etc.
     *   types (std::vector<Type>): Types de l'espèce.
     *
     * Sortie:
     *   PokemonSpecies: Espèce initialisée avec ces données.
     */
    PokemonSpecies(std::string name,
                   std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
                   std::vector<Type> types);

    /**
     * Retourne le nom de l'espèce.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Référence vers le nom stocké.
     */
    const std::string& name() const;

    /**
     * Retourne la statistique de base demandée.
     *
     * Entrées:
     *   stat (Stat): Statistique à consulter.
     *
     * Sortie:
     *   int: Valeur de base de cette statistique pour l'espèce.
     */
    int baseStat(Stat stat) const;

    /**
     * Retourne les types de l'espèce.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::vector<Type>&: Liste des types dans l'ordre où ils ont été définis.
     */
    const std::vector<Type>& types() const;

private:
    std::string name_; ///< Nom de l'espèce.
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats_{}; ///< Statistiques de base.
    std::vector<Type> types_; ///< Types élémentaires de l'espèce.
};

} // namespace pokemon
