#include "pokemon/BattlePokemonData.hpp"
#include "pokemon/GameData.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/ShowdownExporter.hpp"
#include "pokemon/ShowdownImporter.hpp"

#include <cassert>
#include <iostream>
#include <sstream>

using namespace pokemon;

namespace {
void testSpeciesReferencesGlobalCatalogs() {
    GameData data;
    const PokemonSpecies& bulbasaur = data.species("Bulbasaur");

    bool foundEnergyBall = false;
    for (const MoveData* move : bulbasaur.movePool()) {
        if (move && move->name == "Energy Ball") {
            assert(move == &data.move("Energy Ball"));
            foundEnergyBall = true;
        }
    }
    assert(foundEnergyBall);

    bool foundOvergrow = false;
    bool foundChlorophyll = false;
    for (const AbilityData* ability : bulbasaur.abilities()) {
        if (ability && ability->id == "Overgrow") {
            assert(ability == &data.ability("Overgrow"));
            foundOvergrow = true;
        }
        if (ability && ability->id == "Chlorophyll") {
            assert(ability == &data.ability("Chlorophyll"));
            foundChlorophyll = true;
        }
    }
    assert(foundOvergrow);
    assert(foundChlorophyll);
}

void testBattlePokemonKeepsCatalogReferences() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Bulbasaur";
    config.ivs.fill(31);
    config.moves = {"Energy Ball", "Body Slam", "", ""};
    config.ability = "Overgrow";
    config.heldItem = "Air Balloon";

    Pokemon first = builder.buildPokemon(config);
    Pokemon second = builder.buildPokemon(config);

    assert(first.ability() == &data.ability("Overgrow"));
    assert(second.ability() == first.ability());
    assert(first.heldItem() == &data.item("Air Balloon"));
    assert(second.heldItem() == first.heldItem());
    assert(first.moves()[0] && first.moves()[0]->data() == &data.move("Energy Ball"));
    assert(second.moves()[0] && second.moves()[0]->data() == first.moves()[0]->data());
}

void testShowdownStoresIdsAndRestoresReferences() {
    GameData data;
    TeamBuilder builder(data);
    PokemonConfig config;
    config.species = "Charmander";
    config.ivs.fill(31);
    config.moves = {"Ember", "Quick Attack", "", ""};
    config.ability = "Blaze";
    config.heldItem = "Life Orb";

    Trainer original("Refs");
    assert(builder.addPokemon(original, config));

    const std::string saved = ShowdownExporter::toText(original);
    assert(saved.find("Charmander @ Life Orb") != std::string::npos);
    assert(saved.find("Ability: Blaze") != std::string::npos);
    assert(saved.find("- Ember") != std::string::npos);

    Trainer loaded = ShowdownImporter::buildTeam(saved, "Refs", data);
    assert(loaded.team().size() == 1);
    assert(loaded.team()[0].heldItem() == &data.item("Life Orb"));
    assert(loaded.team()[0].ability() == &data.ability("Blaze"));
    assert(loaded.team()[0].moves()[0]->data() == &data.move("Ember"));
}
}

int main() {
    testSpeciesReferencesGlobalCatalogs();
    testBattlePokemonKeepsCatalogReferences();
    testShowdownStoresIdsAndRestoresReferences();
    std::cout << "Tous les tests v0.7.4 sont passes.\n";
    return 0;
}
