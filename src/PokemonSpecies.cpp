#include "pokemon/PokemonSpecies.hpp"

#include <algorithm>
#include <utility>

namespace pokemon {

PokemonSpecies::PokemonSpecies(
    std::string name,
    std::array<int, static_cast<std::size_t>(Stat::Count)> baseStats,
    std::vector<Type> types,
    std::vector<const MoveData*> movePool,
    std::vector<const AbilityData*> abilities)
    : name_(std::move(name)), baseStats_(baseStats), types_(std::move(types)),
      movePool_(std::move(movePool)), abilities_(std::move(abilities)) {}

const std::string& PokemonSpecies::name() const { return name_; }
int PokemonSpecies::baseStat(Stat stat) const { return baseStats_.at(static_cast<std::size_t>(stat)); }
const std::vector<Type>& PokemonSpecies::types() const { return types_; }
const std::vector<const MoveData*>& PokemonSpecies::movePool() const { return movePool_; }
const std::vector<const AbilityData*>& PokemonSpecies::abilities() const { return abilities_; }

bool PokemonSpecies::canLearnMove(const MoveData* move) const {
    return move && std::find(movePool_.begin(), movePool_.end(), move) != movePool_.end();
}

bool PokemonSpecies::canLearnMove(std::string_view moveId) const {
    return std::any_of(movePool_.begin(), movePool_.end(), [&](const MoveData* move) {
        return move && move->name == moveId;
    });
}

bool PokemonSpecies::canHaveAbility(const AbilityData* ability) const {
    return ability && std::find(abilities_.begin(), abilities_.end(), ability) != abilities_.end();
}

bool PokemonSpecies::canHaveAbility(std::string_view abilityId) const {
    return std::any_of(abilities_.begin(), abilities_.end(), [&](const AbilityData* ability) {
        return ability && ability->id == abilityId;
    });
}

} // namespace pokemon
