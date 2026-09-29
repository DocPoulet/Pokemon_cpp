#pragma once

#include "pokemon/Move.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <map>
#include <string>

namespace pokemon {

class GameData {
public:
    GameData();

    const MoveData& move(const std::string& name) const;
    const PokemonSpecies& species(const std::string& name) const;

private:
    std::map<std::string, MoveData> moves_;
    std::map<std::string, PokemonSpecies> species_;
};

} // namespace pokemon
