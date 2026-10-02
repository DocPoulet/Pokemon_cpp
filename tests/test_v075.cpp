#include "pokemon/GameData.hpp"
#include "pokemon/ShowdownExporter.hpp"
#include "pokemon/ShowdownImporter.hpp"
#include "pokemon/TeamBuilder.hpp"

#include <cassert>
#include <fstream>
#include <string>

using namespace pokemon;

int main() {
    GameData data;
    TeamBuilder builder(data);

    PokemonConfig config;
    config.species = "Bulbasaur";
    config.nickname = "Miguel";
    config.gender = "M";
    config.level = 100;
    config.ivs.fill(31);
    config.ivs[static_cast<std::size_t>(Stat::Attack)] = 0;
    config.evs[static_cast<std::size_t>(Stat::HP)] = 100;
    config.evs[static_cast<std::size_t>(Stat::Defense)] = 156;
    config.evs[static_cast<std::size_t>(Stat::SpecialAttack)] = 172;
    config.evs[static_cast<std::size_t>(Stat::Speed)] = 56;
    config.nature = Nature("Quiet", Stat::SpecialAttack, Stat::Speed);
    config.moves = {"Body Slam", "Curse", "Double-Edge", "Energy Ball"};
    config.ability = "Overgrow";
    config.heldItem = "Air Balloon";
    config.shiny = true;
    config.teraType = Type::Electric;
    config.dynamaxLevel = 7;
    config.gigantamax = true;

    Trainer original("Showdown");
    assert(builder.addPokemon(original, config));

    const std::string text = ShowdownExporter::toText(original);
    assert(text.find("Miguel (Bulbasaur) (M) @ Air Balloon") != std::string::npos);
    assert(text.find("Ability: Overgrow") != std::string::npos);
    assert(text.find("EVs: 100 HP / 156 Def / 172 SpA / 56 Spe") != std::string::npos);
    assert(text.find("Quiet Nature") != std::string::npos);
    assert(text.find("IVs: 0 Atk") != std::string::npos);
    assert(text.find("Shiny: Yes") != std::string::npos);
    assert(text.find("Tera Type: Electric") != std::string::npos);
    assert(text.find("Dynamax Level: 7") != std::string::npos);
    assert(text.find("Gigantamax: Yes") != std::string::npos);

    Trainer loaded = ShowdownImporter::buildTeam(text, "Reloaded", data);
    assert(loaded.team().size() == 1);
    const Pokemon& pokemon = loaded.team().front();
    assert(pokemon.nickname() == "Miguel");
    assert(pokemon.iv(Stat::Attack) == 0);
    assert(pokemon.iv(Stat::HP) == 31);
    assert(pokemon.dynamaxLevel() == 7);
    assert(pokemon.gigantamax());
    assert(pokemon.ability() == &data.ability("Overgrow"));
    assert(pokemon.heldItem() == &data.item("Air Balloon"));
    assert(pokemon.moves()[3]->data() == &data.move("Energy Ball"));

    const std::string path = "/tmp/pokemon_cpp_showdown_test.txt";
    assert(ShowdownExporter::saveFile(original, path));
    Trainer fromFile = ShowdownImporter::loadFile(path, "File", data);
    assert(fromFile.team().size() == 1);
    assert(fromFile.team().front().nickname() == "Miguel");

    return 0;
}
