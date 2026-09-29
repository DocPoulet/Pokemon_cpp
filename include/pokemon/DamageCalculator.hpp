#pragma once

#include "pokemon/Battle.hpp"

namespace pokemon {

struct DamageResult {
    int damage = 0;
    double effectiveness = 1.0;
    bool critical = false;
    bool hit = true;
};

class DamageCalculator {
public:
    static double effectiveness(Type attackType, const PokemonSpecies& defender,
                                Weather weather);
    static double rawDamage(const Pokemon& attacker, const Pokemon& defender,
                            const MoveData& move, const Battle& battle,
                            int attackerPlayer, int defenderPlayer);
    static DamageResult calculate(Pokemon& attacker, Pokemon& defender,
                                  const MoveData& move, Battle& battle,
                                  int attackerPlayer, int defenderPlayer);
};

} // namespace pokemon
