#include "pokemon/Battle.hpp"

#include "pokemon/DamageCalculator.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace pokemon {
namespace {
std::string playerLabel(int player) {
    return player == 0 ? "J1" : "J2";
}
}

Battle::Battle(Trainer& player1, Trainer& player2, unsigned int seed)
    : trainers_{&player1, &player2}, rng_(seed) {}

Trainer& Battle::trainer(int player) {
    return *trainers_.at(static_cast<std::size_t>(player));
}

const Trainer& Battle::trainer(int player) const {
    return *trainers_.at(static_cast<std::size_t>(player));
}

Pokemon* Battle::active(int player) {
    auto& team = trainer(player).team();
    if (team.empty()) return nullptr;
    const auto index = activeIndex_.at(static_cast<std::size_t>(player));
    if (index >= team.size()) return nullptr;
    return &team[index];
}

const Pokemon* Battle::active(int player) const {
    const auto& team = trainer(player).team();
    if (team.empty()) return nullptr;
    const auto index = activeIndex_.at(static_cast<std::size_t>(player));
    if (index >= team.size()) return nullptr;
    return &team[index];
}

std::size_t Battle::activeIndex(int player) const {
    return activeIndex_.at(static_cast<std::size_t>(player));
}

bool Battle::switchPokemon(int player, std::size_t index, std::vector<BattleEvent>* events) {
    auto& team = trainer(player).team();
    if (index >= team.size() || team[index].fainted()) return false;
    if (index == activeIndex(player) && active(player) && !active(player)->fainted()) return false;

    activeIndex_.at(static_cast<std::size_t>(player)) = index;
    resetStages(player);
    if (events) {
        events->push_back({EventType::Switched,
            trainer(player).name() + " envoie " + team[index].name() + " !", player, 0});
    }
    return true;
}

bool Battle::finished() const {
    return trainer(0).allFainted() || trainer(1).allFainted();
}

int Battle::winner() const {
    if (!finished()) return -1;
    if (trainer(0).allFainted() && !trainer(1).allFainted()) return 1;
    if (trainer(1).allFainted() && !trainer(0).allFainted()) return 0;
    return -1;
}

Weather Battle::weather() const { return weather_; }
Terrain Battle::terrain() const { return terrain_; }
int Battle::weatherTurns() const { return weatherTurns_; }
int Battle::terrainTurns() const { return terrainTurns_; }

int Battle::stage(int player, Stat statValue) const {
    return stages_.at(static_cast<std::size_t>(player))
        .stats.at(static_cast<std::size_t>(statValue));
}

int Battle::accuracyStage(int player, AccuracyStat statValue) const {
    return stages_.at(static_cast<std::size_t>(player))
        .accuracy.at(static_cast<std::size_t>(statValue));
}

void Battle::changeStage(int player, Stat statValue, int delta) {
    auto& value = stages_.at(static_cast<std::size_t>(player))
        .stats.at(static_cast<std::size_t>(statValue));
    value = std::clamp(value + delta, -6, 6);
}

void Battle::changeAccuracyStage(int player, AccuracyStat statValue, int delta) {
    auto& value = stages_.at(static_cast<std::size_t>(player))
        .accuracy.at(static_cast<std::size_t>(statValue));
    value = std::clamp(value + delta, -6, 6);
}

double Battle::statMultiplier(int player, Stat statValue) const {
    const int s = stage(player, statValue);
    if (s > 0) return (2.0 + s) / 2.0;
    if (s < 0) return 2.0 / (2.0 - s);
    return 1.0;
}

double Battle::accuracyMultiplier(int player, AccuracyStat statValue) const {
    const int s = accuracyStage(player, statValue);
    if (s > 0) return (3.0 + s) / 3.0;
    if (s < 0) return 3.0 / (3.0 - s);
    return 1.0;
}

int Battle::randomInt(int minInclusive, int maxInclusive) {
    std::uniform_int_distribution<int> dist(minInclusive, maxInclusive);
    return dist(rng_);
}

bool Battle::rollPercent(int percent) {
    if (percent <= 0) return false;
    if (percent >= 100) return true;
    return randomInt(1, 100) <= percent;
}

const MoveData& Battle::struggleMove() const {
    static const MoveData struggle{
        "Lutte", Type::Neutral, 50, MoveCategory::Physical, -1, 0, 1,
        false, true, {}
    };
    return struggle;
}

void Battle::resetStages(int player) {
    stages_.at(static_cast<std::size_t>(player)) = StatStages{};
}

void Battle::setWeather(Weather weatherValue, int turns, std::vector<BattleEvent>& events) {
    weather_ = weatherValue;
    weatherTurns_ = std::max(0, turns);
    events.push_back({EventType::WeatherChanged,
        "Meteo : " + std::string(toString(weatherValue)) + ".", -1, weatherTurns_});
}

