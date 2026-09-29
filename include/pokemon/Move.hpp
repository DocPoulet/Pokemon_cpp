#pragma once

#include "pokemon/Types.hpp"

#include <optional>
#include <string>
#include <vector>

namespace pokemon {

enum class EffectKind {
    StatChange,
    AccuracyChange,
    SetWeather,
    SetTerrain,
    RandomStatChange
};

struct MoveEffect {
    EffectKind kind = EffectKind::StatChange;
    Target target = Target::Opponent;
    Stat stat = Stat::Attack;
    AccuracyStat accuracyStat = AccuracyStat::Accuracy;
    int stages = 0;
    int chancePercent = 100;
    Weather weather = Weather::None;
    Terrain terrain = Terrain::None;
};

struct MoveData {
    std::string name;
    Type type = Type::Neutral;
    int power = 0;
    MoveCategory category = MoveCategory::Status;
    int accuracy = 100; // -1 = ne rate jamais
    int criticalBonus = 0;
    int pp = 5;
    bool recoilThird = false;
    bool struggle = false;
    std::vector<MoveEffect> effects;
};

class MoveInstance {
public:
    explicit MoveInstance(const MoveData* data = nullptr);

    const MoveData* data() const;
    int currentPP() const;
    int maxPP() const;
    bool usable() const;
    void consumePP();
    void restorePP();

private:
    const MoveData* data_ = nullptr;
    int currentPP_ = 0;
};

} // namespace pokemon
