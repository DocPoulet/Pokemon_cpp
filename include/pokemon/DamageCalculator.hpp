#pragma once

#include "pokemon/Battle.hpp"

namespace pokemon {

/**
 * Regroupe les informations produites par un calcul de dégâts.
 *
 * Entrées:
 *   damage (int): Nombre final de PV à retirer.
 *   effectiveness (double): Multiplicateur de type appliqué.
 *   critical (bool): Indique si le coup est critique.
 *   hit (bool): Indique si l'attaque a touché sa cible.
 *
 * Sortie:
 *   DamageResult : résultat complet exploitable par le moteur et l'interface.
 */
struct DamageResult {
    int damage = 0; ///< Dégâts finaux infligés.
    double effectiveness = 1.0; ///< Multiplicateur d'efficacité des types.
    bool critical = false; ///< true si l'attaque est critique.
    bool hit = true; ///< false si l'attaque a échoué à cause de la précision.
};

/**
 * Fournit les calculs de dégâts et d'efficacité des types.
 *
 * Entrées:
 *   Pokémon attaquant et défenseur, attaque utilisée, état du combat et index des joueurs.
 *
 * Sortie:
 *   Valeurs numériques ou DamageResult sans stocker d'état interne persistant.
 */
class DamageCalculator {
public:
    /**
     * Calcule le multiplicateur d'efficacité d'un type offensif sur une espèce.
     *
     * Entrées:
     *   attackType (Type): Type de l'attaque.
     *   defender (const PokemonSpecies&): Espèce du Pokémon défenseur.
     *   weather (Weather): Météo actuellement active.
     *
     * Sortie:
     *   double: Multiplicateur final d'efficacité, par exemple 0.0, 0.5, 1.0, 2.0 ou 4.0.
     */
    static double effectiveness(Type attackType, const PokemonSpecies& defender,
                                Weather weather);

    /**
     * Calcule les dégâts théoriques avant précision, critique et variance finale.
     *
     * Entrées:
     *   attacker (const Pokemon&): Pokémon qui lance l'attaque.
     *   defender (const Pokemon&): Pokémon ciblé.
     *   move (const MoveData&): Attaque utilisée.
     *   battle (const Battle&): Combat fournissant boosts, météo et terrain.
     *   attackerPlayer (int): Index du joueur attaquant.
     *   defenderPlayer (int): Index du joueur défenseur.
     *
     * Sortie:
     *   double: Dégâts bruts avant les tirages aléatoires de fin de calcul.
     */
    static double rawDamage(const Pokemon& attacker, const Pokemon& defender,
                            const MoveData& move, const Battle& battle,
                            int attackerPlayer, int defenderPlayer);

    /**
     * Résout complètement une attaque offensive.
     *
     * Entrées:
     *   attacker (Pokemon&): Pokémon attaquant.
     *   defender (Pokemon&): Pokémon défenseur.
     *   move (const MoveData&): Attaque utilisée.
     *   battle (Battle&): Combat utilisé pour les boosts et les tirages aléatoires.
     *   attackerPlayer (int): Index du joueur attaquant.
     *   defenderPlayer (int): Index du joueur défenseur.
     *
     * Sortie:
     *   DamageResult: Résultat contenant réussite, critique, efficacité et dégâts finaux.
     */
    static DamageResult calculate(Pokemon& attacker, Pokemon& defender,
                                  const MoveData& move, Battle& battle,
                                  int attackerPlayer, int defenderPlayer);
};

} // namespace pokemon
