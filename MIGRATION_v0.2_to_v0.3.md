# Migration v0.2 -> v0.3

## Correspondances principales

| v0.2 | v0.3 |
|---|---|
| `CreatureBase` | `PokemonSpecies` |
| `Creature` | `Pokemon` |
| `Attaque` globale | `MoveData` |
| copie d'attaque dans un slot | `MoveInstance` |
| `Joueur` | `Trainer` |
| `Combat` | `Battle` |
| `calculerDegatsPur` | `DamageCalculator::rawDamage` |
| `effectuerAttaque` | `Battle::resolveTurn` / `resolveMove` |
| `switchCombat` | `BattleAction{ActionType::Switch, ...}` |
| `attaqueAleatoire` | `GreedyAI::chooseAction` |
| `std::cout` dans le moteur | `BattleEvent` puis affichage par `ConsoleUI` |
| `Workshop_POO_pt3.hpp/.cpp` | fichiers specialises dans `include/pokemon/` et `src/` |

## Exemple : creer un Pokemon

```cpp
GameData data;
Pokemon salameche(&data.species("Salameche"), 50);
salameche.setMove(0, &data.move("Flammeche"));
```

## Exemple : creer un combat sans interface console

```cpp
Trainer p1("J1");
Trainer p2("J2");
p1.addPokemon(salameche);
p2.addPokemon(carapuce);

Battle battle(p1, p2, 42);
auto events = battle.resolveTurn(
    {ActionType::Move, 0},
    {ActionType::Move, 0}
);
```

Le moteur peut ainsi etre teste, branche a une future UI graphique, ou utilise pour des simulations IA vs IA sans `std::cin`/`std::cout`.
