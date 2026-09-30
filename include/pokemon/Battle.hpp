#pragma once

#include "pokemon/Trainer.hpp"
#include "pokemon/Types.hpp"

#include <array>
#include <cstddef>
#include <random>
#include <string>
#include <vector>

namespace pokemon {

/**
 * Définit le type d'action qu'un joueur peut demander pendant un tour.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   ActionType : valeur utilisée avec BattleAction::index pour interpréter l'action.
 */
enum class ActionType {
    Move,   ///< Utiliser une attaque ; index désigne le slot de l'attaque.
    Switch, ///< Changer de Pokémon ; index désigne sa position dans l'équipe.
    Run     ///< Tenter de quitter le combat.
};

/**
 * Décrit une action soumise au moteur de combat.
 *
 * Entrées:
 *   type (ActionType): Nature de l'action demandée.
 *   index (std::size_t): Slot d'attaque ou index d'équipe selon le type.
 *
 * Sortie:
 *   BattleAction : instruction complète pouvant être traitée par Battle::resolveTurn().
 */
struct BattleAction {
    ActionType type = ActionType::Move; ///< Type de l'action.
    std::size_t index = 0; ///< Index associé à l'action.
};

/**
 * Définit la catégorie d'un événement produit par le moteur.
 *
 * Entrées:
 *   Aucune.
 *
 * Sortie:
 *   EventType : valeur permettant à l'interface d'interpréter un BattleEvent.
 */
enum class EventType {
    Text,           ///< Message générique.
    MoveUsed,       ///< Une attaque vient d'être utilisée.
    Damage,         ///< Des dégâts ont été infligés.
    Heal,           ///< Des PV ont été restaurés.
    Fainted,        ///< Un Pokémon est tombé KO.
    Switched,       ///< Un Pokémon actif a été remplacé.
    WeatherChanged, ///< La météo a changé.
    TerrainChanged, ///< Le terrain a changé.
    StageChanged,   ///< Un cran de statistique a changé.
    StatusChanged,  ///< Un statut persistant a été appliqué, retiré ou a empêché une action.
    BattleEnded     ///< Le combat est terminé.
};

/**
 * Représente une information produite pendant la résolution d'un combat.
 *
 * Entrées:
 *   type (EventType): Catégorie de l'événement.
 *   text (std::string): Message lisible associé.
 *   player (int): Joueur concerné, ou -1 si l'événement est global.
 *   value (int): Valeur numérique optionnelle, par exemple des dégâts.
 *
 * Sortie:
 *   BattleEvent : donnée consommable par une interface console, graphique ou des tests.
 */
struct BattleEvent {
    EventType type = EventType::Text; ///< Nature de l'événement.
    std::string text; ///< Message lisible destiné à l'interface.
    int player = -1; ///< Joueur concerné, ou -1 si aucun joueur précis.
    int value = 0; ///< Valeur numérique complémentaire lorsque nécessaire.
};

/**
 * Stocke les modifications temporaires de statistiques d'un Pokémon actif.
 *
 * Entrées:
 *   stats: Crans appliqués aux statistiques principales.
 *   accuracy: Crans appliqués à la précision et à l'esquive.
 *
 * Sortie:
 *   StatStages : état temporaire consulté par Battle et DamageCalculator.
 */
struct StatStages {
    std::array<int, static_cast<std::size_t>(Stat::Count)> stats{}; ///< Crans des statistiques principales.
    std::array<int, static_cast<std::size_t>(AccuracyStat::Count)> accuracy{}; ///< Crans de précision et d'esquive.
};

/**
 * Représente l'état complet et les règles d'un combat entre deux dresseurs.
 *
 * Entrées:
 *   Deux Trainer existants et, à chaque tour, une BattleAction pour chaque joueur.
 *
 * Sortie:
 *   Battle : moteur maintenant Pokémon actifs, boosts, météo, terrain et générateur aléatoire.
 *   resolveTurn() retourne les BattleEvent décrivant ce qui s'est produit.
 */
class Battle {
public:
    /**
     * Crée un combat entre deux dresseurs.
     *
     * Entrées:
     *   player1 (Trainer&): Premier dresseur.
     *   player2 (Trainer&): Second dresseur.
     *   seed (unsigned int): Graine du générateur aléatoire ; aléatoire par défaut.
     *
     * Sortie:
     *   Battle: Combat initialisé avec le premier Pokémon de chaque équipe comme actif.
     */
    Battle(Trainer& player1, Trainer& player2,
           unsigned int seed = std::random_device{}());

