#include "pokemon/DamageCalculator.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

namespace pokemon {
namespace {
constexpr std::array<std::array<double, 18>, 18> chart{{
    {{0.5,2.0,0.5,1.0,1.0,0.5,2.0,0.5,0.5,0.5,0.5,0.0,0.5,0.5,2.0,1.0,1.0,0.5}},
    {{1.0,1.0,1.0,1.0,1.0,2.0,1.0,1.0,0.5,1.0,1.0,1.0,2.0,0.5,1.0,1.0,0.5,2.0}},
    {{1.0,1.0,2.0,0.5,0.5,2.0,0.5,2.0,1.0,1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,1.0}},
    {{0.5,1.0,1.0,0.5,2.0,1.0,0.5,0.5,1.0,1.0,2.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0}},
    {{0.5,1.0,1.0,1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,2.0,1.0,1.0,0.5}},
    {{2.0,0.5,0.0,1.0,1.0,1.0,1.0,1.0,0.5,1.0,1.0,2.0,1.0,1.0,1.0,1.0,0.5,1.0}},
    {{0.5,1.0,1.0,2.0,1.0,0.5,0.5,0.5,0.5,1.0,0.5,1.0,1.0,2.0,2.0,1.0,1.0,1.0}},
    {{2.0,2.0,1.0,1.0,1.0,1.0,2.0,0.5,1.0,1.0,1.0,1.0,1.0,2.0,1.0,1.0,1.0,1.0}},
    {{1.0,0.5,1.0,1.0,1.0,1.0,2.0,1.0,1.0,1.0,0.5,1.0,1.0,2.0,0.5,1.0,1.0,2.0}},
    {{1.0,2.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,0.0,1.0,1.0}},
    {{1.0,1.0,1.0,0.5,0.5,1.0,2.0,2.0,2.0,1.0,0.5,2.0,1.0,1.0,0.5,1.0,1.0,2.0}},
    {{1.0,0.5,1.0,1.0,1.0,0.5,1.0,1.0,0.5,1.0,0.5,0.5,2.0,1.0,2.0,1.0,1.0,1.0}},
    {{1.0,0.5,1.0,1.0,1.0,1.0,1.0,1.0,2.0,1.0,1.0,1.0,0.5,1.0,1.0,2.0,2.0,1.0}},
    {{2.0,2.0,1.0,2.0,1.0,1.0,0.5,1.0,1.0,0.5,2.0,0.5,1.0,1.0,2.0,1.0,1.0,0.5}},
    {{1.0,1.0,1.0,2.0,0.0,1.0,1.0,2.0,1.0,1.0,2.0,0.5,1.0,0.5,1.0,1.0,1.0,1.0}},
    {{1.0,0.0,1.0,1.0,1.0,1.0,1.0,1.0,0.5,0.0,1.0,0.5,1.0,1.0,1.0,2.0,2.0,1.0}},
    {{1.0,2.0,1.0,1.0,1.0,2.0,1.0,1.0,2.0,1.0,1.0,1.0,0.0,1.0,1.0,0.5,0.5,1.0}},
    {{1.0,0.5,1.0,1.0,2.0,1.0,1.0,2.0,0.5,1.0,0.5,1.0,1.0,2.0,0.0,1.0,1.0,1.0}}
}};

bool hasType(const Pokemon& pokemon, Type type) {
    for (const auto pokemonType : pokemon.species().types()) {
        if (pokemonType == type) return true;
    }
    return false;
}
}

double DamageCalculator::effectiveness(Type attackType,
                                       const PokemonSpecies& defender,
                                       Weather /*weather*/) {
    const auto attackIndex = static_cast<std::size_t>(attackType);
    if (attackIndex >= 18) return 1.0;

    double total = 1.0;
    for (const auto defenderType : defender.types()) {
        const auto defenderIndex = static_cast<std::size_t>(defenderType);
        if (defenderIndex >= 18) continue;
        total *= chart[defenderIndex][attackIndex];
    }
    return total;
}

double DamageCalculator::rawDamage(const Pokemon& attacker,
                                   const Pokemon& defender,
                                   const MoveData& move,
                                   const Battle& battle,
                                   int attackerPlayer,
                                   int defenderPlayer) {
    if (move.category == MoveCategory::Status || move.power <= 0) return 0.0;

    const double eff = effectiveness(move.type, defender.species(), battle.weather());
    if (eff == 0.0) return 0.0;

    const Stat attackStat = move.category == MoveCategory::Physical
        ? Stat::Attack : Stat::SpecialAttack;
    const Stat defenseStat = move.category == MoveCategory::Physical
        ? Stat::Defense : Stat::SpecialDefense;

    const double attack = std::max(1.0,
        attacker.stat(attackStat) * battle.statMultiplier(attackerPlayer, attackStat));
    const double defense = std::max(1.0,
        defender.stat(defenseStat) * battle.statMultiplier(defenderPlayer, defenseStat));

    double weather = 1.0;
    if (battle.weather() == Weather::Sun) {
        if (move.type == Type::Fire) weather *= 1.5;
        if (move.type == Type::Water) weather *= 0.5;
    } else if (battle.weather() == Weather::Rain) {
        if (move.type == Type::Water) weather *= 1.5;
        if (move.type == Type::Fire) weather *= 0.5;
    }

    const double stab = hasType(attacker, move.type) ? 1.5 : 1.0;
    return (((((attacker.level() * 0.4) + 2.0) * move.power * attack) / defense) / 50.0 + 2.0)
        * weather * stab * eff;
}

DamageResult DamageCalculator::calculate(Pokemon& attacker,
                                         Pokemon& defender,
                                         const MoveData& move,
                                         Battle& battle,
                                         int attackerPlayer,
                                         int defenderPlayer) {
    DamageResult result;
    result.effectiveness = effectiveness(move.type, defender.species(), battle.weather());

    if (move.accuracy >= 0) {
        const double accuracy = battle.accuracyMultiplier(attackerPlayer, AccuracyStat::Accuracy);
        const double evasion = battle.accuracyMultiplier(defenderPlayer, AccuracyStat::Evasion);
        const int chance = static_cast<int>(std::clamp(move.accuracy * accuracy / evasion, 0.0, 100.0));
        result.hit = battle.rollPercent(chance);
    }

    if (!result.hit || move.category == MoveCategory::Status || move.power <= 0) return result;
    if (result.effectiveness == 0.0) return result;

    int denominator = 24;
    if (move.criticalBonus == 1) denominator = 8;
    else if (move.criticalBonus == 2) denominator = 2;
    else if (move.criticalBonus >= 3) denominator = 1;

    result.critical = battle.randomInt(1, denominator) == 1;
    const double crit = result.critical ? 1.5 : 1.0;
    const double random = battle.randomInt(85, 100) / 100.0;
    result.damage = std::max(1, static_cast<int>(
        rawDamage(attacker, defender, move, battle, attackerPlayer, defenderPlayer) * crit * random));
    return result;
}

} // namespace pokemon
