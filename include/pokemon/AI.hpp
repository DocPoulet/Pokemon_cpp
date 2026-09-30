#pragma once

#include "pokemon/Battle.hpp"

namespace pokemon {

/**
 * Interface commune aux contrôleurs capables de choisir une action de combat.
 *
 * Entrées:
 *   battle (const Battle&): État courant du combat.
 *   player (int): Index du joueur contrôlé, généralement 0 ou 1.
 *
 * Sortie:
 *   BattleAction : action à transmettre au moteur pour le prochain tour.
 */
class BattleController {
public:
    /**
     * Détruit un contrôleur via un pointeur ou une référence de classe de base.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Aucune.
     */
    virtual ~BattleController() = default;

    /**
     * Choisit l'action d'un joueur pour le tour courant.
     *
     * Entrées:
     *   battle (const Battle&): Combat consulté sans le modifier.
     *   player (int): Index du joueur pour lequel choisir une action.
     *
     * Sortie:
     *   BattleAction: Attaque, remplacement ou fuite selon l'implémentation.
     */
    virtual BattleAction chooseAction(const Battle& battle, int player) = 0;
};

/**
 * IA simple qui privilégie l'action offensive estimée la plus rentable.
 *
 * Entrées:
 *   battle (const Battle&): État actuel du combat.
 *   player (int): Joueur contrôlé par l'IA.
 *
 * Sortie:
 *   GreedyAI : contrôleur renvoyant une attaque ou un remplacement valide.
 */
class GreedyAI final : public BattleController {
public:
    /**
     * Choisit automatiquement l'action du joueur contrôlé.
     *
     * Entrées:
     *   battle (const Battle&): Combat à analyser.
     *   player (int): Index du joueur contrôlé.
     *
     * Sortie:
     *   BattleAction: Meilleure action trouvée par l'heuristique gloutonne.
     */
    BattleAction chooseAction(const Battle& battle, int player) override;
};


/**
 * IA de v0.6 capable de comparer attaque et remplacement.
 *
 * Entrées:
 *   battle (const Battle&): Etat courant du combat, équipes incluses.
 *   player (int): Joueur contrôlé.
 *
 * Sortie:
 *   TacticalAI: Contrôleur qui privilégie un KO, une attaque rentable ou un switch utile.
 */
class TacticalAI final : public BattleController {
public:
    /**
     * Choisit une action offensive ou un remplacement selon une heuristique simple.
     *
     * Entrées:
     *   battle (const Battle&): Combat à analyser sans le modifier.
     *   player (int): Index du joueur contrôlé.
     *
     * Sortie:
     *   BattleAction: Attaque ou switch considéré comme le plus intéressant.
     */
    BattleAction chooseAction(const Battle& battle, int player) override;
};

} // namespace pokemon
