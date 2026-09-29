#pragma once

#include "pokemon/Pokemon.hpp"

#include <string>
#include <vector>

namespace pokemon {

class Trainer {
public:
    explicit Trainer(std::string name = "Dresseur");

    const std::string& name() const;
    bool addPokemon(const Pokemon& pokemon);
    std::vector<Pokemon>& team();
    const std::vector<Pokemon>& team() const;
    bool allFainted() const;

private:
    std::string name_;
    std::vector<Pokemon> team_;
};

} // namespace pokemon
