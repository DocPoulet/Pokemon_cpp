# Pokemon_cpp — v0.7.1 Data-driven Pokemon

`Pokemon_cpp` est un moteur de combat Pokémon en C++17. La v0.7 déplace les données globales des espèces et les Pokémon prêts au combat hors du code C++.

## Nouveautés v0.7

- `data/pokemon_species.json` : espèces, stats de base, types, movepools et talents possibles ;
- `data/battle_pokemon.json` : Pokémon individuels prêts au combat ;
- surnom optionnel avec fallback automatique vers le nom de l'espèce ;
- champ `form` préparé pour les futures formes alternatives, Méga, Dynamax, etc. ;
- attaques uniques et limitées au movepool de l'espèce ;
- talent obligatoire et limité aux talents possibles de l'espèce ;
- `BattlePokemonData` pour charger et construire les presets ;
- sauvegardes d'équipe `POKEMON_TEAM_V2` conservant surnom et forme ;
- compatibilité de lecture avec `POKEMON_TEAM_V1` ;
- parseur JSON interne léger : aucune bibliothèque externe à installer.

## Organisation des données

```text
data/
  pokemon_species.json   données globales des espèces
  battle_pokemon.json    Pokémon individuels prêts au combat
```

Les attaques restent encore définies dans `GameData.cpp` en v0.7. Leur migration vers JSON pourra être faite séparément.

Voir `docs/DATA_FORMAT.md` pour le schéma détaillé et les règles de validation.

## Compiler

```bash
cmake -S . -B build
cmake --build build
```

## Lancer

```bash
./build/pokemon_cpp
```

## Tests

```bash
ctest --test-dir build --output-on-failure
```

La v0.7 conserve les tests v0.5 et v0.6 et ajoute une suite dédiée au chargement JSON et aux nouvelles contraintes de configuration.

## Fallback MissingNo.

`MissingNo.` est défini directement dans le code et ne dépend d'aucun JSON. Si une espèce, un preset ou une configuration de Pokémon est introuvable ou invalide, le jeu construit ce fallback au lieu d'arrêter le programme. Il est niveau 100, de types Vol/Normal, possède les stats de base `33 / 136 / 0 / 1 / 1 / 29`, aucun talent, aucun objet et quatre attaques distinctes tirées aléatoirement dans le catalogue disponible. Les erreurs de données restent affichées sur `stderr`.

Lorsque le catalogue JSON fonctionne, MissingNo. est caché de la liste normale du Team Builder. Si aucun catalogue d'espèces n'est disponible, il devient l'unique espèce proposée.