    /**
     * Retourne un dresseur avec accès en modification.
     *
     * Entrées:
     *   player (int): Index du joueur, 0 ou 1.
     *
     * Sortie:
     *   Trainer&: Référence vers le dresseur demandé.
     */
    Trainer& trainer(int player);

    /**
     * Retourne un dresseur en lecture seule.
     *
     * Entrées:
     *   player (int): Index du joueur, 0 ou 1.
     *
     * Sortie:
     *   const Trainer&: Référence constante vers le dresseur demandé.
     */
    const Trainer& trainer(int player) const;

    /**
     * Retourne le Pokémon actif d'un joueur avec accès en modification.
     *
     * Entrées:
     *   player (int): Index du joueur, 0 ou 1.
     *
     * Sortie:
     *   Pokemon*: Pointeur vers le Pokémon actif, ou nullptr si aucun Pokémon n'est disponible.
     */
    Pokemon* active(int player);

    /**
     * Retourne le Pokémon actif d'un joueur en lecture seule.
     *
     * Entrées:
     *   player (int): Index du joueur, 0 ou 1.
     *
     * Sortie:
     *   const Pokemon*: Pointeur constant vers le Pokémon actif, ou nullptr.
     */
    const Pokemon* active(int player) const;

    /**
     * Retourne la position du Pokémon actuellement actif dans l'équipe.
     *
     * Entrées:
     *   player (int): Index du joueur, 0 ou 1.
     *
     * Sortie:
     *   std::size_t: Index du Pokémon actif dans Trainer::team().
     */
    std::size_t activeIndex(int player) const;

    /**
     * Change le Pokémon actif d'un joueur.
     *
     * Entrées:
     *   player (int): Index du joueur concerné.
     *   index (std::size_t): Position du nouveau Pokémon dans l'équipe.
     *   events (std::vector<BattleEvent>*): Liste optionnelle recevant l'événement de switch.
     *
     * Sortie:
     *   bool: true si le remplacement a été effectué, false si le choix est invalide.
     *
     * Effets:
     *   Réinitialise les boosts temporaires du joueur lors d'un remplacement réussi.
     */
    bool switchPokemon(int player, std::size_t index,
                       std::vector<BattleEvent>* events = nullptr);

    /**
     * Indique si le combat est terminé.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   bool: true si au moins une des deux équipes n'a plus de Pokémon disponibles.
     */
    bool finished() const;

    /**
     * Détermine le gagnant du combat.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   int: 0 si le joueur 1 gagne, 1 si le joueur 2 gagne,
     *        -1 si le combat continue ou si aucun gagnant unique n'existe.
     */
    int winner() const;

    /** Retourne la météo active. Entrées: aucune. Sortie: Weather courant. */
    Weather weather() const;

    /** Retourne le terrain actif. Entrées: aucune. Sortie: Terrain courant. */
    Terrain terrain() const;

    /** Retourne la durée restante de la météo. Entrées: aucune. Sortie: nombre de tours restants. */
    int weatherTurns() const;

    /** Retourne la durée restante du terrain. Entrées: aucune. Sortie: nombre de tours restants. */
    int terrainTurns() const;

    /**
     * Retourne le cran actuel d'une statistique pour un joueur.
     *
     * Entrées:
     *   player (int): Index du joueur.
     *   stat (Stat): Statistique concernée.
     *
     * Sortie:
     *   int: Cran actuel, borné entre -6 et +6.
     */
    int stage(int player, Stat stat) const;

