# Pokemon_cpp — v0.5 Mechanics

`Pokemon_cpp` est un moteur de combat Pokémon en C++17. La v0.5 enrichit le **Battle Engine** sans remettre en cause l'architecture introduite en v0.3 et documentée en v0.4.

## Nouveautés de la v0.5

- statuts persistants : brûlure, poison, paralysie, sommeil et gel ;
- ordre d'action basé sur la priorité puis la Vitesse ;
- attaques à recul générique ;
- attaques drainantes ;
- attaques multi-coups ;
- objets tenus : Restes et Orbe Vie ;
- talents : Brasier, Torrent, Engrais, Lévitation et Cran ;
- immunités de statut élémentaires ;
- dégâts de fin de tour pour brûlure et poison ;
- affichage console des statuts ;
- nouveaux événements de combat liés aux statuts ;
- nouveaux tests de non-régression et de mécanique.

## Structure

```text
Pokemon_cpp/
├── CMakeLists.txt
├── Doxyfile
├── README.md
├── docs/
│   ├── ARCHITECTURE.md
│   ├── DOCUMENTATION_GUIDE.md
│   └── ROADMAP.md
├── include/pokemon/
│   ├── AI.hpp
│   ├── Battle.hpp
│   ├── ConsoleUI.hpp
│   ├── DamageCalculator.hpp
│   ├── GameData.hpp
│   ├── Mechanics.hpp
│   ├── Move.hpp
│   ├── Nature.hpp
│   ├── Pokemon.hpp
│   ├── PokemonSpecies.hpp
│   ├── Trainer.hpp
│   └── Types.hpp
├── src/
│   └── ... implementations ...
└── tests/
    └── test_v05.cpp
```

## Architecture en une phrase

`ConsoleUI` et les IA produisent des `BattleAction`, `Battle` applique les règles et renvoie des `BattleEvent`, tandis que `DamageCalculator` isole la formule de dégâts et que `Mechanics.hpp` regroupe les statuts, objets et talents.

## Compiler

```bash
cmake -S . -B build
cmake --build build
```

## Lancer

Sous Linux, WSL ou macOS :

```bash
./build/pokemon_cpp
```

Sous Windows avec un générateur Visual Studio :

```powershell
.\build\Debug\pokemon_cpp.exe
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

## Documentation

La convention de documentation lisible de la v0.4 reste obligatoire : chaque nouvelle API explique son but, ses entrées, sa sortie et ses effets importants.

Si Doxygen est installé :

```bash
cmake --build build --target docs
```

La documentation générée se trouve dans `docs/generated/html/index.html`.

## Attaques de démonstration ajoutées

- `Vive-Attaque` : priorité +1 ;
- `Mega-Sangsue` : soigne 50 % des dégâts infligés ;
- `Belier` : inflige du recul ;
- `Combo-Griffe` : frappe de 2 à 5 fois ;
- `Feu Follet` : brûlure ;
- `Toxik` : poison simple dans cette version ;
- `Cage-Eclair` : paralysie ;
- `Poudre Dodo` : sommeil ;
- `Laser Glace` : 10 % de gel.

## Roadmap

- `v0.2` — Stabilisation
- `v0.3` — Battle Engine
- `v0.4` — Documentation
- **`v0.5` — Mechanics** ← version actuelle
- `v0.6` — Game
- `v1.0` — Pokemon Battle Simulator
