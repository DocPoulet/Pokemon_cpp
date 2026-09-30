#include "pokemon/AI.hpp"

#include "pokemon/DamageCalculator.hpp"

#include <algorithm>
#include <limits>

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


namespace {
double bestMoveScore(const Pokemon& self, const Pokemon& opponent,
                     const Battle& battle, int player) {
    double best = -1.0;
    for (const auto& instance : self.moves()) {
        if (!instance || !instance->usable() || !instance->data()) continue;
        const MoveData& move = *instance->data();
        double score = DamageCalculator::rawDamage(self, opponent, move, battle, player, 1 - player);
        if (move.category == MoveCategory::Status) {
            score = opponent.status() == StatusCondition::None ? 18.0 : 3.0;
        }
        if (move.priority > 0) score += 2.0 * move.priority;
        if (move.drainPercent > 0 && self.currentHP() < self.maxHP()) score += 5.0;
        best = std::max(best, score);
    }
    return best;
}
}

BattleAction TacticalAI::chooseAction(const Battle& battle, int player) {
    const Pokemon* self = battle.active(player);
    const Pokemon* opponent = battle.active(1 - player);
    if (!self || !opponent) return {ActionType::Move, 0};

    const auto& team = battle.trainer(player).team();
    if (self->fainted()) {
        double bestReserve = -1.0;
        std::size_t bestIndex = battle.activeIndex(player);
        for (std::size_t i = 0; i < team.size(); ++i) {
            if (team[i].fainted() || i == battle.activeIndex(player)) continue;
            const double score = bestMoveScore(team[i], *opponent, battle, player);
            if (score > bestReserve) { bestReserve = score; bestIndex = i; }
        }
        return {ActionType::Switch, bestIndex};
    }

    if (!self->hasUsableMove()) return {ActionType::Move, 0};

    std::size_t bestSlot = 0;
    double currentBest = -1.0;
    for (std::size_t i = 0; i < self->moves().size(); ++i) {
        const auto& instance = self->moves()[i];
        if (!instance || !instance->usable() || !instance->data()) continue;
        const MoveData& move = *instance->data();
        double score = DamageCalculator::rawDamage(*self, *opponent, move, battle, player, 1 - player);
        if (move.category == MoveCategory::Status) {
            score = opponent->status() == StatusCondition::None ? 18.0 : 3.0;
        }
        if (move.priority > 0) score += 2.0 * move.priority;
        if (move.drainPercent > 0 && self->currentHP() < self->maxHP()) score += 5.0;
        if (score > currentBest) { currentBest = score; bestSlot = i; }
    }

    const bool vulnerable = self->currentHP() * 100 <= self->maxHP() * 40;
    if (vulnerable) {
        double bestReserve = currentBest;
        std::size_t bestIndex = battle.activeIndex(player);
        for (std::size_t i = 0; i < team.size(); ++i) {
            if (i == battle.activeIndex(player) || team[i].fainted() || !team[i].hasUsableMove()) continue;
            const double score = bestMoveScore(team[i], *opponent, battle, player);
            if (score > bestReserve * 1.35) {
                bestReserve = score;
                bestIndex = i;
            }
        }
        if (bestIndex != battle.activeIndex(player)) return {ActionType::Switch, bestIndex};
    }

    return {ActionType::Move, bestSlot};
}

} // namespace pokemon
