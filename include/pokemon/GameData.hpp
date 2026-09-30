#pragma once

#include "pokemon/Move.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <map>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Centralise les données globales connues du jeu.
 *
 * Entrées:
 *   Les définitions d'attaques restent intégrées au moteur en v0.7.
 *   Les espèces sont chargées depuis data/pokemon_species.json.
 *
 * Sortie:
 *   GameData: catalogue permettant de retrouver attaques et espèces par leur nom.
 */
class GameData {
public:
    /**
     * Charge le catalogue standard du projet.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   GameData: attaques intégrées et espèces chargées depuis le fichier JSON standard.
     */
    GameData();

    /**
     * Charge le catalogue avec un fichier d'espèces explicite.
     *
     * Entrées:
     *   speciesFile (const std::string&): Chemin vers un pokemon_species.json compatible.
     *
     * Sortie:
     *   GameData: catalogue construit avec les données du fichier fourni.
     */
    explicit GameData(const std::string& speciesFile);

    const MoveData& move(const std::string& name) const;
    const PokemonSpecies& species(const std::string& name) const;
    std::vector<std::string> moveNames() const;
    std::vector<std::string> speciesNames() const;
    bool hasMove(const std::string& name) const;
    bool hasSpecies(const std::string& name) const;

private:
    std::map<std::string, MoveData> moves_;
    std::map<std::string, PokemonSpecies> species_;

    void loadMoves();
    void loadSpeciesJson(const std::string& path);
};

} // namespace pokemon
