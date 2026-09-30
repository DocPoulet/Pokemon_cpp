#pragma once

#include "pokemon/Pokemon.hpp"
#include <string>
#include <vector>

namespace pokemon {

/**
 * Représente un dresseur et son équipe de Pokémon.
 *
 * Entrées:
 *   name (std::string): Nom affiché du dresseur.
 *   Pokemon: Créatures ajoutées progressivement avec addPokemon().
 *
 * Sortie:
 *   Trainer : objet contenant jusqu'à six Pokémon et permettant de consulter l'état de l'équipe.
 */
class Trainer {
public:
    /**
     * Construit un dresseur avec une équipe vide.
     *
     * Entrées:
     *   name (std::string): Nom du dresseur.
     *
     * Sortie:
     *   Trainer: Dresseur initialisé avec aucune créature dans son équipe.
     */
    explicit Trainer(std::string name = "Dresseur");

    /**
     * Retourne le nom du dresseur.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Référence vers le nom stocké.
     */
    const std::string& name() const;

    /**
     * Ajoute un Pokémon à l'équipe.
     *
     * Entrées:
     *   pokemon (const Pokemon&): Pokémon à copier dans l'équipe.
     *
     * Sortie:
     *   bool: true si le Pokémon a été ajouté, false si l'équipe contient déjà six Pokémon.
     */
    bool addPokemon(const Pokemon& pokemon);

    /**
     * Retourne l'équipe avec accès en modification.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   std::vector<Pokemon>&: Référence vers le conteneur interne de l'équipe.
     */
    std::vector<Pokemon>& team();

    /**
     * Retourne l'équipe en lecture seule.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::vector<Pokemon>&: Référence constante vers le conteneur interne.
     */
    const std::vector<Pokemon>& team() const;

    /**
     * Indique si le dresseur n'a plus de Pokémon capables de combattre.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   bool: true si l'équipe est vide ou si tous les Pokémon sont KO.
     */
    bool allFainted() const;

private:
    std::string name_; ///< Nom du dresseur.
    std::vector<Pokemon> team_; ///< Équipe du dresseur, limitée à six Pokémon par addPokemon().
};

} // namespace pokemon
