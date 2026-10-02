#pragma once

#include "pokemon/BattlePokemonData.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/GameMode.hpp"
#include "pokemon/Trainer.hpp"

#include <iosfwd>
#include <string>

namespace pokemon {

/**
 * Orchestre les menus du jeu sans déplacer les règles de combat hors de Battle.
 *
 * Entrées:
 *   input / output: Flux de la console.
 *   GameData: catalogue global des espèces et attaques.
 *   BattlePokemonData: presets JSON de Pokémon prêts au combat.
 *
 * Sortie:
 *   GameApp: application capable de construire des équipes, importer/exporter des équipes Showdown et lancer un combat.
 */
class GameApp {
public:
    GameApp(std::istream& input, std::ostream& output);
    int run();

private:
    std::istream& input_;
    std::ostream& output_;
    GameData data_;
    BattlePokemonData battlePokemon_;

    int readInt();
    std::string readWord();
    Trainer makeQuickTeam(const std::string& name, int variant) const;
    Trainer buildTeamInteractive(const std::string& name);
    Trainer importShowdownTeam(const std::string& name);
    void runBattle(GameMode mode, Trainer player1, Trainer player2);
};

} // namespace pokemon
