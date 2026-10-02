#include "pokemon/GameData.hpp"
#include "pokemon/Localization.hpp"
#include "pokemon/ShowdownImporter.hpp"

#include <cassert>
#include <iostream>

using namespace pokemon;

int main() {
    GameData data;

    // Les 151 espèces de Kanto sont présentes. MissingNo. reste un fallback interne
    // et n'est pas exposé dans la liste normale quand le catalogue est chargé.
    const auto names = data.speciesNames();
    assert(names.size() == 151);
    assert(data.hasSpecies("Bulbasaur"));
    assert(data.hasSpecies("Mew"));
    assert(data.hasSpecies("Mewtwo"));
    assert(data.hasSpecies("Farfetch’d"));

    // Quelques références représentatives du Pokédex et des learnsets Showdown.
    const auto& mewtwo = data.species("Mewtwo");
    assert(mewtwo.baseStat(Stat::HP) == 106);
    assert(mewtwo.baseStat(Stat::SpecialAttack) == 154);
    assert(mewtwo.canLearnMove("Psystrike"));
    assert(mewtwo.canHaveAbility("Pressure"));

    const auto& ditto = data.species("Ditto");
    assert(ditto.movePool().size() == 1);
    assert(ditto.canLearnMove("Transform"));
    assert(ditto.canHaveAbility("Imposter"));

    const auto& mew = data.species("Mew");
    assert(mew.baseStat(Stat::HP) == 100);
    assert(mew.baseStat(Stat::Attack) == 100);
    assert(mew.canLearnMove("Earthquake"));
    assert(mew.canLearnMove("Tera Blast"));

    // Les références du movepool pointent toujours vers le catalogue global unique.
    bool foundEarthquake = false;
    for (const MoveData* move : mew.movePool()) {
        if (move && move->name == "Earthquake") {
            assert(move == &data.move("Earthquake"));
            foundEarthquake = true;
        }
    }
    assert(foundEarthquake);

    // Affichage français séparé des IDs moteur.
    assert(frenchSpeciesName("Caterpie") == "Chenipan");
    assert(frenchSpeciesName("Gengar") == "Ectoplasma");
    assert(frenchSpeciesName("Mr. Mime") == "M. Mime");
    assert(frenchSpeciesName("Farfetch’d") == "Canarticho");
    assert(frenchSpeciesName("Dragonite") == "Dracolosse");

    // Les exports Showdown ASCII courants restent acceptés pour Farfetch'd,
    // et les talents à plusieurs mots ne sont plus compactés.
    const std::string showdown =
        "Duck (Farfetch'd) @ Leftovers\n"
        "Ability: Keen Eye\n"
        "Level: 50\n"
        "- Brave Bird\n"
        "- Knock Off\n";
    auto trainer = ShowdownImporter::buildTeam(showdown, "Kanto", data);
    assert(trainer.team().size() == 1);
    assert(trainer.team()[0].species().name() == "Farfetch’d");
    assert(trainer.team()[0].ability() == &data.ability("Keen Eye"));

    std::cout << "Tous les tests v0.8.1 sont passes.\n";
    return 0;
}
