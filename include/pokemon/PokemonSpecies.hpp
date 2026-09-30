#pragma once

#include "pokemon/Mechanics.hpp"
#include "pokemon/Types.hpp"

#include <array>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Décrit les données globales communes à tous les Pokémon d'une même espèce.
 *
 * Entrées:
 *   name (std::string): Nom canonique de l'espèce.
 *   baseStats: Statistiques de base HP, Attaque, Défense, Attaque Spéciale,
 *              Défense Spéciale et Vitesse.
 *   types (std::vector<Type>): Un ou deux types élémentaires.
 *   movePool (std::vector<std::string>): Attaques que l'espèce est autorisée à utiliser.
 *   abilities (std::vector<Ability>): Talents que l'espèce peut posséder.
 *
 * Sortie:
 *   PokemonSpecies: définition globale immuable chargée depuis pokemon_species.json.
 */
class PokemonSpecies {
public:
    PokemonSpecies(std::string name,
                   std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
                   std::vector<Type> types,
                   std::vector<std::string> movePool = {},
                   std::vector<Ability> abilities = {});

    /** Retourne le nom canonique de l'espèce. Entrées: aucune. Sortie: nom stocké. */
    const std::string& name() const;

    /** Retourne une statistique de base. Entrée: Stat. Sortie: valeur numérique correspondante. */
    int baseStat(Stat stat) const;

    /** Retourne les types de l'espèce. Entrées: aucune. Sortie: liste des types. */
    const std::vector<Type>& types() const;

    /**
     * Retourne le movepool autorisé pour cette espèce.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::vector<std::string>&: noms des attaques autorisées dans les presets et le Team Builder.
     */
    const std::vector<std::string>& movePool() const;

    /**
     * Retourne les talents possibles de l'espèce.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::vector<Ability>&: talents qu'un Pokémon de cette espèce peut sélectionner.
     */
    const std::vector<Ability>& abilities() const;

    /** Vérifie si l'espèce peut apprendre une attaque. Entrée: nom d'attaque. Sortie: bool. */
    bool canLearnMove(const std::string& moveName) const;

    /** Vérifie si un talent est autorisé pour l'espèce. Entrée: Ability. Sortie: bool. */
    bool canHaveAbility(Ability ability) const;

private:
    std::string name_;
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats_{};
    std::vector<Type> types_;
    std::vector<std::string> movePool_;
    std::vector<Ability> abilities_;
};

} // namespace pokemon
