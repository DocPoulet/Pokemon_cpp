#pragma once

#include "pokemon/Battle.hpp"

namespace pokemon {

class BattleController {
public:
    virtual ~BattleController() = default;
    virtual BattleAction chooseAction(const Battle& battle, int player) = 0;
};

class GreedyAI final : public BattleController {
public:
    BattleAction chooseAction(const Battle& battle, int player) override;
};

} // namespace pokemon
