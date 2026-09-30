#include "pokemon/TeamBuilder.hpp"

#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>

namespace pokemon {

TeamBuilder::TeamBuilder(const GameData& data) : data_(data) {}

std::string TeamBuilder::validate(const PokemonConfig& config) const {
    if (config.species.empty()) return "L'espece ne peut pas etre vide.";
    if (!data_.hasSpecies(config.species)) return "Espece inconnue: " + config.species;
    if (config.form.empty()) return "La forme ne peut pas etre vide. Utilisez Base tant que les formes ne sont pas implementees.";
    if (config.level < 1 || config.level > 100) return "Le niveau doit etre compris entre 1 et 100.";

    const PokemonSpecies& species = data_.species(config.species);

    int totalEV = 0;
    for (std::size_t i = 0; i < config.ivs.size(); ++i) {
        if (config.ivs[i] < 0 || config.ivs[i] > 31) return "Un IV doit etre compris entre 0 et 31.";
        if (config.evs[i] < 0 || config.evs[i] > 252) return "Un EV doit etre compris entre 0 et 252.";
        totalEV += config.evs[i];
    }
    if (totalEV > 510) return "Le total des EV ne peut pas depasser 510.";

    std::set<std::string> selectedMoves;
    int moveCount = 0;
    for (const auto& move : config.moves) {
        if (move.empty()) continue;
        ++moveCount;
        if (!data_.hasMove(move)) return "Attaque inconnue: " + move;
        if (!species.canLearnMove(move)) return config.species + " ne peut pas apprendre " + move + ".";
        if (!selectedMoves.insert(move).second) return "Une attaque ne peut pas etre selectionnee deux fois: " + move;
    }
    if (moveCount == 0) return "Un Pokemon pret au combat doit posseder au moins une attaque.";

    const int item = static_cast<int>(config.heldItem);
    if (item < static_cast<int>(HeldItem::None) || item > static_cast<int>(HeldItem::BlueOrb)) {
        return "Objet tenu invalide.";
    }

    if (config.ability == Ability::None) return "Le talent d'un Pokemon pret au combat ne peut pas etre nul.";
    if (!species.canHaveAbility(config.ability)) {
        return std::string("Talent non autorise pour ") + config.species + ": " + std::string(toString(config.ability));
    }
    return {};
}

Pokemon TeamBuilder::buildPokemon(const PokemonConfig& config) const {
    const std::string error = validate(config);
    if (!error.empty()) {
        std::cerr << "[Data] Pokemon invalide: " << error
                  << ". Remplacement par MissingNo.\n";
        Pokemon missing(&data_.species("MissingNo."), 100, Nature("Missing"), "", "Base");
        missing.setAbility(Ability::None);
        missing.setHeldItem(HeldItem::None);

        auto names = data_.moveNames();
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(names.begin(), names.end(), rng);
        const std::size_t count = std::min<std::size_t>(4, names.size());
        for (std::size_t i = 0; i < count; ++i) missing.setMove(i, &data_.move(names[i]));
        missing.setHP(missing.maxHP());
        return missing;
    }

    Pokemon pokemon(&data_.species(config.species), config.level, config.nature,
                    config.nickname, config.form);
    for (std::size_t i = 0; i < config.ivs.size(); ++i) {
        const auto stat = static_cast<Stat>(i);
        pokemon.setIV(stat, config.ivs[i]);
        pokemon.setEV(stat, config.evs[i]);
    }
    for (std::size_t i = 0; i < config.moves.size(); ++i) {
        if (!config.moves[i].empty()) pokemon.setMove(i, &data_.move(config.moves[i]));
    }
    pokemon.setHeldItem(config.heldItem);
    pokemon.setAbility(config.ability);
    pokemon.setHP(pokemon.maxHP());
    return pokemon;
}

bool TeamBuilder::addPokemon(Trainer& trainer, const PokemonConfig& config) const {
    return trainer.addPokemon(buildPokemon(config));
}

} // namespace pokemon
