#include "pokemon/ShowdownExporter.hpp"

#include "pokemon/DataCodec.hpp"

#include <array>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace pokemon {
namespace {
const char* showdownStat(Stat stat) {
    switch (stat) {
        case Stat::HP: return "HP";
        case Stat::Attack: return "Atk";
        case Stat::Defense: return "Def";
        case Stat::SpecialAttack: return "SpA";
        case Stat::SpecialDefense: return "SpD";
        case Stat::Speed: return "Spe";
        case Stat::Count: break;
    }
    return "?";
}

std::string spreadLine(const Pokemon& pokemon, bool ivs) {
    std::vector<std::string> parts;
    for (std::size_t i = 0; i < static_cast<std::size_t>(Stat::Count); ++i) {
        const auto stat = static_cast<Stat>(i);
        const int value = ivs ? pokemon.iv(stat) : pokemon.ev(stat);
        if ((ivs && value == 31) || (!ivs && value == 0)) continue;
        parts.push_back(std::to_string(value) + " " + showdownStat(stat));
    }
    if (parts.empty()) return {};
    std::string result;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i != 0) result += " / ";
        result += parts[i];
    }
    return result;
}

void writePokemon(const Pokemon& pokemon, std::ostream& output) {
    const std::string& species = pokemon.species().name();
    if (!pokemon.nickname().empty()) output << pokemon.nickname() << " (" << species << ")";
    else output << species;

    if (!pokemon.gender().empty()) output << " (" << pokemon.gender() << ")";
    if (pokemon.heldItem()) output << " @ " << pokemon.heldItem()->id;
    output << '\n';

    if (pokemon.ability()) output << "Ability: " << pokemon.ability()->id << '\n';
    if (pokemon.level() != 100) output << "Level: " << pokemon.level() << '\n';

    const std::string evs = spreadLine(pokemon, false);
    if (!evs.empty()) output << "EVs: " << evs << '\n';

    output << pokemon.nature().name() << " Nature\n";

    const std::string ivs = spreadLine(pokemon, true);
    if (!ivs.empty()) output << "IVs: " << ivs << '\n';

    if (pokemon.shiny()) output << "Shiny: Yes\n";
    if (pokemon.teraType()) output << "Tera Type: " << dataId(*pokemon.teraType()) << '\n';
    if (pokemon.dynamaxLevel() != 10) output << "Dynamax Level: " << pokemon.dynamaxLevel() << '\n';
    if (pokemon.gigantamax()) output << "Gigantamax: Yes\n";

    for (const auto& move : pokemon.moves()) {
        if (move && move->data()) output << "- " << move->data()->name << '\n';
    }
}
} // namespace

void ShowdownExporter::write(const Trainer& trainer, std::ostream& output) {
    for (std::size_t i = 0; i < trainer.team().size(); ++i) {
        if (i != 0) output << '\n';
        writePokemon(trainer.team()[i], output);
    }
}

std::string ShowdownExporter::toText(const Trainer& trainer) {
    std::ostringstream output;
    write(trainer, output);
    return output.str();
}

bool ShowdownExporter::saveFile(const Trainer& trainer, const std::string& path) {
    std::ofstream output(path);
    if (!output) return false;
    write(trainer, output);
    return static_cast<bool>(output);
}

} // namespace pokemon