void Battle::setTerrain(Terrain terrainValue, int turns, std::vector<BattleEvent>& events) {
    terrain_ = terrainValue;
    terrainTurns_ = std::max(0, turns);
    events.push_back({EventType::TerrainChanged,
        "Terrain : " + std::string(toString(terrainValue)) + ".", -1, terrainTurns_});
}

void Battle::applyMoveEffects(int attackerPlayer, const MoveData& move,
                              Pokemon& attacker, Pokemon& defender,
                              std::vector<BattleEvent>& events) {
    const int defenderPlayer = 1 - attackerPlayer;

    for (const auto& effect : move.effects) {
        if (!rollPercent(effect.chancePercent)) continue;

        const int targetPlayer = effect.target == Target::Self ? attackerPlayer : defenderPlayer;
        Pokemon& targetPokemon = effect.target == Target::Self ? attacker : defender;

        switch (effect.kind) {
            case EffectKind::StatChange:
                changeStage(targetPlayer, effect.stat, effect.stages);
                events.push_back({EventType::StageChanged,
                    targetPokemon.name() + " : " + std::string(toString(effect.stat)) +
                    (effect.stages > 0 ? " augmente." : " diminue."),
                    targetPlayer, effect.stages});
                break;

            case EffectKind::AccuracyChange:
                changeAccuracyStage(targetPlayer, effect.accuracyStat, effect.stages);
                events.push_back({EventType::StageChanged,
                    targetPokemon.name() + (effect.stages > 0
                        ? " gagne en precision/esquive." : " perd en precision/esquive."),
                    targetPlayer, effect.stages});
                break;

            case EffectKind::SetWeather:
                setWeather(effect.weather, 5, events);
                break;

            case EffectKind::SetTerrain:
                setTerrain(effect.terrain, 5, events);
                break;

            case EffectKind::RandomStatChange: {
                const int statIndex = randomInt(
                    static_cast<int>(Stat::Attack), static_cast<int>(Stat::Speed));
                const auto randomStat = static_cast<Stat>(statIndex);
                changeStage(targetPlayer, randomStat, effect.stages);
                events.push_back({EventType::StageChanged,
                    targetPokemon.name() + " : " + std::string(toString(randomStat)) +
                    " augmente fortement.", targetPlayer, effect.stages});
                break;
            }
        }
    }
}

void Battle::resolveMove(int attackerPlayer, std::size_t slot,
                         std::vector<BattleEvent>& events) {
    const int defenderPlayer = 1 - attackerPlayer;
    Pokemon* attacker = active(attackerPlayer);
    Pokemon* defender = active(defenderPlayer);
    if (!attacker || !defender || attacker->fainted() || defender->fainted()) return;

    MoveInstance* instance = nullptr;
    const MoveData* move = nullptr;

    if (!attacker->hasUsableMove()) {
        move = &struggleMove();
    } else {
        if (slot >= attacker->moves().size() || !attacker->moves()[slot] ||
            !attacker->moves()[slot]->usable()) {
            events.push_back({EventType::Text,
                attacker->name() + " ne peut pas utiliser cette attaque.", attackerPlayer, 0});
            return;
        }
        instance = &*attacker->moves()[slot];
        move = instance->data();
    }

    events.push_back({EventType::MoveUsed,
        attacker->name() + " utilise " + move->name + " !", attackerPlayer, 0});

    if (instance) instance->consumePP();

    auto result = DamageCalculator::calculate(
        *attacker, *defender, *move, *this, attackerPlayer, defenderPlayer);

    if (!result.hit) {
        events.push_back({EventType::Text, "L'attaque echoue.", attackerPlayer, 0});
        return;
    }

    if (result.effectiveness == 0.0 && move->category != MoveCategory::Status) {
        events.push_back({EventType::Text,
            defender->name() + " est immunise.", defenderPlayer, 0});
        return;
    }

    if (result.critical) {
        events.push_back({EventType::Text, "Coup critique !", attackerPlayer, 0});
    }
    if (result.effectiveness > 1.0) {
        events.push_back({EventType::Text, "C'est super efficace !", attackerPlayer, 0});
    } else if (result.effectiveness > 0.0 && result.effectiveness < 1.0) {
        events.push_back({EventType::Text, "Ce n'est pas tres efficace...", attackerPlayer, 0});
    }

    if (result.damage > 0) {
        defender->damage(result.damage);
        events.push_back({EventType::Damage,
            defender->name() + " perd " + std::to_string(result.damage) + " PV.",
            defenderPlayer, result.damage});
    }

    applyMoveEffects(attackerPlayer, *move, *attacker, *defender, events);

    if (move->recoilThird && result.damage > 0 && !attacker->fainted()) {
        const int recoil = std::max(1, result.damage / 3);
        attacker->damage(recoil);
        events.push_back({EventType::Damage,
            attacker->name() + " subit " + std::to_string(recoil) + " PV de contrecoup.",
            attackerPlayer, recoil});
    }

    if (move->struggle && !attacker->fainted()) {
        const int recoil = std::max(1, attacker->maxHP() / 4);
        attacker->damage(recoil);
        events.push_back({EventType::Damage,
            attacker->name() + " est blesse par le contrecoup de Lutte (" +
                std::to_string(recoil) + " PV).", attackerPlayer, recoil});
    }

    if (defender->fainted()) {
        events.push_back({EventType::Fainted, defender->name() + " est KO !", defenderPlayer, 0});
    }
    if (attacker->fainted()) {
        events.push_back({EventType::Fainted, attacker->name() + " est KO !", attackerPlayer, 0});
    }
}

