#include "pokemon/Types.hpp"

namespace pokemon {

std::string_view toString(Stat stat) {
    switch (stat) {
        case Stat::HP: return "PV";
        case Stat::Attack: return "Attaque";
        case Stat::Defense: return "Defense";
        case Stat::SpecialAttack: return "Attaque Speciale";
        case Stat::SpecialDefense: return "Defense Speciale";
        case Stat::Speed: return "Vitesse";
        case Stat::Count: break;
    }
    return "Inconnu";
}

std::string_view toString(Type type) {
    switch (type) {
        case Type::Steel: return "Acier";
        case Type::Fighting: return "Combat";
        case Type::Dragon: return "Dragon";
        case Type::Water: return "Eau";
        case Type::Electric: return "Electrique";
        case Type::Fairy: return "Fee";
        case Type::Fire: return "Feu";
        case Type::Ice: return "Glace";
        case Type::Bug: return "Insecte";
        case Type::Normal: return "Normal";
        case Type::Grass: return "Plante";
        case Type::Poison: return "Poison";
        case Type::Psychic: return "Psy";
        case Type::Rock: return "Roche";
        case Type::Ground: return "Sol";
        case Type::Ghost: return "Spectre";
        case Type::Dark: return "Tenebres";
        case Type::Flying: return "Vol";
        case Type::Neutral: return "Neutre";
        case Type::Count: break;
    }
    return "Inconnu";
}

std::string_view toString(MoveCategory category) {
    switch (category) {
        case MoveCategory::Status: return "Statut";
        case MoveCategory::Physical: return "Physique";
        case MoveCategory::Special: return "Special";
    }
    return "Inconnu";
}

std::string_view toString(Weather weather) {
    switch (weather) {
        case Weather::None: return "Aucune";
        case Weather::Sun: return "Soleil";
        case Weather::Rain: return "Pluie";
        case Weather::Sandstorm: return "Tempete de sable";
        case Weather::Snow: return "Neige";
        case Weather::ExtremelyHarshSunlight: return "Soleil Intense";
        case Weather::HeavyRain: return "Pluie Battante";
        case Weather::StrongWinds: return "Vent Mysterieux";
    }
    return "Inconnue";
}

std::string_view toString(Terrain terrain) {
    switch (terrain) {
        case Terrain::None: return "Aucun";
        case Terrain::Electric: return "Electrique";
        case Terrain::Misty: return "Brumeux";
        case Terrain::Grassy: return "Herbu";
        case Terrain::Psychic: return "Psychique";
    }
    return "Inconnu";
}

} // namespace pokemon
