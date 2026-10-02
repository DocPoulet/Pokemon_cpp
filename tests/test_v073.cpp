#include "pokemon/GameData.hpp"
#include "pokemon/Localization.hpp"
#include "pokemon/ShowdownImporter.hpp"
#include "pokemon/ShowdownExporter.hpp"

#include <cassert>
#include <sstream>
#include <string>

using namespace pokemon;

int main() {
    GameData data;

    // Le moteur utilise les identifiants anglais comme source de vérité.
    assert(data.hasSpecies("Bulbasaur"));
    assert(!data.hasSpecies("Bulbizarre"));
    assert(data.hasMove("Ember"));
    assert(!data.hasMove("Flammeche"));
    assert(data.species("Bulbasaur").canLearnMove("Energy Ball"));

    // La traduction est strictement réservée à l'affichage.
    assert(frenchSpeciesName("Bulbasaur") == "Bulbizarre");
    assert(frenchMoveName("Ember") == "Flammèche");

    const std::string showdown =
        "Miguel (Bulbasaur) (M) @ Air Balloon\n"
        "Ability: Overgrow\n"
        "EVs: 100 HP / 156 Def / 172 SpA / 56 Spe\n"
        "Quiet Nature\n"
        "IVs: 0 Atk\n"
        "Shiny: Yes\n"
        "Tera Type: Electric\n"
        "Dynamax Level: 7\n"
        "Gigantamax: Yes\n"
        "- Body Slam\n"
        "- Curse\n"
        "- Double-Edge\n"
        "- Energy Ball\n";

    const auto parsed = ShowdownImporter::parseText(showdown);
    assert(parsed.pokemon.size() == 1);
    const PokemonConfig& config = parsed.pokemon.front();
    assert(config.species == "Bulbasaur");
    assert(config.dynamaxLevel == 7);
    assert(config.gigantamax);
    assert(config.ivs[static_cast<std::size_t>(Stat::HP)] == 31);
    assert(config.ivs[static_cast<std::size_t>(Stat::Attack)] == 0);

    Trainer team = ShowdownImporter::buildTeam(showdown, "Import", data);
    assert(team.team().size() == 1);
    const Pokemon& pokemon = team.team().front();
    assert(pokemon.species().name() == "Bulbasaur");
    assert(pokemon.dynamaxLevel() == 7);
    assert(pokemon.gigantamax());
    assert(battleDisplayName(pokemon) == "Miguel");

    // Les valeurs Showdown absentes utilisent bien les valeurs par défaut demandées.
    const auto defaults = ShowdownImporter::parseText(
        "Bulbasaur\nAbility: Overgrow\nHardy Nature\n- Energy Ball\n");
    assert(defaults.pokemon.front().dynamaxLevel == 10);
    assert(!defaults.pokemon.front().gigantamax);

    // Le format Showdown est désormais l’unique format de sauvegarde d’équipe.
    const std::string saved = ShowdownExporter::toText(team);
    assert(saved.find("Dynamax Level: 7") != std::string::npos);
    assert(saved.find("Gigantamax: Yes") != std::string::npos);
    Trainer loaded = ShowdownImporter::buildTeam(saved, "Import", data);
    assert(loaded.team().front().dynamaxLevel() == 7);
    assert(loaded.team().front().gigantamax());

    return 0;
}