void Battle::applyEndTurn(std::vector<BattleEvent>& events) {
    if (weather_ == Weather::Sandstorm) {
        for (int player = 0; player < 2; ++player) {
            Pokemon* pokemon = active(player);
            if (!pokemon || pokemon->fainted()) continue;

            bool immune = false;
            for (const auto type : pokemon->species().types()) {
                if (type == Type::Rock || type == Type::Ground || type == Type::Steel) {
                    immune = true;
                    break;
                }
            }

            if (!immune) {
                const int damage = std::max(1, pokemon->maxHP() / 16);
                pokemon->damage(damage);
                events.push_back({EventType::Damage,
                    pokemon->name() + " est blesse par la tempete de sable (" +
                        std::to_string(damage) + " PV).", player, damage});
                if (pokemon->fainted()) {
                    events.push_back({EventType::Fainted,
                        pokemon->name() + " est KO !", player, 0});
                }
            }
        }
    }

    if (terrain_ == Terrain::Grassy) {
        for (int player = 0; player < 2; ++player) {
            Pokemon* pokemon = active(player);
            if (!pokemon || pokemon->fainted()) continue;
            const int before = pokemon->currentHP();
            pokemon->heal(std::max(1, pokemon->maxHP() / 16));
            const int healed = pokemon->currentHP() - before;
            if (healed > 0) {
                events.push_back({EventType::Heal,
                    pokemon->name() + " recupere " + std::to_string(healed) +
                        " PV grace au Champ Herbu.", player, healed});
            }
        }
    }

    if (weatherTurns_ > 0 && --weatherTurns_ == 0) {
        weather_ = Weather::None;
        events.push_back({EventType::WeatherChanged, "La meteo se dissipe.", -1, 0});
    }
    if (terrainTurns_ > 0 && --terrainTurns_ == 0) {
        terrain_ = Terrain::None;
        events.push_back({EventType::TerrainChanged, "Le terrain se dissipe.", -1, 0});
    }
}

std::vector<BattleEvent> Battle::resolveTurn(const BattleAction& action1,
                                              const BattleAction& action2) {
    std::vector<BattleEvent> events;
    if (finished()) return events;

    const std::array<BattleAction, 2> actions{action1, action2};

    for (int player = 0; player < 2; ++player) {
        if (actions[static_cast<std::size_t>(player)].type == ActionType::Run) {
            events.push_back({EventType::BattleEnded,
                trainer(player).name() + " prend la fuite.", player, 0});
            return events;
        }
    }

    // Les changements sont resolus avant les attaques.
    for (int player = 0; player < 2; ++player) {
        const auto& action = actions[static_cast<std::size_t>(player)];
        if (action.type == ActionType::Switch) {
            if (!switchPokemon(player, action.index, &events)) {
                events.push_back({EventType::Text,
                    playerLabel(player) + " : changement impossible.", player, 0});
            }
        }
    }

    std::vector<int> movers;
    for (int player = 0; player < 2; ++player) {
        if (actions[static_cast<std::size_t>(player)].type == ActionType::Move) movers.push_back(player);
    }

    if (movers.size() == 2) {
        const Pokemon* p0 = active(0);
        const Pokemon* p1 = active(1);
        if (p0 && p1) {
            const double speed0 = p0->stat(Stat::Speed) * statMultiplier(0, Stat::Speed);
            const double speed1 = p1->stat(Stat::Speed) * statMultiplier(1, Stat::Speed);
            if (speed1 > speed0 || (speed0 == speed1 && randomInt(0, 1) == 1)) {
                std::swap(movers[0], movers[1]);
            }
        }
    }

    for (const int player : movers) {
        Pokemon* pokemon = active(player);
        Pokemon* opponent = active(1 - player);
        if (!pokemon || !opponent || pokemon->fainted() || opponent->fainted()) continue;
        resolveMove(player, actions[static_cast<std::size_t>(player)].index, events);
    }

    applyEndTurn(events);

    if (finished()) {
        const int winningPlayer = winner();
        if (winningPlayer >= 0) {
            events.push_back({EventType::BattleEnded,
                trainer(winningPlayer).name() + " gagne le combat !", winningPlayer, 0});
        }
    }

    return events;
}

} // namespace pokemon
