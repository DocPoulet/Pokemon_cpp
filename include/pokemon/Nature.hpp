#pragma once

#include "pokemon/Types.hpp"
#include <string>

namespace pokemon {

/**
 * Décrit la nature d'un Pokémon et son influence sur les statistiques.
 *
 * Entrées:
 *   name (std::string): Nom de la nature.
 *   increased (Stat): Statistique augmentée de 10 %.
 *   decreased (Stat): Statistique diminuée de 10 %.
 *
 * Sortie:
 *   Nature : objet capable de fournir le multiplicateur correspondant à une statistique.
 */
class Nature {
public:
    /**
     * Construit une nature.
     *
     * Entrées:
     *   name (std::string): Nom affiché de la nature.
     *   increased (Stat): Statistique favorisée.
     *   decreased (Stat): Statistique défavorisée.
     *
     * Sortie:
     *   Nature: Nature initialisée avec les trois valeurs fournies.
     */
    Nature(std::string name = "Hardi",
           Stat increased = Stat::Attack,
           Stat decreased = Stat::Attack);

    /**
     * Retourne le nom de la nature.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   const std::string&: Référence vers le nom stocké.
     */
    const std::string& name() const;

    /**
     * Retourne la statistique augmentée par la nature.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Stat: Statistique bénéficiant du bonus de 10 %.
     */
    Stat increased() const;

    /**
     * Retourne la statistique diminuée par la nature.
     *
     * Entrées:
     *   Aucune.
     *
     * Sortie:
     *   Stat: Statistique subissant le malus de 10 %.
     */
    Stat decreased() const;

    /**
     * Calcule le multiplicateur appliqué par la nature à une statistique.
     *
     * Entrées:
     *   stat (Stat): Statistique dont on veut connaître le modificateur.
     *
     * Sortie:
     *   double: 1.1 si la stat est augmentée, 0.9 si elle est diminuée, sinon 1.0.
     */
    double multiplier(Stat stat) const;

private:
    std::string name_;  ///< Nom de la nature.
    Stat increased_;    ///< Statistique augmentée.
    Stat decreased_;    ///< Statistique diminuée.
};

} // namespace pokemon
