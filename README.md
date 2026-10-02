# Pokemon_cpp — v0.8.1 Kanto Species

`Pokemon_cpp` est un moteur de combat Pokémon en C++17. Les données métier utilisent des identifiants anglais canoniques et les équipes utilisateur utilisent désormais **un seul format : Pokémon Showdown**.

## v0.8.1 — 151 espèces de Kanto

Le catalogue des espèces contient maintenant les **151 Pokémon de la première génération**, de `Bulbasaur` à `Mew`. Pour chaque espèce :

- stats de base ;
- un ou deux types ;
- talents possibles ;
- movepool complet issu des learnsets Pokémon Showdown ;
- références directes vers les catalogues globaux de moves/talents ;
- nom français uniquement dans la couche d'affichage.

Les Méga-Évolutions, formes régionales et formes Gigamax ne sont pas ajoutées comme espèces séparées à ce stade : le champ `form` reste prévu pour ces mécaniques futures.

`tools/sync_showdown_catalogs.py` régénère maintenant aussi `data/pokemon_species.json`.

## v0.8 — Catalogues complets Showdown

La v0.8 embarque un snapshot des catalogues officiels Pokémon Showdown utilisés comme base structurée par Smogon :

- **951 attaques** ;
- **317 talents** ;
- **580 objets** ;
- identifiants anglais canoniques uniquement ;
- métadonnées Showdown `Past`, `Future`, `LGPE`, `Gmax`, `Unobtainable` conservées ;
- attaques : type, catégorie, puissance, précision, PP, priorité, recul, drain et multi-coups disponibles dans le catalogue ;
- talents/objets non encore simulés acceptés et conservés au lieu d'être rejetés ;
- `tools/sync_showdown_catalogs.py` permet de régénérer les trois catalogues depuis les sources Showdown.

Le catalogue et le moteur sont volontairement séparés : **présent dans le catalogue** ne signifie pas encore **effet spécial simulé à 100 %**. Les mécaniques déjà présentes dans les versions précédentes restent fonctionnelles.

```bash
python3 tools/sync_showdown_catalogs.py --output data
```

Ou, si Python a été détecté par CMake :

```bash
cmake --build build --target sync_catalogs
```

Voir `docs/CATALOGS.md` pour le détail.

## v0.7.5 — un seul format d'équipe

Le format propriétaire de sauvegarde utilisé pendant les versions beta a été supprimé avec son code de lecture/écriture.

Désormais :

- `ShowdownImporter` importe un set, une équipe collée ou un fichier Showdown ;
- `ShowdownExporter` exporte une équipe construite dans le jeu au format Showdown ;
- le Team Builder sauvegarde directement en `.txt` Showdown ;
- le menu de chargement lit directement un `.txt` Showdown ;
- les tests de round-trip utilisent tous `Showdown -> Trainer -> Showdown` ;
- il n'existe plus de second format de sauvegarde d'équipe à maintenir.

## Données globales

```text
data/
  moves.json             catalogue global des attaques
  abilities.json         catalogue global des talents
  items.json             catalogue global des objets
  pokemon_species.json   espèces + références vers moves/abilities
  battle_pokemon.json    presets internes prêts au combat
```

`battle_pokemon.json` n'est pas une sauvegarde utilisateur : c'est une bibliothèque interne de presets utilisée par le jeu.

Les attaques, talents et objets sont définis une seule fois dans les catalogues globaux. Les espèces et Pokémon de combat conservent des références vers ces définitions.

## Format Showdown

Exemple :

```text
Miguel (Bulbasaur) (M) @ Air Balloon
Ability: Overgrow
EVs: 100 HP / 156 Def / 172 SpA / 56 Spe
Quiet Nature
IVs: 0 Atk
Shiny: Yes
Tera Type: Electric
Dynamax Level: 7
Gigantamax: Yes
- Body Slam
- Curse
- Double-Edge
- Energy Ball
```

Valeurs par défaut :

- niveau : `100` ;
- IV : `31` partout ;
- EV : `0` partout ;
- `Dynamax Level` : `10` ;
- `Gigantamax` : `No` ;
- Shiny : `No` ;
- sexe et Tera Type : non précisés.

Les lignes absentes utilisant leur valeur par défaut peuvent être omises lors de l'export.

Voir `docs/SHOWDOWN_TEAMS.md` pour le détail.

## Identifiants internes

Le moteur utilise l'anglais comme source de vérité :

```text
Bulbasaur
Energy Ball
Overgrow
Air Balloon
Quiet
```

Le français est réservé à la couche d'affichage (`Localization`). Cela empêche d'avoir deux entrées métier équivalentes, par exemple `Ember` et `Flammèche`.

## Fallback MissingNo.

`MissingNo.` est défini en dur et ne dépend d'aucun JSON. Une espèce ou configuration invalide peut être remplacée par ce fallback afin que le programme continue de fonctionner. Il est niveau 100, Normal/Vol, avec les stats de base `33 / 136 / 0 / 1 / 1 / 29`, aucun talent et des attaques distinctes tirées dans le catalogue disponible.

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
