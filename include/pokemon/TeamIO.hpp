#pragma once

#include "pokemon/GameData.hpp"
#include "pokemon/Trainer.hpp"

#include <iosfwd>
#include <string>

namespace pokemon {

/**
 * Sérialise et recharge des équipes dans un format texte versionné propre au projet.
 *
 * Entrées:
 *   Trainer: Équipe à sauvegarder.
 *   GameData: Catalogue nécessaire pour reconstruire espèces et attaques lors du chargement.
 *   std::istream / std::ostream: Flux texte, donc utilisable avec des fichiers ou des tests en mémoire.
 *
 * Sortie:
 *   Une sauvegarde POKEMON_TEAM_V2 ou un Trainer reconstruit et validé. Le lecteur accepte aussi V1.
 */
class TeamIO {
public:
    /** Sauvegarde une équipe vers un flux texte. Entrée : Trainer. Sortie : aucune. */
    static void save(const Trainer& trainer, std::ostream& output);

    /**
     * Recharge une équipe depuis un flux texte.
     *
     * Entrées:
     *   input (std::istream&): Sauvegarde au format POKEMON_TEAM_V2 ou ancien format V1.
     *   data (const GameData&): Catalogue utilisé pour résoudre espèces et attaques.
     *
     * Sortie:
     *   Trainer: Nouvelle équipe indépendante de la sauvegarde.
     *
     * Erreurs:
     *   Lance std::runtime_error si le format ou une donnée est invalide.
     */
    static Trainer load(std::istream& input, const GameData& data);

    /** Sauvegarde une équipe dans un fichier. Entrées : Trainer et chemin. Sortie : true en cas de succès. */
    static bool saveFile(const Trainer& trainer, const std::string& path);

    /** Recharge une équipe depuis un fichier. Entrées : chemin et GameData. Sortie : Trainer. */
    static Trainer loadFile(const std::string& path, const GameData& data);
};

} // namespace pokemon
