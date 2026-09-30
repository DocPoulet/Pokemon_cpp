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

bool hasType(const Pokemon& pokemon, Type type) {
    for (const auto pokemonType : pokemon.species().types()) {
        if (pokemonType == type) return true;
    }
    return false;
}

bool immuneToStatus(const Pokemon& pokemon, StatusCondition status) {
    switch (status) {
        case StatusCondition::Burn:
            return hasType(pokemon, Type::Fire);
        case StatusCondition::Poison:
            return hasType(pokemon, Type::Poison) || hasType(pokemon, Type::Steel);
        case StatusCondition::Paralysis:
            return hasType(pokemon, Type::Electric);
        case StatusCondition::Freeze:
            return hasType(pokemon, Type::Ice);
        case StatusCondition::Sleep:
        case StatusCondition::None:
            return false;
    }
    return false;
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

            case EffectKind::ApplyStatus: {
                if (targetPokemon.fainted() || effect.status == StatusCondition::None) break;
                if (targetPokemon.status() != StatusCondition::None) {
                    events.push_back({EventType::StatusChanged,
                        targetPokemon.name() + " a deja un probleme de statut.", targetPlayer, 0});
                    break;
                }
                if (immuneToStatus(targetPokemon, effect.status)) {
                    events.push_back({EventType::StatusChanged,
                        targetPokemon.name() + " est immunise contre " +
                            std::string(toString(effect.status)) + ".", targetPlayer, 0});
                    break;
                }
                const int turns = effect.status == StatusCondition::Sleep ? randomInt(1, 3) : 0;
                if (targetPokemon.setStatus(effect.status, turns)) {
                    events.push_back({EventType::StatusChanged,
                        targetPokemon.name() + " subit " + std::string(toString(effect.status)) + ".",
                        targetPlayer, turns});
                }
                break;
            }
        }
    }
}

bool Battle::canAct(int player, std::vector<BattleEvent>& events) {
    Pokemon* pokemon = active(player);
    if (!pokemon || pokemon->fainted()) return false;

    switch (pokemon->status()) {
        case StatusCondition::None:
        case StatusCondition::Burn:
        case StatusCondition::Poison:
            return true;
        case StatusCondition::Paralysis:
            if (rollPercent(25)) {
                events.push_back({EventType::StatusChanged,
                    pokemon->name() + " est paralyse et ne peut pas agir !", player, 0});
                return false;
            }
            return true;
        case StatusCondition::Sleep:
            if (pokemon->statusTurns() <= 0) {
                pokemon->cureStatus();
                events.push_back({EventType::StatusChanged,
                    pokemon->name() + " se reveille !", player, 0});
                return true;
            }
            pokemon->setStatusTurns(pokemon->statusTurns() - 1);
            events.push_back({EventType::StatusChanged,
                pokemon->name() + " dort profondement.", player, pokemon->statusTurns()});
            if (pokemon->statusTurns() == 0) pokemon->cureStatus();
            return false;
        case StatusCondition::Freeze:
            if (rollPercent(20)) {
                pokemon->cureStatus();
                events.push_back({EventType::StatusChanged,
                    pokemon->name() + " degele !", player, 0});
                return true;
            }
            events.push_back({EventType::StatusChanged,
                pokemon->name() + " est gele et ne peut pas agir !", player, 0});
            return false;
    }
    return true;
}

int Battle::movePriority(int player, const BattleAction& action) const {
    if (action.type != ActionType::Move) return 0;
    const Pokemon* pokemon = active(player);
    if (!pokemon || pokemon->fainted()) return 0;
    if (!pokemon->hasUsableMove()) return struggleMove().priority;
    if (action.index >= pokemon->moves().size()) return 0;
    const auto& instance = pokemon->moves()[action.index];
    if (!instance || !instance->usable() || !instance->data()) return 0;
    return instance->data()->priority;
}

