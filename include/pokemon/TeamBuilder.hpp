#pragma once

#include "pokemon/GameData.hpp"
#include "pokemon/Mechanics.hpp"
#include "pokemon/Nature.hpp"
#include "pokemon/Trainer.hpp"

#include <array>
#include <string>

namespace pokemon {

/**
 * Décrit complètement un Pokémon avant sa création dans une équipe.
 *
 * Entrées:
 *   species (std::string): Nom d'espèce présent dans GameData.
 *   nickname (std::string): Surnom optionnel ; vide signifie utiliser le nom de l'espèce.
 *   form (std::string): Identifiant de forme persisté pour un futur système de formes.
 *   level (int): Niveau souhaité, borné par Pokemon entre 1 et 100.
 *   nature (Nature): Nature utilisée pour le calcul des statistiques.
 *   ivs / evs: Valeurs des six statistiques dans l'ordre de Stat.
 *   moves: Jusqu'à quatre noms d'attaques ; une chaîne vide représente un slot vide.
 *   heldItem / ability: Objet tenu et talent du Pokémon.
 *
 * Sortie:
 *   PokemonConfig: Configuration validable et transformable en Pokemon par TeamBuilder.
 */
struct PokemonConfig {
    std::string species;
    std::string nickname;
    std::string form = "Base";
    int level = 50;
    Nature nature{};
    std::array<int, static_cast<std::size_t>(Stat::Count)> ivs{};
    std::array<int, static_cast<std::size_t>(Stat::Count)> evs{};
    std::array<std::string, 4> moves{};
    HeldItem heldItem = HeldItem::None;
    Ability ability = Ability::None;
};

/**
 * Construit des Pokémon et des équipes à partir de configurations indépendantes de l'UI.
 *
 * Entrées:
 *   GameData: Catalogue contenant les espèces et attaques référencées par les configurations.
 *   PokemonConfig: Données d'un Pokémon à construire.
 *
 * Sortie:
 *   Pokemon ou Trainer configuré. Une configuration invalide produit MissingNo. comme fallback.
 */
class TeamBuilder {
public:
    explicit TeamBuilder(const GameData& data);

    /**
     * Vérifie qu'une configuration peut être construite avec le catalogue actuel.
     *
     * Entrées:
     *   config (const PokemonConfig&): Configuration à contrôler.
     *
     * Sortie:
     *   std::string: Chaîne vide si elle est valide, sinon description de la première erreur.
     */
    std::string validate(const PokemonConfig& config) const;

    /**
     * Construit un Pokémon complet à partir d'une configuration valide.
     *
     * Entrées:
     *   config (const PokemonConfig&): Espèce, niveau, IV, EV, attaques, objet et talent.
     *
     * Sortie:
     *   Pokemon: Instance demandée si la configuration est valide ; sinon MissingNo.
     *
     * Effets:
     *   MissingNo. est niveau 100, sans talent, et reçoit jusqu’à quatre attaques aléatoires.
     */
    Pokemon buildPokemon(const PokemonConfig& config) const;

    /**
     * Ajoute un Pokémon configuré à une équipe.
     *
     * Entrées:
     *   trainer (Trainer&): Équipe cible.
     *   config (const PokemonConfig&): Pokémon à construire.
     *
     * Sortie:
     *   bool: true si le Pokémon a été ajouté, false si l'équipe est déjà pleine.
     */
    bool addPokemon(Trainer& trainer, const PokemonConfig& config) const;

private:
    const GameData& data_;
};

} // namespace pokemon
