#include "pokemon/Pokemon.hpp"

#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <utility>

namespace pokemon {

Pokemon::Pokemon(const PokemonSpecies* species, int level, Nature nature)
    : species_(species), level_(std::clamp(level, 1, 100)), nature_(std::move(nature)) {
    if (!species_) throw std::invalid_argument("PokemonSpecies ne peut pas etre null");
    ivs_.fill(0);
    evs_.fill(0);
    currentHP_ = maxHP();
}

const PokemonSpecies& Pokemon::species() const { return *species_; }
const std::string& Pokemon::name() const { return species_->name(); }
int Pokemon::level() const { return level_; }
int Pokemon::currentHP() const { return currentHP_; }
int Pokemon::maxHP() const { return stat(Stat::HP); }
bool Pokemon::fainted() const { return currentHP_ <= 0; }

void Pokemon::setHP(int hp) {
    currentHP_ = std::clamp(hp, 0, maxHP());
}

void Pokemon::damage(int amount) {
    if (amount <= 0) return;
    setHP(currentHP_ - amount);
}

void Pokemon::heal(int amount) {
    if (amount <= 0) return;
    setHP(currentHP_ + amount);
}

int Pokemon::stat(Stat statValue) const {
    const auto i = static_cast<std::size_t>(statValue);
    const int base = species_->baseStat(statValue);
    const int iv = ivs_[i];
    const int ev = evs_[i];

    if (statValue == Stat::HP) {
        if (name() == "Munja") return 1;
        return (((2 * base + iv + ev / 4) * level_) / 100) + level_ + 10;
    }

    const int raw = (((2 * base + iv + ev / 4) * level_) / 100) + 5;
    return static_cast<int>(raw * nature_.multiplier(statValue));
}

int Pokemon::iv(Stat statValue) const {
    return ivs_.at(static_cast<std::size_t>(statValue));
}

int Pokemon::ev(Stat statValue) const {
    return evs_.at(static_cast<std::size_t>(statValue));
}

void Pokemon::setIV(Stat statValue, int value) {
    ivs_.at(static_cast<std::size_t>(statValue)) = std::clamp(value, 0, 31);
    setHP(currentHP_);
}

bool Pokemon::setEV(Stat statValue, int value) {
    value = std::clamp(value, 0, 252);
    auto& current = evs_.at(static_cast<std::size_t>(statValue));

    if (value <= current) {
        current = value;
        setHP(currentHP_);
        return true;
    }

    const int remaining = 510 - totalEV();
    if (remaining <= 0) return false;

    const int requested = value - current;
    const int addable = std::min(requested, remaining);
    current += addable;
    setHP(currentHP_);
    return current == value;
}

int Pokemon::totalEV() const {
    return std::accumulate(evs_.begin(), evs_.end(), 0);
}

const Nature& Pokemon::nature() const { return nature_; }

void Pokemon::setNature(Nature nature) {
    nature_ = std::move(nature);
    setHP(currentHP_);
}

bool Pokemon::setMove(std::size_t slot, const MoveData* move) {
    if (slot >= moves_.size() || !move) return false;
    moves_[slot] = MoveInstance(move);
    return true;
}

void Pokemon::clearMove(std::size_t slot) {
    if (slot < moves_.size()) moves_[slot].reset();
}

std::array<std::optional<MoveInstance>, 4>& Pokemon::moves() { return moves_; }
const std::array<std::optional<MoveInstance>, 4>& Pokemon::moves() const { return moves_; }

bool Pokemon::hasUsableMove() const {
    for (const auto& move : moves_) {
        if (move && move->usable()) return true;
    }
    return false;
}

StatusCondition Pokemon::status() const { return status_; }

bool Pokemon::setStatus(StatusCondition statusValue, int turns) {
    if (statusValue == StatusCondition::None) {
        cureStatus();
        return true;
    }
    if (status_ != StatusCondition::None) return false;
    status_ = statusValue;
    statusTurns_ = std::max(0, turns);
    return true;
}

void Pokemon::cureStatus() {
    status_ = StatusCondition::None;
    statusTurns_ = 0;
}

int Pokemon::statusTurns() const { return statusTurns_; }

void Pokemon::setStatusTurns(int turns) {
    statusTurns_ = std::max(0, turns);
}

HeldItem Pokemon::heldItem() const { return heldItem_; }

void Pokemon::setHeldItem(HeldItem item) { heldItem_ = item; }

Ability Pokemon::ability() const { return ability_; }

void Pokemon::setAbility(Ability abilityValue) { ability_ = abilityValue; }

} // namespace pokemon
