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
        output_ << '\n';
    }
}

void ConsoleUI::printEvents(const std::vector<BattleEvent>& events) const {
    for (const auto& event : events) {
        output_ << event.text << '\n';
    }
}

BattleAction ConsoleUI::chooseMove(Battle& battle) {
    Pokemon* pokemon = battle.active(0);
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

BattleAction ConsoleUI::chooseSwitch(Battle& battle) {
    while (true) {
        printTeam(battle.trainer(0));
        output_ << "Choisissez un Pokemon: ";
        const int choice = readInt();
        if (choice < 1 || choice > static_cast<int>(battle.trainer(0).team().size())) {
            output_ << "Choix invalide.\n";
            continue;
        }
        const std::size_t index = static_cast<std::size_t>(choice - 1);
        if (battle.trainer(0).team()[index].fainted() || index == battle.activeIndex(0)) {
            output_ << "Ce changement est impossible.\n";
            continue;
        }
        return {ActionType::Switch, index};
    }
}

BattleAction ConsoleUI::chooseHumanAction(Battle& battle) {
    while (true) {
        const Pokemon* player = battle.active(0);
        const Pokemon* opponent = battle.active(1);
        output_ << "\n==========================\n";
        if (player && opponent) {
            output_ << player->name() << " " << player->currentHP() << "/" << player->maxHP()
                    << " PV  //  " << opponent->name() << " "
                    << opponent->currentHP() << "/" << opponent->maxHP() << " PV\n";
        }
        output_ << "1. Attaquer\n2. Equipe\n3. Sac\n4. Fuite\n> ";
        const int choice = readInt();
        switch (choice) {
            case 1: return chooseMove(battle);
            case 2: return chooseSwitch(battle);
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
    printTeam(battle.trainer(0));
    output_ << '\n';

    bool escaped = false;
    while (!battle.finished() && !escaped) {
        if (battle.active(0) && battle.active(0)->fainted()) {
            const auto action = chooseSwitch(battle);
            std::vector<BattleEvent> events;
            battle.switchPokemon(0, action.index, &events);
            printEvents(events);
        }

        if (battle.active(1) && battle.active(1)->fainted()) {
            const BattleAction action = opponentAI.chooseAction(battle, 1);
            if (action.type == ActionType::Switch) {
                std::vector<BattleEvent> events;
                battle.switchPokemon(1, action.index, &events);
                printEvents(events);
            }
        }

        if (battle.finished()) break;

        const BattleAction human = chooseHumanAction(battle);
        if (human.type == ActionType::Run) {
            output_ << "Vous prenez la fuite.\n";
            escaped = true;
            break;
        }

        const BattleAction ai = opponentAI.chooseAction(battle, 1);
        const auto events = battle.resolveTurn(human, ai);
        printEvents(events);
    }

    if (!escaped && battle.finished()) {
        const int winner = battle.winner();
        if (winner >= 0) {
            output_ << battle.trainer(winner).name() << " a gagne le combat !\n";
        }
    }
}

} // namespace pokemon
