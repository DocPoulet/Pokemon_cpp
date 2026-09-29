#pragma once

#include "pokemon/Trainer.hpp"
#include "pokemon/Types.hpp"

#include <array>
#include <cstddef>
#include <optional>
#include <random>
#include <string>
#include <vector>

namespace pokemon {

enum class ActionType {
    Move,
    Switch,
    Run
};

struct BattleAction {
    ActionType type = ActionType::Move;
    std::size_t index = 0;
};

enum class EventType {
    Text,
    MoveUsed,
    Damage,
    Heal,
    Fainted,
    Switched,
    WeatherChanged,
    TerrainChanged,
    StageChanged,
    BattleEnded
};

struct BattleEvent {
    EventType type = EventType::Text;
    std::string text;
    int player = -1;
    int value = 0;
};

struct StatStages {
    std::array<int, static_cast<std::size_t>(Stat::Count)> stats{};
    std::array<int, static_cast<std::size_t>(AccuracyStat::Count)> accuracy{};
};

class Battle {
public:
    Battle(Trainer& player1, Trainer& player2, unsigned int seed = std::random_device{}());

    Trainer& trainer(int player);
    const Trainer& trainer(int player) const;

    Pokemon* active(int player);
    const Pokemon* active(int player) const;
    std::size_t activeIndex(int player) const;
    bool switchPokemon(int player, std::size_t index, std::vector<BattleEvent>* events = nullptr);

    bool finished() const;
    int winner() const; // -1 aucun, 0 joueur 1, 1 joueur 2

    Weather weather() const;
    Terrain terrain() const;
    int weatherTurns() const;
    int terrainTurns() const;

    int stage(int player, Stat stat) const;
    int accuracyStage(int player, AccuracyStat stat) const;
    void changeStage(int player, Stat stat, int delta);
    void changeAccuracyStage(int player, AccuracyStat stat, int delta);
    double statMultiplier(int player, Stat stat) const;
    double accuracyMultiplier(int player, AccuracyStat stat) const;

    int randomInt(int minInclusive, int maxInclusive);
    bool rollPercent(int percent);

    const MoveData& struggleMove() const;

    std::vector<BattleEvent> resolveTurn(const BattleAction& action1,
                                         const BattleAction& action2);

private:
    std::array<Trainer*, 2> trainers_;
    std::array<std::size_t, 2> activeIndex_{0, 0};
    std::array<StatStages, 2> stages_{};
    Weather weather_ = Weather::None;
    int weatherTurns_ = 0;
    Terrain terrain_ = Terrain::None;
    int terrainTurns_ = 0;
    std::mt19937 rng_;

    void resetStages(int player);
    void setWeather(Weather weather, int turns, std::vector<BattleEvent>& events);
    void setTerrain(Terrain terrain, int turns, std::vector<BattleEvent>& events);
    void applyEndTurn(std::vector<BattleEvent>& events);
    void resolveMove(int attackerPlayer, std::size_t slot, std::vector<BattleEvent>& events);
    void applyMoveEffects(int attackerPlayer, const MoveData& move,
                          Pokemon& attacker, Pokemon& defender,
                          std::vector<BattleEvent>& events);
};

} // namespace pokemon
