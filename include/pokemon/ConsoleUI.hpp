#pragma once

#include "pokemon/AI.hpp"
#include "pokemon/Battle.hpp"

#include <iosfwd>

namespace pokemon {

/**
 * Gère l'interface texte utilisée pour jouer dans un terminal.
 *
 * Entrées:
 *   input (std::istream&): Flux utilisé pour lire les choix du joueur.
 *   output (std::ostream&): Flux utilisé pour afficher menus, équipes et événements.
 *   Battle / BattleController: Objets fournis à run() pour lancer une partie.
 *
 * Sortie:
 *   ConsoleUI : couche d'interface séparée du moteur de combat.
 */
class ConsoleUI {
public:
    /**
     * Construit l'interface console autour de deux flux.
     *
     * Entrées:
     *   input (std::istream&): Source des saisies utilisateur.
     *   output (std::ostream&): Destination des messages affichés.
     *
     * Sortie:
     *   ConsoleUI: Interface prête à lancer une boucle de jeu.
     */
    ConsoleUI(std::istream& input, std::ostream& output);

    /**
     * Lance la boucle interactive d'un combat jusqu'à sa fin ou à une fuite.
     *
     * Entrées:
     *   battle (Battle&): Combat déjà initialisé à piloter.
     *   opponentAI (BattleController&): Contrôleur utilisé pour le second joueur.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Lit les choix humains, demande les actions de l'IA, résout les tours et affiche les événements.
     */
    void run(Battle& battle, BattleController& opponentAI);

    /**
     * Lance un combat en choisissant indépendamment le contrôleur de chaque joueur.
     *
     * Entrées:
     *   battle (Battle&): Combat à piloter.
     *   player1Controller / player2Controller: Contrôleur IA, ou nullptr pour un joueur humain.
     *
     * Sortie:
     *   Aucune.
     */
    void run(Battle& battle, BattleController* player1Controller, BattleController* player2Controller);

private:
    std::istream& input_; ///< Flux de saisie utilisateur.
    std::ostream& output_; ///< Flux d'affichage.

    /**
     * Demande au joueur humain quelle action effectuer.
     *
     * Entrées:
     *   battle (Battle&): Combat courant utilisé pour afficher les choix disponibles.
     *
     * Sortie:
     *   BattleAction: Action valide choisie par l'utilisateur.
     */
    BattleAction chooseHumanAction(Battle& battle, int player);

    /**
     * Demande au joueur de sélectionner une attaque utilisable.
     *
     * Entrées:
     *   battle (Battle&): Combat courant et Pokémon actif du joueur.
     *
     * Sortie:
     *   BattleAction: Action de type Move contenant le slot choisi.
     */
    BattleAction chooseMove(Battle& battle, int player);

    /**
     * Demande au joueur de sélectionner un Pokémon de remplacement.
     *
     * Entrées:
     *   battle (Battle&): Combat courant et équipe du joueur.
     *
     * Sortie:
     *   BattleAction: Action de type Switch contenant l'index choisi.
     */
    BattleAction chooseSwitch(Battle& battle, int player);

    /**
     * Affiche le contenu d'une équipe et l'état de ses Pokémon.
     *
     * Entrées:
     *   trainer (const Trainer&): Dresseur dont l'équipe doit être affichée.
     *
     * Sortie:
     *   Aucune.
     */
    void printTeam(const Trainer& trainer) const;

    /**
     * Affiche les événements produits par le moteur de combat.
     *
     * Entrées:
     *   events (const std::vector<BattleEvent>&): Événements ordonnés d'un tour.
     *
     * Sortie:
     *   Aucune.
     */
    void printEvents(const std::vector<BattleEvent>& events) const;

    /**
     * Lit un entier depuis le flux d'entrée en gérant les saisies invalides.
     *
     * Entrées:
     *   Aucune directement ; la valeur est lue depuis input_.
     *
     * Sortie:
     *   int: Entier correctement extrait du flux.
     */
    int readInt();
};

} // namespace pokemon