    /**
     * Retourne le cran actuel de précision ou d'esquive.
     *
     * Entrées:
     *   player (int): Index du joueur.
     *   stat (AccuracyStat): Précision ou esquive.
     *
     * Sortie:
     *   int: Cran actuel, borné entre -6 et +6.
     */
    int accuracyStage(int player, AccuracyStat stat) const;

    /**
     * Modifie le cran d'une statistique principale.
     *
     * Entrées:
     *   player (int): Index du joueur concerné.
     *   stat (Stat): Statistique à modifier.
     *   delta (int): Nombre de crans à ajouter ou retirer.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   La valeur finale est automatiquement limitée entre -6 et +6.
     */
    void changeStage(int player, Stat stat, int delta);

    /**
     * Modifie le cran de précision ou d'esquive.
     *
     * Entrées:
     *   player (int): Index du joueur concerné.
     *   stat (AccuracyStat): Précision ou esquive.
     *   delta (int): Nombre de crans à ajouter ou retirer.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   La valeur finale est automatiquement limitée entre -6 et +6.
     */
    void changeAccuracyStage(int player, AccuracyStat stat, int delta);

    /**
     * Convertit le cran d'une statistique principale en multiplicateur numérique.
     *
     * Entrées:
     *   player (int): Index du joueur.
     *   stat (Stat): Statistique concernée.
     *
     * Sortie:
     *   double: Multiplicateur utilisé dans les calculs de combat.
     */
    double statMultiplier(int player, Stat stat) const;

    /**
     * Convertit un cran de précision ou d'esquive en multiplicateur numérique.
     *
     * Entrées:
     *   player (int): Index du joueur.
     *   stat (AccuracyStat): Précision ou esquive.
     *
     * Sortie:
     *   double: Multiplicateur utilisé pour les tests de précision.
     */
    double accuracyMultiplier(int player, AccuracyStat stat) const;

    /**
     * Génère un entier pseudo-aléatoire dans un intervalle inclusif.
     *
     * Entrées:
     *   minInclusive (int): Borne minimale incluse.
     *   maxInclusive (int): Borne maximale incluse.
     *
     * Sortie:
     *   int: Valeur comprise entre les deux bornes.
     */
    int randomInt(int minInclusive, int maxInclusive);

    /**
     * Effectue un tirage de probabilité exprimé en pourcentage.
     *
     * Entrées:
     *   percent (int): Probabilité de réussite en pourcentage.
     *
     * Sortie:
     *   bool: true si le tirage réussit, sinon false.
     */
    bool rollPercent(int percent);

    /**
     * Retourne la définition spéciale de l'attaque Lutte.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const MoveData&: Définition partagée utilisée lorsqu'aucune attaque n'a de PP.
     */
    const MoveData& struggleMove() const;

    /**
     * Résout un tour complet à partir des actions des deux joueurs.
     *
     * Entrées:
     *   action1 (const BattleAction&): Action choisie par le premier joueur.
     *   action2 (const BattleAction&): Action choisie par le second joueur.
     *
     * Sortie:
     *   std::vector<BattleEvent>: Événements produits dans leur ordre d'exécution.
     *
     * Effets:
     *   Peut modifier les PV, PP, boosts, Pokémon actifs, météo, terrain et état du combat.
     */
    std::vector<BattleEvent> resolveTurn(const BattleAction& action1,
                                         const BattleAction& action2);

private:
    std::array<Trainer*, 2> trainers_; ///< Deux dresseurs participant au combat.
    std::array<std::size_t, 2> activeIndex_{0, 0}; ///< Index du Pokémon actif de chaque joueur.
    std::array<StatStages, 2> stages_{}; ///< Boosts temporaires des deux Pokémon actifs.
    Weather weather_ = Weather::None; ///< Météo actuelle.
    int weatherTurns_ = 0; ///< Nombre de tours de météo restants.
    Terrain terrain_ = Terrain::None; ///< Terrain actuel.
    int terrainTurns_ = 0; ///< Nombre de tours de terrain restants.
    std::mt19937 rng_; ///< Générateur pseudo-aléatoire propre au combat.

