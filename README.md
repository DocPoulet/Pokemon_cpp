# Pokemon_cpp — v0.2 Stabilisation

Petit simulateur de combat Pokémon en C++17.

## Ce que change la v0.2

Cette version se concentre sur la stabilité avant le gros refactor de la v0.3.

- correction de la source de vérité du Pokémon actif ;
- correction du switch du bot (`P2`, et non plus `P1`) ;
- fin de combat réellement prise en compte par la boucle principale ;
- `Lutte` devient un fallback sans modifier l'espèce du Pokémon ;
- les PP sont maintenant propres à chaque instance de Pokémon ;
- impossibilité de choisir volontairement une attaque à 0 PP ;
- validation plus robuste des entrées console ;
- `std::mt19937` remplace `rand()` ;
- correction de la probabilité des effets secondaires ;
- bonus Défense des Pokémon Glace appliqué uniquement sous neige ;
- stages remis à zéro lors d'un switch ;
- ajout de CMake ;
- ajout de warnings stricts ;
- ajout de premiers tests automatisés ;
- `main.cpp` séparé du moteur pour permettre les tests.

## Compiler

### Linux / macOS / MinGW

```bash
cmake -S . -B build
cmake --build build
```

Puis :

```bash
./build/pokemon_cpp
```

Sous Windows avec Visual Studio, l'exécutable se trouvera généralement dans `build/Debug/` ou `build/Release/`.

## Tests

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

Les tests couvrent notamment :

- le changement réel de Pokémon actif ;
- l'indépendance des PP entre deux Pokémon ;
- les limites IV/EV ;
- quelques interactions de types ;
- les limites de stages ;
- le clamp des PV ;
- la détection de fin de combat.

## Structure

```text
Pokemon_cpp_v0.2/
├── CMakeLists.txt
├── main.cpp
├── Workshop_POO_pt3.hpp
├── Workshop_POO_pt3.cpp
├── EffetsAttaques.hpp
├── EffetsAttaques.cpp
├── Liste_Attaques.hpp
├── Liste_Attaques.cpp
├── Pokedex_1g.hpp
├── Pokedex_1g.cpp
├── Terrain.hpp
├── Terrain.cpp
└── tests/
    └── test_v02.cpp
```

## Suite prévue

La v0.3 pourra ensuite découper `Workshop_POO_pt3` en composants dédiés (`Pokemon`, `Move`, `Battle`, `DamageCalculator`, etc.) sans devoir en même temps corriger les bugs fondamentaux de la v0.2.
