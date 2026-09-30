# Données JSON — v0.7

La v0.7 sépare désormais les données globales des espèces et les Pokémon déjà configurés pour le combat.

## `data/pokemon_species.json`

Ce fichier décrit une espèce, pas un Pokémon individuel.

Chaque entrée contient :

- `name` : nom canonique de l'espèce ;
- `base_stats` : HP, Attack, Defense, SpecialAttack, SpecialDefense, Speed ;
- `types` : un ou deux types ;
- `move_pool` : attaques que l'espèce est autorisée à sélectionner ;
- `abilities` : talents possibles de l'espèce.

Exemple :

```json
{
  "name": "Salameche",
  "base_stats": {
    "hp": 39,
    "attack": 52,
    "defense": 43,
    "special_attack": 60,
    "special_defense": 50,
    "speed": 65
  },
  "types": ["Fire"],
  "move_pool": ["Flammeche", "Griffe", "Vive-Attaque"],
  "abilities": ["Blaze"]
}
```

`GameData` vérifie au chargement que les attaques référencées existent, qu'il n'y a pas de doublons et qu'au moins un talent non nul est déclaré.

## `data/battle_pokemon.json`

Ce fichier contient des Pokémon individuels prêts au combat et réutilisables dans plusieurs équipes.

Chaque preset contient :

- `id` : identifiant unique du preset ;
- `species` : espèce globale ;
- `nickname` : surnom optionnel. Une chaîne vide utilise le nom de l'espèce ;
- `form` : identifiant de forme. En v0.7, `Base` est utilisé et ce champ n'a encore aucun effet mécanique ;
- `level` : niveau entre 1 et 100 ;
- `nature` : nom + statistique augmentée + statistique diminuée ;
- `ivs` : six IV entre 0 et 31 ;
- `evs` : six EV, avec un maximum de 252 par statistique et 510 au total ;
- `moves` : entre une et quatre attaques, sans doublon et obligatoirement dans le movepool ;
- `ability` : talent obligatoire et autorisé par l'espèce ;
- `held_item` : objet tenu, `None` étant autorisé.

Exemple :

```json
{
  "id": "quick_salameche",
  "species": "Salameche",
  "nickname": "",
  "form": "Base",
  "level": 50,
  "nature": {
    "name": "Timide",
    "increased": "Speed",
    "decreased": "Attack"
  },
  "ivs": {
    "hp": 31,
    "attack": 31,
    "defense": 31,
    "special_attack": 31,
    "special_defense": 31,
    "speed": 31
  },
  "evs": {
    "hp": 0,
    "attack": 0,
    "defense": 0,
    "special_attack": 252,
    "special_defense": 4,
    "speed": 252
  },
  "moves": ["Flammeche", "Vive-Attaque", "Feu Follet", "Aiguisage"],
  "ability": "Blaze",
  "held_item": "LifeOrb"
}
```

## Champ `form`

`form` est volontairement préparé avant l'implémentation des formes alternatives.

En v0.7 :

- la valeur est chargée ;
- elle est stockée dans `Pokemon` ;
- elle est sauvegardée/rechargée dans les équipes ;
- elle n'altère pas les statistiques, types, attaques, talents ou transformations.

Une future version pourra associer cet identifiant à des formes régionales, Méga-Évolutions, Primo-Résurgences, Dynamax/Gigamax ou autres transformations sans changer le format des presets.
