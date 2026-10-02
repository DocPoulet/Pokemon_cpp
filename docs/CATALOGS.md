# Catalogues globaux — v0.8

La v0.8 sépare les **données connues** des **mécaniques déjà simulées**.

## Sources

Le snapshot embarqué est construit depuis les données officielles du projet Pokémon Showdown, utilisé par Smogon. Les pages Smogon servent de référence humaine ; le dépôt Showdown fournit les données structurées permettant une synchronisation reproductible.

Le snapshot v0.8 contient, CAP et entrées custom exclus :

- 951 attaques ;
- 317 talents ;
- 580 objets.

Les entrées `Past`, `Future`, `LGPE`, `Gmax` ou `Unobtainable` restent dans le catalogue avec leur marqueur `non_standard` afin de pouvoir lire des équipes Showdown historiques ou futures sans créer d'identifiant parallèle.

## Identifiants

Tous les identifiants métier sont les noms anglais canoniques Showdown :

```text
Earthquake
Primordial Sea
Choice Scarf
```

Le français ne doit être utilisé que par `Localization` au moment de l'affichage.

## Catalogue vs mécanique

Une entrée peut exister sans que son effet complet soit encore simulé.

- `GameData::hasAbility("Intimidate")` retourne `true`.
- `GameData::ability("Intimidate").implemented()` peut retourner `false`.
- le talent reste importable, exportable et référencable ; seul son effet de combat reste à implémenter.

Même principe pour les objets et les attaques complexes. Les champs de base d'une attaque (type, catégorie, puissance, précision, PP, priorité, recul, drain et multi-coups quand ils sont directement représentables) sont conservés dans le snapshot.

## Mise à jour

Le dépôt contient `tools/sync_showdown_catalogs.py`, sans dépendance Python externe.

```bash
python3 tools/sync_showdown_catalogs.py --output data
```

Avec CMake et Python disponibles :

```bash
cmake --build build --target sync_catalogs
```

Cette commande remplace `moves.json`, `abilities.json` et `items.json` par un snapshot frais des sources Showdown.

## Espèces Kanto

`pokemon_species.json` contient les 151 espèces du Pokédex national #001 à #151. Les données de base viennent du Pokédex Pokémon Showdown et les movepools de ses learnsets. Les variantes régionales, Méga et autres formes ne sont pas dupliquées ici : elles seront traitées plus tard via le système de formes.

Les noms stockés sont les IDs/noms anglais canoniques utilisés par le moteur. `Localization` fournit le nom français pour l'affichage. Les moves et talents présents dans une espèce sont uniquement des références vers `moves.json` et `abilities.json`.
