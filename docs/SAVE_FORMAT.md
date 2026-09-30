# Format d'équipe `POKEMON_TEAM_V2`

La v0.7 conserve le format texte des équipes afin de rester compatible avec les sauvegardes du Team Builder, mais ajoute les champs nécessaires au nouveau modèle.

Exemple :

```text
POKEMON_TEAM_V2
trainer=DocPoulet
count=1
pokemon=Salameche;Zippo;Base;50;Timide;5;1;2;1;31,31,31,31,31,31;0,0,0,252,0,252;Flammeche|Vive-Attaque|Feu Follet|Aiguisage
```

Une ligne `pokemon=` contient, dans l'ordre :

1. espèce ;
2. surnom, vide pour utiliser le nom de l'espèce ;
3. forme, actuellement `Base` ;
4. niveau ;
5. nom de la nature ;
6. statistique augmentée ;
7. statistique diminuée ;
8. objet tenu ;
9. talent ;
10. six IV ;
11. six EV ;
12. quatre slots d'attaque séparés par `|`.

Le lecteur accepte encore `POKEMON_TEAM_V1`. Lors du chargement d'une ancienne sauvegarde, le surnom devient vide et la forme devient `Base`. Si un ancien Pokémon avait `Ability::None`, le premier talent autorisé de son espèce est utilisé afin de respecter les règles v0.7.

Les fichiers `data/pokemon_species.json` et `data/battle_pokemon.json` ont un rôle différent : ils décrivent respectivement le catalogue global et les presets individuels fournis avec le jeu. Une sauvegarde `.team` reste une équipe créée par l'utilisateur.
