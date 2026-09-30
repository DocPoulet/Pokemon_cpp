#include "pokemon/GameMode.hpp"

namespace pokemon {
std::string_view toString(GameMode mode) {
    switch (mode) {
        case GameMode::PlayerVsAI: return "Joueur vs IA";
        case GameMode::PlayerVsPlayer: return "Joueur vs Joueur";
        case GameMode::AIvsAI: return "IA vs IA";
    }
    return "Inconnu";
}
} // namespace pokemon
