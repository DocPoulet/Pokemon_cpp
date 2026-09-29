#pragma once

#include <cstddef>
#include <string_view>

namespace pokemon {

enum class Stat : std::size_t {
    HP = 0,
    Attack,
    Defense,
    SpecialAttack,
    SpecialDefense,
    Speed,
    Count
};

enum class AccuracyStat : std::size_t {
    Accuracy = 0,
    Evasion,
    Count
};

enum class Type : std::size_t {
    Steel = 0,
    Fighting,
    Dragon,
    Water,
    Electric,
    Fairy,
    Fire,
    Ice,
    Bug,
    Normal,
    Grass,
    Poison,
    Psychic,
    Rock,
    Ground,
    Ghost,
    Dark,
    Flying,
    Neutral,
    Count
};

enum class MoveCategory {
    Status = 0,
    Physical,
    Special
};

enum class Weather {
    None,
    Sun,
    Rain,
    Sandstorm,
    Snow
};

enum class Terrain {
    None,
    Electric,
    Misty,
    Grassy,
    Psychic
};

enum class Target {
    Self,
    Opponent
};

std::string_view toString(Stat stat);
std::string_view toString(Type type);
std::string_view toString(MoveCategory category);
std::string_view toString(Weather weather);
std::string_view toString(Terrain terrain);

} // namespace pokemon
