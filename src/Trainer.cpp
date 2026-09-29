#include "pokemon/Trainer.hpp"

#include <utility>

namespace pokemon {

Trainer::Trainer(std::string name) : name_(std::move(name)) {}
const std::string& Trainer::name() const { return name_; }

bool Trainer::addPokemon(const Pokemon& pokemon) {
    if (team_.size() >= 6) return false;
    team_.push_back(pokemon);
    return true;
}

std::vector<Pokemon>& Trainer::team() { return team_; }
const std::vector<Pokemon>& Trainer::team() const { return team_; }

bool Trainer::allFainted() const {
    if (team_.empty()) return true;
    for (const auto& pokemon : team_) {
        if (!pokemon.fainted()) return false;
    }
    return true;
}

} // namespace pokemon
