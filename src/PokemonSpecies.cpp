#include "pokemon/PokemonSpecies.hpp"

#include <utility>

namespace pokemon {

PokemonSpecies::PokemonSpecies(
    std::string name,
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
    std::vector<Type> types)
    : name_(std::move(name)), baseStats_(baseStats), types_(std::move(types)) {}

const std::string& PokemonSpecies::name() const { return name_; }

int PokemonSpecies::baseStat(Stat stat) const {
    return baseStats_.at(static_cast<std::size_t>(stat));
}

const std::vector<Type>& PokemonSpecies::types() const { return types_; }

} // namespace pokemon
