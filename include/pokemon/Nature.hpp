#pragma once

#include "pokemon/Types.hpp"

#include <string>

namespace pokemon {

class Nature {
public:
    Nature(std::string name = "Hardi",
           Stat increased = Stat::Attack,
           Stat decreased = Stat::Attack);

    const std::string& name() const;
    Stat increased() const;
    Stat decreased() const;
    double multiplier(Stat stat) const;

private:
    std::string name_;
    Stat increased_;
    Stat decreased_;
};

} // namespace pokemon
