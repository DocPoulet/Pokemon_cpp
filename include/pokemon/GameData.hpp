#pragma once

#include "pokemon/Catalog.hpp"
#include "pokemon/Move.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <map>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Centralise les catalogues globaux du jeu.
 *
 * Entrées:
 *   moves.json: Définitions uniques de toutes les attaques.
 *   abilities.json: Définitions uniques de tous les talents.
 *   items.json: Définitions uniques de tous les objets tenus.
 *   pokemon_species.json: Espèces qui ne stockent que des références vers ces catalogues.
 *
 * Sortie:
 *   GameData: Source unique des définitions partagées par le moteur.
 */
class GameData {
public:
    GameData();
    explicit GameData(const std::string& speciesFile);

    const MoveData& move(const std::string& id) const;
    const AbilityData& ability(const std::string& id) const;
    const ItemData& item(const std::string& id) const;
    const PokemonSpecies& species(const std::string& id) const;

    std::vector<std::string> moveNames() const;
    std::vector<std::string> abilityNames() const;
    std::vector<std::string> itemNames() const;
    std::vector<std::string> speciesNames() const;

    bool hasMove(const std::string& id) const;
    bool hasAbility(const std::string& id) const;
    bool hasItem(const std::string& id) const;
    bool hasSpecies(const std::string& id) const;

private:
    std::map<std::string, MoveData> moves_;
    std::map<std::string, AbilityData> abilities_;
    std::map<std::string, ItemData> items_;
    std::map<std::string, PokemonSpecies> species_;

    void loadMovesJson(const std::string& path);
    void loadAbilitiesJson(const std::string& path);
    void loadItemsJson(const std::string& path);
    void loadSpeciesJson(const std::string& path);
    void installFallbackCatalogs();
};

} // namespace pokemon
