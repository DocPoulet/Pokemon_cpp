#include "pokemon/Nature.hpp"

#include <utility>

namespace pokemon {

Nature::Nature(std::string name, Stat increased, Stat decreased)
    : name_(std::move(name)), increased_(increased), decreased_(decreased) {}

const std::string& Nature::name() const { return name_; }
Stat Nature::increased() const { return increased_; }
Stat Nature::decreased() const { return decreased_; }

double Nature::multiplier(Stat stat) const {
    if (stat == Stat::HP) return 1.0;
    if (increased_ == decreased_) return 1.0;
    if (stat == increased_) return 1.1;
    if (stat == decreased_) return 0.9;
    return 1.0;
}

} // namespace pokemon