double Battle::effectiveSpeed(int player) const {
    const Pokemon* pokemon = active(player);
    if (!pokemon) return 0.0;
    double speed = pokemon->stat(Stat::Speed) * statMultiplier(player, Stat::Speed);
    if (pokemon->status() == StatusCondition::Paralysis) speed *= 0.5;
    return speed;
}

void Battle::resolveMove(int attackerPlayer, std::size_t slot,
                         std::vector<BattleEvent>& events) {
    const int defenderPlayer = 1 - attackerPlayer;
    Pokemon* attacker = active(attackerPlayer);
    Pokemon* defender = active(defenderPlayer);
    if (!attacker || !defender || attacker->fainted() || defender->fainted()) return;
    if (!canAct(attackerPlayer, events)) return;

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

    const int minHits = std::max(1, move->minHits);
    const int maxHits = std::max(minHits, move->maxHits);
    const int requestedHits = move->category == MoveCategory::Status ? 1 : randomInt(minHits, maxHits);
    int landedHits = 0;
    int totalDamage = 0;
    bool moveHit = false;
    double effectiveness = 1.0;

    MoveData hitMove = *move;
    for (int hit = 0; hit < requestedHits; ++hit) {
        if (defender->fainted()) break;
        if (hit > 0) hitMove.accuracy = -1;
        auto result = DamageCalculator::calculate(
            *attacker, *defender, hitMove, *this, attackerPlayer, defenderPlayer);
        effectiveness = result.effectiveness;
        if (!result.hit) {
            if (hit == 0) {
                events.push_back({EventType::Text, "L'attaque echoue.", attackerPlayer, 0});
                return;
            }
            break;
        }
        moveHit = true;
        if (result.effectiveness == 0.0 && move->category != MoveCategory::Status) {
            events.push_back({EventType::Text,
                defender->name() + " est immunise.", defenderPlayer, 0});
            return;
        }
        if (result.critical) {
            events.push_back({EventType::Text, "Coup critique !", attackerPlayer, 0});
        }
        if (result.damage > 0) {
            const int before = defender->currentHP();
            defender->damage(result.damage);
            const int actualDamage = before - defender->currentHP();
            totalDamage += actualDamage;
            ++landedHits;
            events.push_back({EventType::Damage,
                defender->name() + " perd " + std::to_string(actualDamage) + " PV.",
                defenderPlayer, actualDamage});
        } else if (move->category == MoveCategory::Status) {
            ++landedHits;
        }
    }

    if (!moveHit) return;
    if (effectiveness > 1.0) {
        events.push_back({EventType::Text, "C'est super efficace !", attackerPlayer, 0});
    } else if (effectiveness > 0.0 && effectiveness < 1.0) {
        events.push_back({EventType::Text, "Ce n'est pas tres efficace...", attackerPlayer, 0});
    }
    if (requestedHits > 1 && landedHits > 0) {
        events.push_back({EventType::Text,
            "L'attaque touche " + std::to_string(landedHits) + " fois !", attackerPlayer, landedHits});
    }

    applyMoveEffects(attackerPlayer, *move, *attacker, *defender, events);

    if (move->drainPercent > 0 && totalDamage > 0 && !attacker->fainted()) {
        const int before = attacker->currentHP();
        attacker->heal(std::max(1, totalDamage * move->drainPercent / 100));
        const int healed = attacker->currentHP() - before;
        if (healed > 0) {
            events.push_back({EventType::Heal,
                attacker->name() + " absorbe " + std::to_string(healed) + " PV.",
                attackerPlayer, healed});
        }
    }

    int recoilPercent = move->recoilPercent;
    if (move->recoilThird && recoilPercent <= 0) recoilPercent = 33;
    if (recoilPercent > 0 && totalDamage > 0 && !attacker->fainted()) {
        const int recoil = std::max(1, totalDamage * recoilPercent / 100);
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
    if (attacker->heldItem() == HeldItem::LifeOrb &&
        move->category != MoveCategory::Status && totalDamage > 0 && !attacker->fainted()) {
        const int recoil = std::max(1, attacker->maxHP() / 10);
        attacker->damage(recoil);
        events.push_back({EventType::Damage,
            attacker->name() + " perd " + std::to_string(recoil) +
                " PV a cause de l'Orbe Vie.", attackerPlayer, recoil});
    }

    if (defender->fainted()) {
        events.push_back({EventType::Fainted, defender->name() + " est KO !", defenderPlayer, 0});
    }
    if (attacker->fainted()) {
        events.push_back({EventType::Fainted, attacker->name() + " est KO !", attackerPlayer, 0});
    }
}

void Battle::applyEndTurn(std::vector<BattleEvent>& events) {
    for (int player = 0; player < 2; ++player) {
        Pokemon* pokemon = active(player);
        if (!pokemon || pokemon->fainted()) continue;
        int residual = 0;
        if (pokemon->status() == StatusCondition::Burn) residual = std::max(1, pokemon->maxHP() / 16);
        else if (pokemon->status() == StatusCondition::Poison) residual = std::max(1, pokemon->maxHP() / 8);
        if (residual > 0) {
            const auto status = pokemon->status();
            pokemon->damage(residual);
            events.push_back({EventType::Damage,
                pokemon->name() + " souffre de " + std::string(toString(status)) +
                    " (" + std::to_string(residual) + " PV).", player, residual});
            if (pokemon->fainted()) {
                events.push_back({EventType::Fainted, pokemon->name() + " est KO !", player, 0});
            }
        }
    }

    if (weather_ == Weather::Sandstorm) {
        for (int player = 0; player < 2; ++player) {
            Pokemon* pokemon = active(player);
            if (!pokemon || pokemon->fainted()) continue;
            const bool immune = hasType(*pokemon, Type::Rock) ||
                hasType(*pokemon, Type::Ground) || hasType(*pokemon, Type::Steel);
            if (!immune) {
                const int damage = std::max(1, pokemon->maxHP() / 16);
                pokemon->damage(damage);
                events.push_back({EventType::Damage,
                    pokemon->name() + " est blesse par la tempete de sable (" +
                        std::to_string(damage) + " PV).", player, damage});
                if (pokemon->fainted()) {
                    events.push_back({EventType::Fainted, pokemon->name() + " est KO !", player, 0});
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

    for (int player = 0; player < 2; ++player) {
        Pokemon* pokemon = active(player);
        if (!pokemon || pokemon->fainted() || pokemon->heldItem() != HeldItem::Leftovers) continue;
        const int before = pokemon->currentHP();
        pokemon->heal(std::max(1, pokemon->maxHP() / 16));
        const int healed = pokemon->currentHP() - before;
        if (healed > 0) {
            events.push_back({EventType::Heal,
                pokemon->name() + " recupere " + std::to_string(healed) +
                    " PV avec les Restes.", player, healed});
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

    for (int player = 0; player < 2; ++player) {
        const auto& action = actions[static_cast<std::size_t>(player)];
        if (action.type == ActionType::Switch && !switchPokemon(player, action.index, &events)) {
            events.push_back({EventType::Text,
                playerLabel(player) + " : changement impossible.", player, 0});
        }
    }

    std::vector<int> movers;
    for (int player = 0; player < 2; ++player) {
        if (actions[static_cast<std::size_t>(player)].type == ActionType::Move) movers.push_back(player);
    }

    if (movers.size() == 2) {
        const int priority0 = movePriority(0, actions[0]);
        const int priority1 = movePriority(1, actions[1]);
        if (priority1 > priority0) {
            std::swap(movers[0], movers[1]);
        } else if (priority0 == priority1) {
            const double speed0 = effectiveSpeed(0);
            const double speed1 = effectiveSpeed(1);
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
