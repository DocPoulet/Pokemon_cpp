#pragma once

#include "pokemon/Move.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/PokemonSpecies.hpp"

#include <array>
#include <optional>
#include <string>

namespace pokemon {

class Pokemon {
public:
    Pokemon(const PokemonSpecies* species,
            int level = 50,
            Nature nature = Nature{});

    const PokemonSpecies& species() const;
    const std::string& name() const;
    int level() const;

    int currentHP() const;
    int maxHP() const;
    bool fainted() const;
    void setHP(int hp);
    void damage(int amount);
    void heal(int amount);

    int stat(Stat stat) const;
    int iv(Stat stat) const;
    int ev(Stat stat) const;
    void setIV(Stat stat, int value);
    bool setEV(Stat stat, int value);
    int totalEV() const;

    const Nature& nature() const;
    void setNature(Nature nature);

    bool setMove(std::size_t slot, const MoveData* move);
    void clearMove(std::size_t slot);
    std::array<std::optional<MoveInstance>, 4>& moves();
    const std::array<std::optional<MoveInstance>, 4>& moves() const;
    bool hasUsableMove() const;

private:
    const PokemonSpecies* species_ = nullptr;
    int level_ = 1;
    int currentHP_ = 0;
    std::array<int, static_cast<std::size_t>(Stat::Count)> ivs_{};
    std::array<int, static_cast<std::size_t>(Stat::Count)> evs_{};
    Nature nature_;
    std::array<std::optional<MoveInstance>, 4> moves_{};
};

} // namespace pokemon
