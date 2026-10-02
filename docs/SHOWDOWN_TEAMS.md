# Équipes Pokémon Showdown

Pokémon Showdown est l'unique format de sauvegarde/import/export des équipes utilisateur du projet.

`ShowdownImporter` transforme le texte Showdown en `PokemonConfig` puis en `Trainer`. `ShowdownExporter` effectue le chemin inverse.

## Valeurs par défaut à l'import

- Niveau : `100`.
- IV : `31` dans les six statistiques.
- EV : `0` dans les six statistiques.
- Forme interne : `Base`.
- Shiny : `No`.
- Sexe : non précisé.
- Tera Type : non précisé.
- Dynamax Level : `10`.
- Gigantamax : `No`.

Une ligne `IVs:` ne remplace que les statistiques citées. `IVs: 0 Atk` produit donc `31 / 0 / 31 / 31 / 31 / 31`.

## Lignes reconnues

```text
Nickname (Species) (M/F) @ Item
Ability: Ability Name
Level: 50
EVs: 252 SpA / 4 SpD / 252 Spe
Timid Nature
IVs: 0 Atk
Shiny: Yes
Tera Type: Electric
Dynamax Level: 10
Gigantamax: Yes
- Move 1
- Move 2
- Move 3
- Move 4
```

Plusieurs blocs séparés par une ligne vide forment une équipe. Seuls les six premiers Pokémon sont conservés.

## Export

Le Team Builder exporte directement ce même format. Les valeurs égales aux valeurs par défaut sont omises quand Showdown permet de les déduire :

- `Level: 100` est omis ;
- une ligne d'IV est omise si tous les IV valent 31 ;
- une ligne d'EV est omise si tous les EV valent 0 ;
- `Dynamax Level: 10` est omis ;
- `Gigantamax: No` est omis ;
- `Shiny: No` est omis.

Le fichier produit peut être réimporté par `ShowdownImporter` sans passer par un autre format intermédiaire.

## Validation

Après le parsing, `TeamBuilder` vérifie :

- espèce existante ;
- talent autorisé par l'espèce ;
- attaque présente dans le movepool ;
- quatre attaques maximum sans doublon ;
- IV entre 0 et 31 ;
- EV entre 0 et 252 et total maximal de 510 ;
- niveau valide ;
- références d'objet/talent présentes dans les catalogues.

Une configuration invalide suit le fallback `MissingNo.` du projet.

## Métadonnées futures

`gender`, `shiny`, `teraType`, `dynamaxLevel` et `gigantamax` sont conservés dans `Pokemon`. Ils n'ont pas encore tous un effet mécanique.

Le champ interne `form` reste préparé pour les futures formes alternatives. Tant que les formes ne sont pas implémentées, sa valeur normale est `Base` et elle n'est pas ajoutée comme ligne propriétaire dans l'export Showdown.

## Identifiants canoniques

Les noms Showdown anglais sont les identifiants métier du moteur. La traduction française est appliquée uniquement lors de l'affichage en combat ou dans les menus.
