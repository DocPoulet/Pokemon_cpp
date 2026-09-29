# Pokemon_cpp — v0.3 Battle Engine

La v0.3 transforme le prototype v0.2 en un petit moteur de combat C++ separe de l'interface console.

## Architecture

```text
include/pokemon/
  Types.hpp            enums fortement types
  Nature.hpp           natures
  Move.hpp             MoveData + MoveInstance
  PokemonSpecies.hpp   donnees d'espece immuables
  Pokemon.hpp          instance d'un Pokemon
  Trainer.hpp          equipe d'un dresseur
  Battle.hpp           etat + actions + evenements du combat
  DamageCalculator.hpp calcul des degats
  AI.hpp                interface de controleur + GreedyAI
  GameData.hpp          catalogue de donnees
  ConsoleUI.hpp         interface console

src/
  ... implementations ...
  main.cpp

tests/
  test_v03.cpp
```

## Changements principaux depuis v0.2

- suppression de `Workshop_POO_pt3.hpp/.cpp` ;
- `enum class` pour les stats, types, categories, meteo, terrain ;
- separation `PokemonSpecies` / `Pokemon` ;
- separation `MoveData` / `MoveInstance` : les PP appartiennent a chaque Pokemon ;
- `BattleAction` (`Move`, `Switch`, `Run`) ;
- `BattleEvent` : le moteur ne depend plus de `std::cin` ou `std::cout` ;
- `DamageCalculator` separe du moteur ;
- interface `BattleController` et premiere `GreedyAI` ;
- `ConsoleUI` seule responsable des entrees/sorties terminal ;
- `GameData` centralise les attaques et especes actuellement disponibles ;
- effets d'attaques structures (`StatChange`, meteo, terrain, boost aleatoire) ;
- tests de regression + tests du moteur v0.3.

## Compiler

```bash
cmake -S . -B build
cmake --build build
```

## Lancer

Linux / WSL / macOS :

```bash
./build/pokemon_cpp
```

Windows avec un generateur Visual Studio :

```powershell
.\build\Debug\pokemon_cpp.exe
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

## Ce qui reste volontairement pour les versions suivantes

La v0.3 est surtout une release d'architecture. Il reste notamment a ajouter les statuts persistants (brulure, poison, paralysie...), davantage d'effets generiques, les talents, les objets complets, plus de Pokemon/attaques et le chargement depuis JSON.
