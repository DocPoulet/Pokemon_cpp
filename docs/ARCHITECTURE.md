# Architecture — Pokemon_cpp v0.5

La v0.5 conserve l'architecture du Battle Engine et ajoute les mécaniques sans les mélanger à l'interface. Les statuts, objets et talents sont représentés par `Mechanics.hpp`, tandis que `Battle` reste l'orchestrateur des tours et `DamageCalculator` le responsable des dégâts.

## Principe général

```text
               +------------------+
               |    ConsoleUI     |
               +---------+--------+
                         |
                    BattleAction
                         v
+-----------+     +------+-------+      +------------------+
| GreedyAI  | --> |    Battle    | ---> |   BattleEvent    |
+-----------+     +------+-------+      +------------------+
                         |
          +--------------+--------------+
          |                             |
          v                             v
+------------------+          +------------------+
| DamageCalculator |          | Trainer / Pokemon|
+------------------+          +------------------+
                                      |
                          +-----------+-----------+
                          |                       |
                          v                       v
                  PokemonSpecies             MoveInstance
                                                  |
                                                  v
                                              MoveData
```

## Responsabilités

### `Types`
Contient les enums fondamentaux. Aucun module métier ne doit redéfinir localement un type, une statistique, une météo ou un terrain.

### `PokemonSpecies`
Contient uniquement les données partagées et immuables d'une espèce : nom, statistiques de base et types.

### `Pokemon`
Contient l'état d'une instance : niveau, PV, IV, EV, nature et attaques équipées.

### `MoveData` / `MoveInstance`
`MoveData` décrit une attaque. `MoveInstance` représente cette attaque équipée par un Pokemon et possède ses PP courants. Cette séparation empêche deux Pokemon de partager accidentellement leurs PP.

### `Trainer`
Possède une équipe de six Pokemon maximum.

### `Battle`
Est la source de vérité du combat : Pokemon actifs, boosts, météo, terrain, RNG et résolution des actions. Il ne lit jamais `std::cin` et n'écrit jamais dans `std::cout`.

### `BattleAction`
Commande d'entrée du moteur. Une UI ou une IA produit une action sans modifier directement l'état du combat.

### `BattleEvent`
Résultat observable du moteur. Une UI peut afficher les événements sans connaître les détails internes du calcul.

### `DamageCalculator`
Centralise la table des types et la formule de dégâts. Le calcul est isolé afin d'être testable séparément.

### `BattleController` / `GreedyAI`
`BattleController` est une interface de décision. `GreedyAI` fournit une première implémentation simple.

### `ConsoleUI`
Seule couche responsable des entrées/sorties terminal. Une future UI SFML pourra remplacer cette couche sans réécrire le moteur.

### `GameData`
Catalogue temporaire codé en C++ pour les espèces et attaques disponibles. La migration vers JSON est prévue ultérieurement.

## Règles de dépendances

- L'UI dépend du moteur ; le moteur ne dépend pas de l'UI.
- Les données d'espèce ne dépendent pas d'une instance de Pokemon.
- Les définitions d'attaques ne stockent aucun état de PP courant.
- Le calcul des dégâts ne décide pas de l'affichage.
- Le RNG est détenu par `Battle` pour permettre des tests déterministes.

## Flux d'un tour

1. Chaque contrôleur construit un `BattleAction`.
2. `Battle::resolveTurn()` valide et ordonne les actions.
3. Les attaques sont résolues via `DamageCalculator`.
4. Les effets secondaires sont appliqués par `Battle`.
5. Les effets de fin de tour sont appliqués.
6. Une liste ordonnée de `BattleEvent` est renvoyée.
7. `ConsoleUI` traduit ces événements en texte.
