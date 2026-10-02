#include "pokemon/BattlePokemonData.hpp"

#include "pokemon/DataCodec.hpp"
#include "pokemon/JsonLite.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace pokemon {
namespace {
std::array<int, static_cast<std::size_t>(Stat::Count)> readStats(const JsonValue& object) {
    std::array<int, static_cast<std::size_t>(Stat::Count)> result{};
    result[static_cast<std::size_t>(Stat::HP)] = object.at("hp").asInt();
    result[static_cast<std::size_t>(Stat::Attack)] = object.at("attack").asInt();
    result[static_cast<std::size_t>(Stat::Defense)] = object.at("defense").asInt();
    result[static_cast<std::size_t>(Stat::SpecialAttack)] = object.at("special_attack").asInt();
    result[static_cast<std::size_t>(Stat::SpecialDefense)] = object.at("special_defense").asInt();
    result[static_cast<std::size_t>(Stat::Speed)] = object.at("speed").asInt();
    return result;
}
}

#ifndef POKEMON_DATA_DIR
#define POKEMON_DATA_DIR "data"
#endif

BattlePokemonData::BattlePokemonData(const GameData& data)
    : BattlePokemonData(data, std::string(POKEMON_DATA_DIR) + "/battle_pokemon.json") {}

BattlePokemonData::BattlePokemonData(const GameData& data, const std::string& path)
    : data_(data) {
    load(path);
}

void BattlePokemonData::load(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        std::cerr << "[Data] Presets de combat introuvables: " << path
                  << ". Les presets demandes seront remplaces par MissingNo.\n";
        return;
    }

    try {
        const JsonValue root = JsonValue::parse(input);
        if (root.at("version").asInt() != 1) {
            std::cerr << "[Data] Version de battle_pokemon.json non supportee. "
                      << "Les presets demandes seront remplaces par MissingNo.\n";
            return;
        }

        TeamBuilder builder(data_);
        for (const auto& entry : root.at("pokemon").asArray()) {
            try {
                const std::string id = entry.at("id").asString();
                if (id.empty()) throw std::runtime_error("Un preset de combat doit avoir un id.");
                if (presets_.find(id) != presets_.end()) throw std::runtime_error("Preset duplique: " + id);

                PokemonConfig config;
                config.species = entry.at("species").asString();
                config.nickname = entry.contains("nickname") ? entry.at("nickname").asString() : "";
                config.form = entry.contains("form") ? entry.at("form").asString() : "Base";
                config.gender = entry.contains("gender") ? entry.at("gender").asString() : "";
                config.shiny = entry.contains("shiny") ? entry.at("shiny").asBoolean() : false;
                if (entry.contains("tera_type") && !entry.at("tera_type").isNull()) {
                    config.teraType = typeFromDataId(entry.at("tera_type").asString());
                }
                config.dynamaxLevel = entry.contains("dynamax_level") ? entry.at("dynamax_level").asInt() : 10;
                config.gigantamax = entry.contains("gigantamax") ? entry.at("gigantamax").asBoolean() : false;
                config.level = entry.at("level").asInt();
                config.ivs = readStats(entry.at("ivs"));
                config.evs = readStats(entry.at("evs"));

                const auto& nature = entry.at("nature");
                config.nature = Nature(
                    nature.at("name").asString(),
                    statFromDataId(nature.at("increased").asString()),
                    statFromDataId(nature.at("decreased").asString()));

                const auto& moves = entry.at("moves").asArray();
                if (moves.empty() || moves.size() > config.moves.size()) {
                    throw std::runtime_error("Preset " + id + ": il faut entre 1 et 4 attaques.");
                }
                for (std::size_t i = 0; i < moves.size(); ++i) config.moves[i] = moves[i].asString();

                config.ability = entry.at("ability").asString();
                config.heldItem = entry.at("held_item").asString();
                if (config.heldItem == "None") config.heldItem.clear();

                const std::string error = builder.validate(config);
                if (!error.empty()) {
                    std::cerr << "[Data] Preset " << id << " invalide: " << error
                              << ". Il produira MissingNo.\n";
                }
                presets_.emplace(id, std::move(config));
            } catch (const std::exception& error) {
                std::cerr << "[Data] Preset ignore car illisible: " << error.what()
                          << ". Toute demande absente produira MissingNo.\n";
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "[Data] battle_pokemon.json invalide: " << error.what()
                  << ". Les presets demandes seront remplaces par MissingNo.\n";
    }
}

const PokemonConfig& BattlePokemonData::preset(const std::string& id) const {
    const auto it = presets_.find(id);
    if (it == presets_.end()) throw std::out_of_range("Preset de combat inconnu: " + id);
    return it->second;
}

Pokemon BattlePokemonData::build(const std::string& id) const {
    const auto it = presets_.find(id);
    if (it != presets_.end()) return TeamBuilder(data_).buildPokemon(it->second);
    PokemonConfig invalid;
    invalid.species = id;
    return TeamBuilder(data_).buildPokemon(invalid);
}

std::vector<std::string> BattlePokemonData::ids() const {
    std::vector<std::string> result;
    result.reserve(presets_.size());
    for (const auto& [id, unused] : presets_) {
        (void)unused;
        result.push_back(id);
    }
    return result;
}

bool BattlePokemonData::has(const std::string& id) const {
    return presets_.find(id) != presets_.end();
}

} // namespace pokemon
