#include "pokemon/PokemonSpecies.hpp"

#include <algorithm>
#include <utility>

namespace pokemon {

PokemonSpecies::PokemonSpecies(
    std::string name,
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
    std::vector<Type> types,
    std::vector<std::string> movePool,
    std::vector<Ability> abilities)
    : name_(std::move(name)),
      baseStats_(baseStats),
      types_(std::move(types)),
      movePool_(std::move(movePool)),
      abilities_(std::move(abilities)) {}

const std::string& PokemonSpecies::name() const { return name_; }

int PokemonSpecies::baseStat(Stat stat) const {
    return baseStats_.at(static_cast<std::size_t>(stat));
}

const std::vector<Type>& PokemonSpecies::types() const { return types_; }
const std::vector<std::string>& PokemonSpecies::movePool() const { return movePool_; }
const std::vector<Ability>& PokemonSpecies::abilities() const { return abilities_; }

bool PokemonSpecies::canLearnMove(const std::string& moveName) const {
    return std::find(movePool_.begin(), movePool_.end(), moveName) != movePool_.end();
}

bool PokemonSpecies::canHaveAbility(Ability ability) const {
    return std::find(abilities_.begin(), abilities_.end(), ability) != abilities_.end();
}

} // namespace pokemon
