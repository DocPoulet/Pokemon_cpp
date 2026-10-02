#include "pokemon/DataCodec.hpp"

#include <stdexcept>

namespace pokemon {

std::string_view dataId(Stat value) {
    switch (value) {
        case Stat::HP: return "HP";
        case Stat::Attack: return "Attack";
        case Stat::Defense: return "Defense";
        case Stat::SpecialAttack: return "SpecialAttack";
        case Stat::SpecialDefense: return "SpecialDefense";
        case Stat::Speed: return "Speed";
        case Stat::Count: break;
    }
    return "Unknown";
}

std::string_view dataId(Type value) {
    switch (value) {
        case Type::Steel: return "Steel";
        case Type::Fighting: return "Fighting";
        case Type::Dragon: return "Dragon";
        case Type::Water: return "Water";
        case Type::Electric: return "Electric";
        case Type::Fairy: return "Fairy";
        case Type::Fire: return "Fire";
        case Type::Ice: return "Ice";
        case Type::Bug: return "Bug";
        case Type::Normal: return "Normal";
        case Type::Grass: return "Grass";
        case Type::Poison: return "Poison";
        case Type::Psychic: return "Psychic";
        case Type::Rock: return "Rock";
        case Type::Ground: return "Ground";
        case Type::Ghost: return "Ghost";
        case Type::Dark: return "Dark";
        case Type::Flying: return "Flying";
        case Type::Neutral: return "Neutral";
        case Type::Count: break;
    }
    return "Unknown";
}

std::string_view dataId(Ability value) {
    switch (value) {
        case Ability::None: return "None";
        case Ability::Blaze: return "Blaze";
        case Ability::Torrent: return "Torrent";
        case Ability::Overgrow: return "Overgrow";
        case Ability::Levitate: return "Levitate";
        case Ability::Guts: return "Guts";
        case Ability::PrimordialSea: return "PrimordialSea";
        case Ability::DesolateLand: return "DesolateLand";
        case Ability::DeltaStream: return "DeltaStream";
        case Ability::Static: return "Static";
        case Ability::Adaptability: return "Adaptability";
    }
    return "Unknown";
}

std::string_view dataId(HeldItem value) {
    switch (value) {
        case HeldItem::None: return "None";
        case HeldItem::Leftovers: return "Leftovers";
        case HeldItem::LifeOrb: return "LifeOrb";
        case HeldItem::RedOrb: return "RedOrb";
        case HeldItem::BlueOrb: return "BlueOrb";
        case HeldItem::AirBalloon: return "AirBalloon";
    }
    return "Unknown";
}

Stat statFromDataId(std::string_view id) {
    if (id == "HP") return Stat::HP;
    if (id == "Attack") return Stat::Attack;
    if (id == "Defense") return Stat::Defense;
    if (id == "SpecialAttack") return Stat::SpecialAttack;
    if (id == "SpecialDefense") return Stat::SpecialDefense;
    if (id == "Speed") return Stat::Speed;
    throw std::invalid_argument("Stat JSON inconnue: " + std::string(id));
}

Type typeFromDataId(std::string_view id) {
    if (id == "Steel") return Type::Steel;
    if (id == "Fighting") return Type::Fighting;
    if (id == "Dragon") return Type::Dragon;
    if (id == "Water") return Type::Water;
    if (id == "Electric") return Type::Electric;
    if (id == "Fairy") return Type::Fairy;
    if (id == "Fire") return Type::Fire;
    if (id == "Ice") return Type::Ice;
    if (id == "Bug") return Type::Bug;
    if (id == "Normal") return Type::Normal;
    if (id == "Grass") return Type::Grass;
    if (id == "Poison") return Type::Poison;
    if (id == "Psychic") return Type::Psychic;
    if (id == "Rock") return Type::Rock;
    if (id == "Ground") return Type::Ground;
    if (id == "Ghost") return Type::Ghost;
    if (id == "Dark") return Type::Dark;
    if (id == "Flying") return Type::Flying;
    if (id == "Neutral") return Type::Neutral;
    throw std::invalid_argument("Type JSON inconnu: " + std::string(id));
}

Ability abilityFromDataId(std::string_view id) {
    if (id == "None") return Ability::None;
    if (id == "Blaze") return Ability::Blaze;
    if (id == "Torrent") return Ability::Torrent;
    if (id == "Overgrow") return Ability::Overgrow;
    if (id == "Levitate") return Ability::Levitate;
    if (id == "Guts") return Ability::Guts;
    if (id == "PrimordialSea") return Ability::PrimordialSea;
    if (id == "DesolateLand") return Ability::DesolateLand;
    if (id == "DeltaStream") return Ability::DeltaStream;
    if (id == "Static") return Ability::Static;
    if (id == "Adaptability") return Ability::Adaptability;
    throw std::invalid_argument("Talent JSON inconnu: " + std::string(id));
}

HeldItem heldItemFromDataId(std::string_view id) {
    if (id == "None") return HeldItem::None;
    if (id == "Leftovers") return HeldItem::Leftovers;
    if (id == "LifeOrb") return HeldItem::LifeOrb;
    if (id == "RedOrb") return HeldItem::RedOrb;
    if (id == "BlueOrb") return HeldItem::BlueOrb;
    if (id == "AirBalloon") return HeldItem::AirBalloon;
    throw std::invalid_argument("Objet JSON inconnu: " + std::string(id));
}

} // namespace pokemon
