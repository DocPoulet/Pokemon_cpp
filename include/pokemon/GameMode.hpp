#pragma once

#include <string_view>

namespace pokemon {

/**
 * Définit qui contrôle chacun des deux dresseurs pendant une partie.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   GameMode: mode utilisé par GameApp et ConsoleUI pour choisir humain ou IA.
 */
enum class GameMode {
    PlayerVsAI,
    PlayerVsPlayer,
    AIvsAI
};

/** Convertit un mode de jeu en libellé lisible. Entrée : GameMode. Sortie : nom du mode. */
std::string_view toString(GameMode mode);

} // namespace pokemon
