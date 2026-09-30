#include "pokemon/ConsoleUI.hpp"

#include <iostream>
#include <limits>

namespace pokemon {

ConsoleUI::ConsoleUI(std::istream& input, std::ostream& output)
    : input_(input), output_(output) {}

int ConsoleUI::readInt() {
    int value = 0;
    while (!(input_ >> value)) {
        input_.clear();
        input_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        output_ << "Entree invalide. Reessayez: ";
    }
    return value;
}

void ConsoleUI::printTeam(const Trainer& trainer) const {
    output_ << "Equipe de " << trainer.name() << ":\n";
    for (std::size_t i = 0; i < trainer.team().size(); ++i) {
        const auto& pokemon = trainer.team()[i];
        output_ << (i + 1) << ". " << pokemon.name()
                << " - " << pokemon.currentHP() << "/" << pokemon.maxHP() << " PV";
        if (pokemon.fainted()) output_ << " [KO]";
        else if (pokemon.status() != StatusCondition::None) {
            output_ << " [" << toString(pokemon.status()) << "]";
        }
        output_ << '\n';
    }
}

void ConsoleUI::printEvents(const std::vector<BattleEvent>& events) const {
    for (const auto& event : events) {
        output_ << event.text << '\n';
    }
}

BattleAction ConsoleUI::chooseMove(Battle& battle, int playerIndex) {
    Pokemon* pokemon = battle.active(playerIndex);
    if (!pokemon) return {ActionType::Move, 0};

    if (!pokemon->hasUsableMove()) {
        output_ << pokemon->name() << " n'a plus de PP : Lutte sera utilisee.\n";
        return {ActionType::Move, 0};
    }

    while (true) {
        output_ << "Attaques de " << pokemon->name() << ":\n";
        for (std::size_t i = 0; i < pokemon->moves().size(); ++i) {
            output_ << (i + 1) << ". ";
            const auto& instance = pokemon->moves()[i];
            if (!instance || !instance->data()) {
                output_ << "[vide]\n";
                continue;
            }
            output_ << instance->data()->name << " - PP "
                    << instance->currentPP() << "/" << instance->maxPP() << '\n';
        }
        output_ << "> ";
        const int choice = readInt();
        if (choice < 1 || choice > 4) {
            output_ << "Choix invalide.\n";
            continue;
        }
        const auto& selected = pokemon->moves()[static_cast<std::size_t>(choice - 1)];
        if (!selected || !selected->usable()) {
            output_ << "Cette attaque n'est pas utilisable.\n";
            continue;
        }
        return {ActionType::Move, static_cast<std::size_t>(choice - 1)};
    }
}

BattleAction ConsoleUI::chooseSwitch(Battle& battle, int playerIndex) {
    while (true) {
        printTeam(battle.trainer(playerIndex));
        output_ << "Choisissez un Pokemon: ";
        const int choice = readInt();
        if (choice < 1 || choice > static_cast<int>(battle.trainer(playerIndex).team().size())) {
            output_ << "Choix invalide.\n";
            continue;
        }
        const std::size_t index = static_cast<std::size_t>(choice - 1);
        if (battle.trainer(playerIndex).team()[index].fainted() || index == battle.activeIndex(playerIndex)) {
            output_ << "Ce changement est impossible.\n";
            continue;
        }
        return {ActionType::Switch, index};
    }
}

BattleAction ConsoleUI::chooseHumanAction(Battle& battle, int playerIndex) {
    while (true) {
        const Pokemon* player = battle.active(playerIndex);
        const Pokemon* opponent = battle.active(1 - playerIndex);
        output_ << "\n==========================\n";
        if (player && opponent) {
            output_ << player->name() << " " << player->currentHP() << "/" << player->maxHP() << " PV";
            if (player->status() != StatusCondition::None) output_ << " [" << toString(player->status()) << "]";
            output_ << "  //  " << opponent->name() << " " << opponent->currentHP() << "/"
                    << opponent->maxHP() << " PV";
            if (opponent->status() != StatusCondition::None) output_ << " [" << toString(opponent->status()) << "]";
            output_ << '\n';
        }
        output_ << "1. Attaquer\n2. Equipe\n3. Sac\n4. Fuite\n> ";
        const int choice = readInt();
        switch (choice) {
            case 1: return chooseMove(battle, playerIndex);
            case 2: return chooseSwitch(battle, playerIndex);
            case 3:
                output_ << "Le sac n'est pas encore implemente.\n";
                break;
            case 4: return {ActionType::Run, 0};
            default:
                output_ << "Choix invalide.\n";
                break;
        }
    }
}

void ConsoleUI::run(Battle& battle, BattleController& opponentAI) {
    run(battle, nullptr, &opponentAI);
}

void ConsoleUI::run(Battle& battle, BattleController* player1Controller,
                    BattleController* player2Controller) {
    printTeam(battle.trainer(0));
    output_ << '\n';
    printTeam(battle.trainer(1));
    output_ << '\n';

    bool escaped = false;
    std::array<BattleController*, 2> controllers{player1Controller, player2Controller};

    while (!battle.finished() && !escaped) {
        for (int player = 0; player < 2; ++player) {
            if (!battle.active(player) || !battle.active(player)->fainted()) continue;
            BattleAction forced = controllers[static_cast<std::size_t>(player)]
                ? controllers[static_cast<std::size_t>(player)]->chooseAction(battle, player)
                : chooseSwitch(battle, player);
            if (forced.type == ActionType::Switch) {
                std::vector<BattleEvent> events;
                battle.switchPokemon(player, forced.index, &events);
                printEvents(events);
            }
        }

        if (battle.finished()) break;

        std::array<BattleAction, 2> actions{};
        for (int player = 0; player < 2; ++player) {
            auto* controller = controllers[static_cast<std::size_t>(player)];
            actions[static_cast<std::size_t>(player)] = controller
                ? controller->chooseAction(battle, player)
                : chooseHumanAction(battle, player);
            if (actions[static_cast<std::size_t>(player)].type == ActionType::Run) {
                output_ << battle.trainer(player).name() << " prend la fuite.\n";
                escaped = true;
                break;
            }
        }
        if (escaped) break;

        const auto events = battle.resolveTurn(actions[0], actions[1]);
        printEvents(events);
    }

}

} // namespace pokemon
