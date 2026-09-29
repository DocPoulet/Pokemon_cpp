#pragma once

#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"

#include <iosfwd>

namespace pokemon {

class ConsoleUI {
public:
    ConsoleUI(std::istream& input, std::ostream& output);

    void run(Battle& battle, BattleController& opponentAI);

private:
    std::istream& input_;
    std::ostream& output_;

    BattleAction chooseHumanAction(Battle& battle);
    BattleAction chooseMove(Battle& battle);
    BattleAction chooseSwitch(Battle& battle);
    void printTeam(const Trainer& trainer) const;
    void printEvents(const std::vector<BattleEvent>& events) const;
    int readInt();
};

} // namespace pokemon
