#include "pokemon/BattlePokemonData.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/TeamIO.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <set>

using namespace pokemon;

namespace {
void testSpeciesLoadedFromJson() {
    GameData data;
    const auto& salameche = data.species("Salameche");
    assert(salameche.baseStat(Stat::Speed) == 65);
    assert(salameche.canLearnMove("Flammeche"));
    assert(!salameche.canLearnMove("Pistolet a O"));
    assert(salameche.canHaveAbility(Ability::Blaze));
    assert(!salameche.canHaveAbility(Ability::Torrent));
}

void testBattlePokemonPresets() {
    GameData data;
    BattlePokemonData presets(data);
    assert(presets.has("quick_salameche"));
    Pokemon pokemon = presets.build("quick_bulbizarre");
    assert(pokemon.species().name() == "Bulbizarre");
    assert(pokemon.name() == "Bulbi");
    assert(pokemon.nickname() == "Bulbi");
    assert(pokemon.form() == "Base");
    assert(pokemon.ability() == Ability::Overgrow);
}

void testDuplicateMovesRejected() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Salameche";
    config.moves = {"Flammeche", "Flammeche", "", ""};
    config.ability = Ability::Blaze;
    assert(!builder.validate(config).empty());
}

void testMovePoolRejected() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Salameche";
    config.moves = {"Pistolet a O", "", "", ""};
    config.ability = Ability::Blaze;
    assert(!builder.validate(config).empty());
}

void testNullAndWrongAbilityRejected() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Carapuce";
    config.moves = {"Pistolet a O", "", "", ""};
    config.ability = Ability::None;
    assert(!builder.validate(config).empty());
    config.ability = Ability::Blaze;
    assert(!builder.validate(config).empty());
    config.ability = Ability::Torrent;
    assert(builder.validate(config).empty());
}

void testNicknameFormAndTeamRoundTrip() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Salameche";
    config.nickname = "Zippo";
    config.form = "Base";
    config.level = 42;
    config.ivs.fill(31);
    config.moves = {"Flammeche", "Vive-Attaque", "", ""};
    config.ability = Ability::Blaze;
    config.heldItem = HeldItem::LifeOrb;

    Trainer trainer("Test");
    builder.addPokemon(trainer, config);
    std::stringstream saved;
    TeamIO::save(trainer, saved);
    Trainer loaded = TeamIO::load(saved, data);
    assert(loaded.team().size() == 1);
    assert(loaded.team()[0].nickname() == "Zippo");
    assert(loaded.team()[0].name() == "Zippo");
    assert(loaded.team()[0].form() == "Base");
}

void testEmptyNicknameFallsBackToSpecies() {
    GameData data;
    BattlePokemonData presets(data);
    Pokemon pokemon = presets.build("quick_salameche");
    assert(pokemon.nickname().empty());
    assert(pokemon.name() == "Salameche");
}

void testMissingNoFallback() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig invalid;
    invalid.species = "PokemonQuiNExistePas";
    Pokemon pokemon = builder.buildPokemon(invalid);

    assert(pokemon.species().name() == "MissingNo.");
    assert(pokemon.name() == "MissingNo.");
    assert(pokemon.level() == 100);
    assert(pokemon.species().baseStat(Stat::HP) == 33);
    assert(pokemon.species().baseStat(Stat::Attack) == 136);
    assert(pokemon.species().baseStat(Stat::Defense) == 0);
    assert(pokemon.species().baseStat(Stat::SpecialAttack) == 1);
    assert(pokemon.species().baseStat(Stat::SpecialDefense) == 1);
    assert(pokemon.species().baseStat(Stat::Speed) == 29);
    assert(pokemon.species().types().size() == 2);
    assert(pokemon.species().types()[0] == Type::Flying);
    assert(pokemon.species().types()[1] == Type::Normal);
    assert(pokemon.ability() == Ability::None);

    int moveCount = 0;
    std::set<std::string> names;
    for (const auto& move : pokemon.moves()) {
        if (move && move->data()) {
            ++moveCount;
            names.insert(move->data()->name);
        }
    }
    assert(moveCount == 4);
    assert(names.size() == 4);
}

void testMissingSpeciesFileStillWorks() {
    GameData data("/tmp/pokemon_cpp_missing_species_file_that_does_not_exist.json");
    assert(data.hasSpecies("MissingNo."));
    assert(data.species("Pikachu").name() == "MissingNo.");
    const auto names = data.speciesNames();
    assert(names.size() == 1);
    assert(names.front() == "MissingNo.");
}

void testUnknownPresetBecomesMissingNo() {
    GameData data;
    BattlePokemonData presets(data, "/tmp/pokemon_cpp_missing_presets_file_that_does_not_exist.json");
    Pokemon pokemon = presets.build("preset_inconnu");
    assert(pokemon.species().name() == "MissingNo.");
}
}

int main() {
    testSpeciesLoadedFromJson();
    testBattlePokemonPresets();
    testDuplicateMovesRejected();
    testMovePoolRejected();
    testNullAndWrongAbilityRejected();
    testNicknameFormAndTeamRoundTrip();
    testEmptyNicknameFallsBackToSpecies();
    testMissingNoFallback();
    testMissingSpeciesFileStillWorks();
    testUnknownPresetBecomesMissingNo();
    std::cout << "Tous les tests v0.7 sont passes.\n";
    return 0;
}