    /**
     * Réinitialise tous les boosts temporaires d'un joueur.
     *
     * Entrées:
     *   player (int): Index du joueur concerné.
     *
     * Sortie:
     *   Aucune.
     */
    void resetStages(int player);

    /**
     * Installe ou remplace la météo active.
     *
     * Entrées:
     *   weather (Weather): Nouvelle météo.
     *   turns (int): Durée en tours.
     *   events (std::vector<BattleEvent>&): Liste recevant l'événement correspondant.
     *
     * Sortie:
     *   Aucune.
     */
    void setWeather(Weather weather, int turns, std::vector<BattleEvent>& events);

    /**
     * Installe ou remplace le terrain actif.
     *
     * Entrées:
     *   terrain (Terrain): Nouveau terrain.
     *   turns (int): Durée en tours.
     *   events (std::vector<BattleEvent>&): Liste recevant l'événement correspondant.
     *
     * Sortie:
     *   Aucune.
     */
    void setTerrain(Terrain terrain, int turns, std::vector<BattleEvent>& events);

    /**
     * Applique les effets automatiques de fin de tour.
     *
     * Entrées:
     *   events (std::vector<BattleEvent>&): Liste recevant les événements produits.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Peut modifier PV, durées de météo/terrain et produire des KO.
     */
    void applyEndTurn(std::vector<BattleEvent>& events);

    /**
     * Résout l'utilisation d'une attaque par un joueur.
     *
     * Entrées:
     *   attackerPlayer (int): Index du joueur attaquant.
     *   slot (std::size_t): Slot d'attaque sélectionné.
     *   events (std::vector<BattleEvent>&): Liste recevant les événements générés.
     *
     * Sortie:
     *   Aucune.
     *
     * Effets:
     *   Vérifie les PP, applique dégâts et effets secondaires, puis met à jour le combat.
     */
    void resolveMove(int attackerPlayer, std::size_t slot,
                     std::vector<BattleEvent>& events);

    /**
     * Applique les effets secondaires déclarés dans MoveData.
     *
     * Entrées:
     *   attackerPlayer (int): Index du joueur attaquant.
     *   move (const MoveData&): Attaque dont les effets doivent être traités.
     *   attacker (Pokemon&): Pokémon lanceur.
     *   defender (Pokemon&): Pokémon ciblé.
     *   events (std::vector<BattleEvent>&): Liste recevant les événements générés.
     *
     * Sortie:
     *   Aucune.
     */
    void applyMoveEffects(int attackerPlayer, const MoveData& move,
                          Pokemon& attacker, Pokemon& defender,
                          std::vector<BattleEvent>& events);

    /**
     * Vérifie si un Pokémon peut agir malgré son statut persistant.
     *
     * Entrées:
     *   player (int): Joueur dont le Pokémon actif tente d'agir.
     *   events (std::vector<BattleEvent>&): Liste recevant les messages de statut.
     *
     * Sortie:
     *   bool: true si l'action peut continuer, false si le statut bloque le tour.
     */
    bool canAct(int player, std::vector<BattleEvent>& events);

    /**
     * Retourne la priorité effective d'une action offensive.
     *
     * Entrées:
     *   player (int): Joueur concerné.
     *   action (const BattleAction&): Action à examiner.
     *
     * Sortie:
     *   int: Priorité de l'attaque, ou 0 si l'action ne correspond pas à une attaque valide.
     */
    int movePriority(int player, const BattleAction& action) const;

    /**
     * Calcule la Vitesse utilisée pour départager deux attaques de même priorité.
     *
     * Entrées:
     *   player (int): Joueur concerné.
     *
     * Sortie:
     *   double: Vitesse après boosts et malus de paralysie.
     */
    double effectiveSpeed(int player) const;
};

} // namespace pokemon
