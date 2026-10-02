#include "pokemon/Localization.hpp"
#include "pokemon/Pokemon.hpp"

#include <unordered_map>

namespace pokemon {
namespace {
std::string_view translate(std::string_view value,
                           const std::unordered_map<std::string, std::string>& table) {
    const auto it = table.find(std::string(value));
    return it == table.end() ? value : std::string_view(it->second);
}
}

std::string_view frenchSpeciesName(std::string_view name) {
    static const std::unordered_map<std::string, std::string> table{
        {"Bulbasaur", "Bulbizarre"},
        {"Ivysaur", "Herbizarre"},
        {"Venusaur", "Florizarre"},
        {"Charmander", "Salamèche"},
        {"Charmeleon", "Reptincel"},
        {"Charizard", "Dracaufeu"},
        {"Squirtle", "Carapuce"},
        {"Wartortle", "Carabaffe"},
        {"Blastoise", "Tortank"},
        {"Caterpie", "Chenipan"},
        {"Metapod", "Chrysacier"},
        {"Butterfree", "Papilusion"},
        {"Weedle", "Aspicot"},
        {"Kakuna", "Coconfort"},
        {"Beedrill", "Dardargnan"},
        {"Pidgey", "Roucool"},
        {"Pidgeotto", "Roucoups"},
        {"Pidgeot", "Roucarnage"},
        {"Rattata", "Rattata"},
        {"Raticate", "Rattatac"},
        {"Spearow", "Piafabec"},
        {"Fearow", "Rapasdepic"},
        {"Ekans", "Abo"},
        {"Arbok", "Arbok"},
        {"Pikachu", "Pikachu"},
        {"Raichu", "Raichu"},
        {"Sandshrew", "Sabelette"},
        {"Sandslash", "Sablaireau"},
        {"Nidoran-F", "Nidoran♀"},
        {"Nidorina", "Nidorina"},
        {"Nidoqueen", "Nidoqueen"},
        {"Nidoran-M", "Nidoran♂"},
        {"Nidorino", "Nidorino"},
        {"Nidoking", "Nidoking"},
        {"Clefairy", "Mélofée"},
        {"Clefable", "Mélodelfe"},
        {"Vulpix", "Goupix"},
        {"Ninetales", "Feunard"},
        {"Jigglypuff", "Rondoudou"},
        {"Wigglytuff", "Grodoudou"},
        {"Zubat", "Nosferapti"},
        {"Golbat", "Nosferalto"},
        {"Oddish", "Mystherbe"},
        {"Gloom", "Ortide"},
        {"Vileplume", "Rafflesia"},
        {"Paras", "Paras"},
        {"Parasect", "Parasect"},
        {"Venonat", "Mimitoss"},
        {"Venomoth", "Aéromite"},
        {"Diglett", "Taupiqueur"},
        {"Dugtrio", "Triopikeur"},
        {"Meowth", "Miaouss"},
        {"Persian", "Persian"},
        {"Psyduck", "Psykokwak"},
        {"Golduck", "Akwakwak"},
        {"Mankey", "Férosinge"},
        {"Primeape", "Colossinge"},
        {"Growlithe", "Caninos"},
        {"Arcanine", "Arcanin"},
        {"Poliwag", "Ptitard"},
        {"Poliwhirl", "Têtarte"},
        {"Poliwrath", "Tartard"},
        {"Abra", "Abra"},
        {"Kadabra", "Kadabra"},
        {"Alakazam", "Alakazam"},
        {"Machop", "Machoc"},
        {"Machoke", "Machopeur"},
        {"Machamp", "Mackogneur"},
        {"Bellsprout", "Chétiflor"},
        {"Weepinbell", "Boustiflor"},
        {"Victreebel", "Empiflor"},
        {"Tentacool", "Tentacool"},
        {"Tentacruel", "Tentacruel"},
        {"Geodude", "Racaillou"},
        {"Graveler", "Gravalanch"},
        {"Golem", "Grolem"},
        {"Ponyta", "Ponyta"},
        {"Rapidash", "Galopa"},
        {"Slowpoke", "Ramoloss"},
        {"Slowbro", "Flagadoss"},
        {"Magnemite", "Magnéti"},
        {"Magneton", "Magnéton"},
        {"Farfetch’d", "Canarticho"},
        {"Doduo", "Doduo"},
        {"Dodrio", "Dodrio"},
        {"Seel", "Otaria"},
        {"Dewgong", "Lamantine"},
        {"Grimer", "Tadmorv"},
        {"Muk", "Grotadmorv"},
        {"Shellder", "Kokiyas"},
        {"Cloyster", "Crustabri"},
        {"Gastly", "Fantominus"},
        {"Haunter", "Spectrum"},
        {"Gengar", "Ectoplasma"},
        {"Onix", "Onix"},
        {"Drowzee", "Soporifik"},
        {"Hypno", "Hypnomade"},
        {"Krabby", "Krabby"},
        {"Kingler", "Krabboss"},
        {"Voltorb", "Voltorbe"},
        {"Electrode", "Électrode"},
        {"Exeggcute", "Nœunœuf"},
        {"Exeggutor", "Noadkoko"},
        {"Cubone", "Osselait"},
        {"Marowak", "Ossatueur"},
        {"Hitmonlee", "Kicklee"},
        {"Hitmonchan", "Tygnon"},
        {"Lickitung", "Excelangue"},
        {"Koffing", "Smogo"},
        {"Weezing", "Smogogo"},
        {"Rhyhorn", "Rhinocorne"},
        {"Rhydon", "Rhinoféros"},
        {"Chansey", "Leveinard"},
        {"Tangela", "Saquedeneu"},
        {"Kangaskhan", "Kangourex"},
        {"Horsea", "Hypotrempe"},
        {"Seadra", "Hypocéan"},
        {"Goldeen", "Poissirène"},
        {"Seaking", "Poissoroy"},
        {"Staryu", "Stari"},
        {"Starmie", "Staross"},
        {"Mr. Mime", "M. Mime"},
        {"Scyther", "Insécateur"},
        {"Jynx", "Lippoutou"},
        {"Electabuzz", "Élektek"},
        {"Magmar", "Magmar"},
        {"Pinsir", "Scarabrute"},
        {"Tauros", "Tauros"},
        {"Magikarp", "Magicarpe"},
        {"Gyarados", "Léviator"},
        {"Lapras", "Lokhlass"},
        {"Ditto", "Métamorph"},
        {"Eevee", "Évoli"},
        {"Vaporeon", "Aquali"},
        {"Jolteon", "Voltali"},
        {"Flareon", "Pyroli"},
        {"Porygon", "Porygon"},
        {"Omanyte", "Amonita"},
        {"Omastar", "Amonistar"},
        {"Kabuto", "Kabuto"},
        {"Kabutops", "Kabutops"},
        {"Aerodactyl", "Ptéra"},
        {"Snorlax", "Ronflex"},
        {"Articuno", "Artikodin"},
        {"Zapdos", "Électhor"},
        {"Moltres", "Sulfura"},
        {"Dratini", "Minidraco"},
        {"Dragonair", "Draco"},
        {"Dragonite", "Dracolosse"},
        {"Mewtwo", "Mewtwo"},
        {"Mew", "Mew"},
        {"MissingNo.", "MissingNo."}
    };
    return translate(name, table);
}

std::string_view frenchMoveName(std::string_view name) {
    static const std::unordered_map<std::string, std::string> table{
        {"Ember", "Flammèche"}, {"Scratch", "Griffe"}, {"Water Gun", "Pistolet à O"},
        {"Vine Whip", "Fouet Lianes"}, {"Hone Claws", "Aiguisage"}, {"Psychic", "Psyko"},
        {"Acupressure", "Acupression"}, {"Sunny Day", "Zénith"}, {"Rain Dance", "Danse Pluie"},
        {"Grassy Terrain", "Champ Herbu"}, {"Quick Attack", "Vive-Attaque"},
        {"Mega Drain", "Méga-Sangsue"}, {"Take Down", "Bélier"}, {"Fury Swipes", "Combo-Griffe"},
        {"Will-O-Wisp", "Feu Follet"}, {"Toxic", "Toxik"}, {"Thunder Wave", "Cage-Éclair"},
        {"Sleep Powder", "Poudre Dodo"}, {"Ice Beam", "Laser Glace"},
        {"Body Slam", "Plaquage"}, {"Curse", "Malédiction"}, {"Double-Edge", "Damoclès"},
        {"Energy Ball", "Éco-Sphère"}, {"Close Combat", "Close Combat"}, {"Struggle", "Lutte"}
    };
    return translate(name, table);
}

std::string_view frenchNatureName(std::string_view name) {
    static const std::unordered_map<std::string, std::string> table{
        {"Hardy","Hardi"},{"Lonely","Solo"},{"Brave","Brave"},{"Adamant","Rigide"},{"Naughty","Mauvais"},
        {"Bold","Assuré"},{"Docile","Docile"},{"Relaxed","Relax"},{"Impish","Malin"},{"Lax","Lâche"},
        {"Timid","Timide"},{"Hasty","Pressé"},{"Serious","Sérieux"},{"Jolly","Jovial"},{"Naive","Naïf"},
        {"Modest","Modeste"},{"Mild","Doux"},{"Quiet","Discret"},{"Bashful","Pudique"},{"Rash","Foufou"},
        {"Calm","Calme"},{"Gentle","Gentil"},{"Sassy","Malpoli"},{"Careful","Prudent"},{"Quirky","Bizarre"}
    };
    return translate(name, table);
}

std::string_view frenchAbilityName(std::string_view name) {
    static const std::unordered_map<std::string, std::string> table{
        {"Blaze","Brasier"},{"Torrent","Torrent"},{"Overgrow","Engrais"},{"Levitate","Lévitation"},
        {"Guts","Cran"},{"Primordial Sea","Mer Primaire"},{"Desolate Land","Terre Finale"},
        {"Delta Stream","Souffle Delta"},{"Static","Statik"},{"Adaptability","Adaptabilité"}
    };
    return translate(name, table);
}

std::string_view frenchItemName(std::string_view name) {
    static const std::unordered_map<std::string, std::string> table{
        {"Leftovers","Restes"},{"Life Orb","Orbe Vie"},{"Red Orb","Orbe Rouge"},
        {"Blue Orb","Orbe Bleue"},{"Air Balloon","Ballon"}
    };
    return translate(name, table);
}

std::string battleDisplayName(const Pokemon& pokemon) {
    if (!pokemon.nickname().empty()) return pokemon.nickname();
    return std::string(frenchSpeciesName(pokemon.species().name()));
}

} // namespace pokemon
