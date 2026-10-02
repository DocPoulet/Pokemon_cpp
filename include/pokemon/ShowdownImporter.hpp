#pragma once

#include "pokemon/GameData.hpp"
#include "pokemon/TeamBuilder.hpp"
#include "pokemon/Trainer.hpp"

#include <iosfwd>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Résultat de lecture d'un export Pokémon Showdown.
 *
 * Entrées:
 *   Texte Showdown contenant un ou plusieurs sets séparés par des lignes vides.
 *
 * Sortie:
 *   pokemon: configurations extraites dans l'ordre du texte.
 *   warnings: lignes reconnues partiellement ou données non supportées.
 */
struct ShowdownImportResult {
    std::vector<PokemonConfig> pokemon;
    std::vector<std::string> warnings;
};

/**
 * Convertit les exports texte Pokémon Showdown vers les structures du simulateur.
 *
 * Entrées:
 *   En-tête Showdown, Ability, EVs, IVs, Nature, Shiny, Tera Type et lignes "- Move".
 *
 * Sortie:
 *   PokemonConfig avec IV à 31 par défaut et EV à 0 par défaut.
 *
 * Notes:
 *   Les champs Gender, Shiny et Tera Type sont conservés mais n'ont pas encore d'effet mécanique.
 */
class ShowdownImporter {
public:
    static ShowdownImportResult parse(std::istream& input);
    static ShowdownImportResult parseText(const std::string& text);

    /**
     * Construit une équipe depuis un export Showdown.
     *
     * Entrées:
     *   text: export Showdown complet.
     *   trainerName: nom du dresseur créé.
     *   data: catalogue du jeu utilisé par TeamBuilder.
     *   warnings: destination optionnelle des avertissements de parsing.
     *
     * Sortie:
     *   Trainer: équipe de six Pokémon maximum. Les configs invalides suivent le fallback MissingNo.
     */
    static Trainer buildTeam(const std::string& text, const std::string& trainerName,
                             const GameData& data, std::vector<std::string>* warnings = nullptr);

    /** Charge un fichier texte Showdown et construit l'équipe correspondante. */
    static Trainer loadFile(const std::string& path, const std::string& trainerName,
                            const GameData& data, std::vector<std::string>* warnings = nullptr);
};

} // namespace pokemon
