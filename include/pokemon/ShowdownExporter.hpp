#pragma once

#include "pokemon/Trainer.hpp"

#include <iosfwd>
#include <string>

namespace pokemon {

/**
 * Exporte les équipes du simulateur au format texte Pokémon Showdown.
 *
 * Entrées:
 *   Trainer contenant jusqu'à six Pokémon prêts au combat.
 *
 * Sortie:
 *   Texte Showdown réimportable par ShowdownImporter et compatible avec les outils Showdown.
 */
class ShowdownExporter {
public:
    /** Écrit une équipe au format Showdown dans un flux texte. */
    static void write(const Trainer& trainer, std::ostream& output);

    /** Retourne l'export Showdown complet d'une équipe. */
    static std::string toText(const Trainer& trainer);

    /** Sauvegarde l'équipe dans un fichier texte Showdown. Retourne true si l'écriture réussit. */
    static bool saveFile(const Trainer& trainer, const std::string& path);
};

} // namespace pokemon
