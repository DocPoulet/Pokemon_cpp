#include "pokemon/GameApp.hpp"

#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"
#include "pokemon/ConsoleUI.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/TeamIO.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace pokemon {

GameApp::GameApp(std::istream& input, std::ostream& output)
    : input_(input), output_(output), data_(), battlePokemon_(data_) {}

int GameApp::readInt() {
    int value = 0;
    while (!(input_ >> value)) {
        input_.clear();
        input_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        output_ << "Entree invalide. Reessayez: ";
    }
    return value;
}

std::string GameApp::readWord() {
    std::string value;
    input_ >> value;
    return value;
}

Trainer GameApp::makeQuickTeam(const std::string& name, int variant) const {
    Trainer trainer(name);
    if (variant % 2 == 0) {
        trainer.addPokemon(battlePokemon_.build("quick_salameche"));
        trainer.addPokemon(battlePokemon_.build("quick_carapuce"));
    } else {
        trainer.addPokemon(battlePokemon_.build("quick_bulbizarre"));
        trainer.addPokemon(battlePokemon_.build("quick_pikachu"));
    }
    return trainer;
}

Trainer GameApp::buildTeamInteractive(const std::string& name) {
    Trainer trainer(name);
    TeamBuilder builder(data_);
    const auto speciesNames = data_.speciesNames();

    output_ << "Nombre de Pokemon dans l'equipe (1-6): ";
    const int count = std::clamp(readInt(), 1, 6);

    for (int n = 0; n < count; ++n) {
        output_ << "\nPokemon " << (n + 1) << ":\n";
        for (std::size_t i = 0; i < speciesNames.size(); ++i) output_ << (i + 1) << ". " << speciesNames[i] << '\n';
        output_ << "Espece: ";
        int speciesChoice = readInt();
        while (speciesChoice < 1 || speciesChoice > static_cast<int>(speciesNames.size())) {
            output_ << "Choix invalide: ";
            speciesChoice = readInt();
        }

        PokemonConfig config;
        config.species = speciesNames[static_cast<std::size_t>(speciesChoice - 1)];
        config.form = "Base";
        config.ivs.fill(31);
        const PokemonSpecies& species = data_.species(config.species);

        output_ << "Surnom (- pour aucun): ";
        config.nickname = readWord();
        if (config.nickname == "-") config.nickname.clear();

        output_ << "Niveau (1-100): ";
        config.level = std::clamp(readInt(), 1, 100);

        const auto& movePool = species.movePool();
        std::set<std::string> selected;
        output_ << "Choisissez entre 1 et 4 attaques du movepool. 0 termine la selection.\n";
        for (std::size_t slot = 0; slot < 4; ++slot) {
            for (std::size_t i = 0; i < movePool.size(); ++i) {
                output_ << (i + 1) << ". " << movePool[i] << ((i + 1) % 4 == 0 ? "\n" : " | ");
            }
            output_ << "\nSlot " << (slot + 1) << ": ";
            int choice = readInt();
            if (choice == 0 && slot > 0) break;
            while (choice < 1 || choice > static_cast<int>(movePool.size()) ||
                   selected.count(movePool[static_cast<std::size_t>(choice - 1)]) != 0) {
                output_ << "Choix invalide ou attaque deja choisie: ";
                choice = readInt();
            }
            config.moves[slot] = movePool[static_cast<std::size_t>(choice - 1)];
            selected.insert(config.moves[slot]);
        }

        output_ << "Objet: 0 Aucun, 1 Restes, 2 Orbe Vie, 3 Orbe Rouge, 4 Orbe Bleue: ";
        config.heldItem = static_cast<HeldItem>(std::clamp(readInt(), 0, 4));

        const auto& abilities = species.abilities();
        if (abilities.empty()) {
            output_ << "Aucun talent disponible pour cette espece de secours.\n";
            config.ability = Ability::None;
        } else {
            output_ << "Talents possibles:\n";
            for (std::size_t i = 0; i < abilities.size(); ++i) output_ << (i + 1) << ". " << toString(abilities[i]) << '\n';
            output_ << "Talent: ";
            int abilityChoice = readInt();
            while (abilityChoice < 1 || abilityChoice > static_cast<int>(abilities.size())) {
                output_ << "Choix invalide: ";
                abilityChoice = readInt();
            }
            config.ability = abilities[static_cast<std::size_t>(abilityChoice - 1)];
        }

        try {
            builder.addPokemon(trainer, config);
        } catch (const std::exception& error) {
            output_ << "Configuration refusee: " << error.what() << "\n";
            --n;
        }
    }
    return trainer;
}

void GameApp::runBattle(GameMode mode, Trainer player1, Trainer player2) {
    if (player1.team().empty() || player2.team().empty()) {
        output_ << "Impossible de lancer un combat avec une equipe vide.\n";
        return;
    }

    Battle battle(player1, player2);
    TacticalAI ai1;
    TacticalAI ai2;
    ConsoleUI ui(input_, output_);

    switch (mode) {
        case GameMode::PlayerVsAI: ui.run(battle, nullptr, &ai2); break;
        case GameMode::PlayerVsPlayer: ui.run(battle, nullptr, nullptr); break;
        case GameMode::AIvsAI: ui.run(battle, &ai1, &ai2); break;
    }
}

int GameApp::run() {
    while (true) {
        output_ << "\n=== Pokemon_cpp v0.7 ===\n"
                << "1. Combat rapide Joueur vs IA\n"
                << "2. Team Builder puis Joueur vs IA\n"
                << "3. Charger une equipe puis Joueur vs IA\n"
                << "4. Combat rapide Joueur vs Joueur\n"
                << "5. Demonstration IA vs IA\n"
                << "0. Quitter\n> ";
        const int choice = readInt();
        if (choice == 0) return 0;

        if (choice == 1) {
            runBattle(GameMode::PlayerVsAI, makeQuickTeam("Joueur", 0), makeQuickTeam("IA", 1));
        } else if (choice == 2) {
            Trainer team = buildTeamInteractive("Joueur");
            output_ << "Sauvegarder cette equipe ? (1 oui / 0 non): ";
            if (readInt() == 1) {
                output_ << "Nom du fichier (ex: mon_equipe.team): ";
                const std::string path = readWord();
                output_ << (TeamIO::saveFile(team, path) ? "Equipe sauvegardee.\n" : "Echec de sauvegarde.\n");
            }
            runBattle(GameMode::PlayerVsAI, team, makeQuickTeam("IA", 1));
        } else if (choice == 3) {
            output_ << "Fichier d'equipe: ";
            const std::string path = readWord();
            try {
                runBattle(GameMode::PlayerVsAI, TeamIO::loadFile(path, data_), makeQuickTeam("IA", 1));
            } catch (const std::exception& error) {
                output_ << "Chargement impossible: " << error.what() << '\n';
            }
        } else if (choice == 4) {
            runBattle(GameMode::PlayerVsPlayer, makeQuickTeam("Joueur 1", 0), makeQuickTeam("Joueur 2", 1));
        } else if (choice == 5) {
            runBattle(GameMode::AIvsAI, makeQuickTeam("IA Rouge", 0), makeQuickTeam("IA Bleue", 1));
        } else {
            output_ << "Choix invalide.\n";
        }
    }
}

} // namespace pokemon
