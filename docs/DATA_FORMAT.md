# Données JSON — v0.7.4

La v0.7.4 utilise un modèle **catalogue global + références**. Une attaque, un talent ou un objet n'est défini qu'une seule fois. Les autres fichiers stockent uniquement son identifiant canonique anglais.

## `data/moves.json`

Contient toutes les définitions d'attaques : `id`, type, catégorie, puissance, précision, PP, priorité, recul, drain, multi-coups et effets secondaires.

Exemple :

```json
{
  "id": "Energy Ball",
  "type": "Grass",
  "power": 90,
  "category": "Special",
  "accuracy": 100,
  "critical_bonus": 0,
  "pp": 10,
  "priority": 0,
  "recoil_percent": 0,
  "drain_percent": 0,
  "min_hits": 1,
  "max_hits": 1,
  "effects": [
    {"kind":"StatChange","target":"Opponent","stat":"SpecialDefense","stages":-1,"chance":10}
  ]
}
```

## `data/abilities.json`

Contient les talents globaux. `id` est l'identifiant métier ; `mechanic` indique au moteur quel comportement déjà implémenté appliquer.

```json
{"id":"Overgrow","mechanic":"Overgrow"}
```

## `data/items.json`

Même principe pour les objets tenus.

```json
{"id":"Life Orb","mechanic":"LifeOrb"}
```

## `data/pokemon_species.json`

Une espèce stocke ses stats/types et **des références par ID** vers les catalogues :

```json
{
  "name": "Bulbasaur",
  "base_stats": {"hp":45,"attack":49,"defense":49,"special_attack":65,"special_defense":65,"speed":45},
  "types": ["Grass", "Poison"],
  "move_pool": ["Vine Whip", "Energy Ball", "Body Slam"],
  "abilities": ["Overgrow"]
}
```

Au chargement, `GameData` transforme ces IDs en pointeurs vers les objets globaux. `PokemonSpecies` ne possède donc aucune copie de `MoveData` ou `AbilityData`.

## `data/battle_pokemon.json`

Un Pokémon prêt au combat stocke seulement les choix faits dans les listes autorisées :

```json
{
  "species": "Bulbasaur",
  "moves": ["Energy Ball", "Body Slam"],
  "ability": "Overgrow",
  "held_item": "Air Balloon"
}
```

`TeamBuilder` vérifie que :

- chaque move existe dans `moves.json` et appartient au movepool de l'espèce ;
- le talent existe dans `abilities.json` et appartient aux talents autorisés ;
- l'objet existe dans `items.json` ou vaut `None` ;
- les attaques choisies sont uniques.

Une fois validé, le `Pokemon` contient des références vers les mêmes objets globaux que son espèce et les autres Pokémon.

## Fallbacks

Si un catalogue global est absent ou illisible, `GameData` installe un catalogue minimal de secours afin que `MissingNo.` puisse toujours être construit. Les erreurs sont affichées sur `stderr` mais ne bloquent pas le programme.
