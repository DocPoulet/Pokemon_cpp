#pragma once

#include "pokemon/GameData.hpp"
#include "pokemon/TeamBuilder.hpp"

#include <map>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Charge une bibliothèque JSON de Pokémon déjà configurés pour le combat.
 *
 * Entrées:
 *   GameData: catalogue global utilisé pour valider espèces, movepools et talents.
 *   battle_pokemon.json: presets avec niveau, IV, EV, nature, attaques, talent,
 *                        objet, surnom et forme.
 *
 * Sortie:
 *   BattlePokemonData: catalogue de PokemonConfig réutilisables par le jeu ou les tests.
 */
class BattlePokemonData {
public:
    /** Charge data/battle_pokemon.json. Entrée: GameData. Sortie: catalogue validé. */
    explicit BattlePokemonData(const GameData& data);

    /** Charge un fichier JSON explicite. Entrées: GameData et chemin. Sortie: catalogue validé. */
    BattlePokemonData(const GameData& data, const std::string& path);

    /** Retourne un preset par identifiant. Entrée: id. Sortie: PokemonConfig. */
    const PokemonConfig& preset(const std::string& id) const;

    /** Construit directement un Pokémon depuis un preset. Entrée: id. Sortie: Pokemon indépendant. */
    Pokemon build(const std::string& id) const;

    /** Retourne tous les identifiants disponibles. Entrées: aucune. Sortie: liste ordonnée. */
    std::vector<std::string> ids() const;

    /** Vérifie l'existence d'un preset. Entrée: id. Sortie: bool. */
    bool has(const std::string& id) const;

private:
    const GameData& data_;
    std::map<std::string, PokemonConfig> presets_;

    void load(const std::string& path);
};

} // namespace pokemon
