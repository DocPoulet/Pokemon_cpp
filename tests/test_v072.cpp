#include "pokemon/GameData.hpp"
#include "pokemon/ShowdownImporter.hpp"
#include "pokemon/ShowdownExporter.hpp"

#include <cassert>
#include <sstream>
#include <string>

using namespace pokemon;

int main() {
    GameData data;
    const std::string sample =
        "Miguel (Bulbasaur) (M) @ Air Balloon\n"
        "Ability: Overgrow\n"
        "EVs: 100 HP / 156 Def / 172 SpA / 56 Spe\n"
        "Quiet Nature\n"
        "IVs: 0 Atk\n"
        "Shiny: Yes\n"
        "Tera Type: Electric\n"
        "- Body Slam\n"
        "- Curse\n"
        "- Double-Edge\n"
        "- Energy Ball\n";

    const auto parsed = ShowdownImporter::parseText(sample);
    assert(parsed.pokemon.size() == 1);
    const auto& c = parsed.pokemon.front();
    assert(c.species == "Bulbasaur");
    assert(c.nickname == "Miguel");
    assert(c.gender == "M");
    assert(c.level == 100);
    assert(c.ivs[static_cast<std::size_t>(Stat::Attack)] == 0);
    assert(c.ivs[static_cast<std::size_t>(Stat::HP)] == 31);
    assert(c.evs[static_cast<std::size_t>(Stat::HP)] == 100);
    assert(c.evs[static_cast<std::size_t>(Stat::Defense)] == 156);
    assert(c.evs[static_cast<std::size_t>(Stat::SpecialAttack)] == 172);
    assert(c.evs[static_cast<std::size_t>(Stat::Speed)] == 56);
    assert(c.ability == "Overgrow");
    assert(c.heldItem == "Air Balloon");
    assert(c.shiny);
    assert(c.teraType && *c.teraType == Type::Electric);
    assert(c.nature.name() == "Quiet");
    assert(c.nature.increased() == Stat::SpecialAttack);
    assert(c.nature.decreased() == Stat::Speed);
    assert(c.moves[0] == "Body Slam");
    assert(c.moves[3] == "Energy Ball");

    Trainer team = ShowdownImporter::buildTeam(sample, "Import", data);
    assert(team.team().size() == 1);
    const Pokemon& p = team.team().front();
    assert(p.species().name() == "Bulbasaur");
    assert(p.name() == "Miguel");
    assert(p.gender() == "M");
    assert(p.shiny());
    assert(p.teraType() && *p.teraType() == Type::Electric);
    assert(p.iv(Stat::Attack) == 0);
    assert(p.iv(Stat::HP) == 31);

    const std::string saved = ShowdownExporter::toText(team);
    Trainer reloaded = ShowdownImporter::buildTeam(saved, "Import", data);
    assert(reloaded.team().size() == 1);
    const Pokemon& q = reloaded.team().front();
    assert(q.name() == "Miguel");
    assert(q.gender() == "M");
    assert(q.shiny());
    assert(q.teraType() && *q.teraType() == Type::Electric);

    const std::string englishSpecies =
        "Charmander @ Life Orb\n"
        "Ability: Blaze\n"
        "Timid Nature\n"
        "- Ember\n"
        "- Quick Attack\n";
    const auto aliases = ShowdownImporter::parseText(englishSpecies);
    assert(aliases.pokemon.front().species == "Charmander");
    assert(aliases.pokemon.front().moves[0] == "Ember");
    assert(aliases.pokemon.front().moves[1] == "Quick Attack");

    return 0;
}
