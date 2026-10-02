#pragma once

#include "pokemon/Catalog.hpp"
#include "pokemon/Move.hpp"
#include "pokemon/Types.hpp"

#include <array>
#include <string>
#include <string_view>
#include <vector>

namespace pokemon {

/**
 * Décrit les données globales communes à tous les Pokémon d'une même espèce.
 *
 * Entrées:
 *   name (std::string): Identifiant canonique anglais de l'espèce.
 *   baseStats: Statistiques de base de l'espèce.
 *   types (std::vector<Type>): Un ou deux types élémentaires.
 *   movePool: Références vers les attaques globales autorisées.
 *   abilities: Références vers les talents globaux autorisés.
 *
 * Sortie:
 *   PokemonSpecies: Définition globale qui ne duplique aucune attaque ni aucun talent.
 */
class PokemonSpecies {
public:
    PokemonSpecies(std::string name,
                   std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
                   std::vector<Type> types,
                   std::vector<const MoveData*> movePool = {},
                   std::vector<const AbilityData*> abilities = {});

    const std::string& name() const;
    int baseStat(Stat stat) const;
    const std::vector<Type>& types() const;

    /** Retourne les références vers les attaques globales apprenables. */
    const std::vector<const MoveData*>& movePool() const;

    /** Retourne les références vers les talents globaux autorisés. */
    const std::vector<const AbilityData*>& abilities() const;

    /** Vérifie si l'espèce peut apprendre l'attaque globale donnée. */
    bool canLearnMove(const MoveData* move) const;

    /** Vérifie si un ID d'attaque appartient au movepool. */
    bool canLearnMove(std::string_view moveId) const;

    /** Vérifie si le talent global donné est autorisé. */
    bool canHaveAbility(const AbilityData* ability) const;

    /** Vérifie si un ID de talent appartient aux talents possibles. */
    bool canHaveAbility(std::string_view abilityId) const;

private:
    std::string name_;
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats_{};
    std::vector<Type> types_;
    std::vector<const MoveData*> movePool_;
    std::vector<const AbilityData*> abilities_;
};

} // namespace pokemon
