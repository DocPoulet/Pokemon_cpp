#pragma once

#include "pokemon/Types.hpp"

#include <array>
#include <string>
#include <vector>

namespace pokemon {

class PokemonSpecies {
public:
    PokemonSpecies(std::string name,
                   std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
                   std::vector<Type> types);

    const std::string& name() const;
    int baseStat(Stat stat) const;
    const std::vector<Type>& types() const;

private:
    std::string name_;
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats_{};
    std::vector<Type> types_;
};

} // namespace pokemon
