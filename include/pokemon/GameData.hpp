#pragma once

#include "pokemon/Move.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <map>
#include <string>

namespace pokemon {

/**
 * Centralise les données statiques connues du prototype.
 *
 * Entrées:
 *   Aucune donnée externe : le constructeur charge actuellement le catalogue intégré au code.
 *
 * Sortie:
 *   GameData : registre permettant de retrouver une attaque ou une espèce par son nom.
 */
class GameData {
public:
    /**
     * Construit et remplit le catalogue de données du jeu.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   GameData: Catalogue prêt à être interrogé avec move() et species().
     */
    GameData();

    /**
     * Recherche une attaque dans le catalogue par son nom.
     *
     * Entrées:
     *   name (const std::string&): Nom exact utilisé comme clé dans le catalogue.
     *
     * Sortie:
     *   const MoveData&: Référence vers la définition de l'attaque trouvée.
     *
     * Erreurs:
     *   Lance std::out_of_range si aucun nom correspondant n'existe.
     */
    const MoveData& move(const std::string& name) const;

    /**
     * Recherche une espèce dans le catalogue par son nom.
     *
     * Entrées:
     *   name (const std::string&): Nom exact utilisé comme clé dans le catalogue.
     *
     * Sortie:
     *   const PokemonSpecies&: Référence vers la définition de l'espèce trouvée.
     *
     * Erreurs:
     *   Lance std::out_of_range si aucun nom correspondant n'existe.
     */
    const PokemonSpecies& species(const std::string& name) const;

private:
    std::map<std::string, MoveData> moves_; ///< Attaques indexées par leur nom.
    std::map<std::string, PokemonSpecies> species_; ///< Espèces indexées par leur nom.
};

} // namespace pokemon
