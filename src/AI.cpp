#include "pokemon/AI.hpp"

#include "pokemon/DamageCalculator.hpp"

#include <algorithm>

namespace pokemon {

BattleAction GreedyAI::chooseAction(const Battle& battle, int player) {
    const Pokemon* self = battle.active(player);
    const Pokemon* opponent = battle.active(1 - player);
    if (!self || !opponent) return {ActionType::Move, 0};

    if (self->fainted()) {
        const auto& team = battle.trainer(player).team();
        for (std::size_t i = 0; i < team.size(); ++i) {
            if (!team[i].fainted()) return {ActionType::Switch, i};
        }
        return {ActionType::Move, 0};
    }

    if (!self->hasUsableMove()) return {ActionType::Move, 0};

    std::size_t bestSlot = 0;
    double bestScore = -1.0;

    for (std::size_t i = 0; i < self->moves().size(); ++i) {
        const auto& instance = self->moves()[i];
        if (!instance || !instance->usable() || !instance->data()) continue;

        const MoveData& move = *instance->data();
        double score = DamageCalculator::rawDamage(
            *self, *opponent, move, battle, player, 1 - player);

        if (move.category == MoveCategory::Status) score = 5.0;
        if (score > bestScore) {
            bestScore = score;
            bestSlot = i;
        }
    }

    return {ActionType::Move, bestSlot};
}

} // namespace pokemon
