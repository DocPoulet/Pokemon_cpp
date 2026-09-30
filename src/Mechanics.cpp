#include "pokemon/Mechanics.hpp"

namespace pokemon {

std::string_view toString(StatusCondition status) {
    switch (status) {
        case StatusCondition::None: return "Aucun";
        case StatusCondition::Burn: return "Brulure";
        case StatusCondition::Poison: return "Poison";
        case StatusCondition::Paralysis: return "Paralysie";
        case StatusCondition::Sleep: return "Sommeil";
        case StatusCondition::Freeze: return "Gel";
    }
    return "Inconnu";
}

std::string_view toString(HeldItem item) {
    switch (item) {
        case HeldItem::None: return "Aucun";
        case HeldItem::Leftovers: return "Restes";
        case HeldItem::LifeOrb: return "Orbe Vie";
        case HeldItem::RedOrb: return "Orbe Rouge";
        case HeldItem::BlueOrb: return "Orbe Bleue";
    }
    return "Inconnu";
}

std::string_view toString(Ability ability) {
    switch (ability) {
        case Ability::None: return "Aucun";
        case Ability::Blaze: return "Brasier";
        case Ability::Torrent: return "Torrent";
        case Ability::Overgrow: return "Engrais";
        case Ability::Levitate: return "Levitation";
        case Ability::Guts: return "Cran";
        case Ability::PrimordialSea: return "Mer Primaire";
        case Ability::DesolateLand: return "Terre Finale";
        case Ability::DeltaStream: return "Souffle Delta";
    }
    return "Inconnu";
}

} // namespace pokemon
