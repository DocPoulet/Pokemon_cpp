# v0.8.1 - Kanto Species Catalog

- Ajout des **151 espèces de la première génération**, de Bulbasaur à Mew.
- Stats de base, types et talents possibles synchronisés depuis Pokémon Showdown.
- Movepools complets issus des learnsets Showdown, toutes générations référencées confondues.
- Les movepools et talents restent des références vers les catalogues globaux, sans duplication des définitions.
- Ajout de la traduction française des 151 noms dans `Localization`, sans changer les IDs moteur anglais.
- Compatibilité d'import pour `Farfetch'd` ASCII vers l'ID canonique `Farfetch’d`.
- Correction de l'import des talents Showdown à plusieurs mots (`Lightning Rod`, `Keen Eye`, etc.).
- `sync_showdown_catalogs.py` synchronise désormais aussi `pokemon_species.json` pour Kanto.
- Ajout des tests v0.8.1 de complétude, stats, learnsets, références globales et import Showdown.

# v0.8.0 - Complete Showdown Catalogs

- Ajout d'un snapshot de 951 attaques officielles Pokémon Showdown.
- Ajout d'un snapshot de 317 talents officiels.
- Ajout d'un snapshot de 580 objets officiels.
- Conservation des marqueurs `Past`, `Future`, `LGPE`, `Gmax` et `Unobtainable`.
- Séparation explicite entre entrée de catalogue et mécanique réellement simulée.
- Conservation des effets déjà implémentés dans les versions précédentes.
- Ajout de métadonnées de catalogue : numéro officiel, génération, statut non standard et description courte lorsque synchronisée.
- Ajout de `tools/sync_showdown_catalogs.py` pour régénérer les catalogues depuis Pokémon Showdown.
- Ajout de la cible CMake optionnelle `sync_catalogs`.
- Normalisation legacy de `PrimordialSea` vers l'ID canonique `Primordial Sea` sans créer de doublon.
- Ajout des tests v0.8 de complétude et de résolution des références globales.

# v0.7.5 - Showdown-only team persistence

- Suppression complète de l'ancien système propriétaire de sauvegarde d'équipe beta.
- Suppression de ses fichiers source, de sa documentation et de ses tests dédiés.
- Ajout de `ShowdownExporter` : le Team Builder sauvegarde directement au format Pokémon Showdown.
- `ShowdownImporter` sait maintenant charger directement un fichier Showdown.
- Le menu de chargement d'équipe utilise exclusivement les fichiers Showdown.
- Les tests de persistance utilisent des round-trips Showdown et vérifient la conservation des références catalogue.
- `battle_pokemon.json` reste un catalogue interne de presets et n'est pas un format de sauvegarde utilisateur.

# v0.7.4 - Global catalogs and references

- Ajout de `data/moves.json`, `data/abilities.json` et `data/items.json`.
- Les définitions normales des attaques ne sont plus codées en dur dans `GameData.cpp`.
- `PokemonSpecies` stocke des références vers les `MoveData` et `AbilityData` globaux.
- `Pokemon` stocke des références vers les `AbilityData` et `ItemData` globaux ; les `MoveInstance` référencent déjà un `MoveData` global.
- Ajout de `Catalog.hpp` avec `AbilityData` et `ItemData`.
- `GameData` expose les catalogues d'attaques, talents et objets ainsi que leurs méthodes `has*`/listes.
- Le Team Builder génère ses listes d'objets/talents directement depuis les catalogues.
- Ajout de tests vérifiant l'identité des pointeurs entre catalogue, espèce et Pokémon de combat.

# v0.7.3 - English canonical data / Showdown Dynamax metadata

- Canonicalisation des noms internes des Pokémon et attaques en anglais.
- Ajout d'une couche `Localization` pour afficher les noms français sans modifier les identifiants métier.
- Ajout de `dynamaxLevel` (10 par défaut) et `gigantamax` (false par défaut) à `PokemonConfig` et `Pokemon`.
- Import Showdown de `Dynamax Level:` et `Gigantamax:`.
- Migration des JSON d'espèces et presets vers les identifiants anglais.
- Compatibilité d'entrée avec plusieurs anciens noms français, normalisés immédiatement vers l'anglais.
- Ajout de tests v0.7.3.

# v0.7.2 - Pokemon Showdown import

- Ajout de `ShowdownImporter` pour importer un Pokémon ou une équipe complète depuis un export texte Pokémon Showdown.
- Les IV sont initialisés à 31 par défaut ; seules les valeurs de la ligne `IVs:` les remplacent.
- Les EV sont initialisés à 0 par défaut.
- Niveau Showdown par défaut : 100 lorsqu'aucune ligne `Level:` n'est présente.
- Lecture du surnom, espèce, sexe, objet, talent, EV, IV, nature, Shiny, Tera Type et des quatre attaques.
- Conservation de `gender`, `shiny` et `teraType` dans `PokemonConfig` puis dans `Pokemon`.
- Ajout du Ballon (`Air Balloon`) comme objet reconnu, sans effet mécanique pour l'instant.
- Ajout de Body Slam, Curse, Double-Edge et Energy Ball pour que l'exemple Bulbizarre soit directement jouable.
- Ajout d'alias anglais vers les noms internes français des espèces et attaques déjà présentes.
- Ajout d'une option de menu permettant de coller une équipe Showdown jusqu'à la ligne `END`.
- Ajout des tests `pokemon_v072_tests`.

# v0.7.1 - MissingNo fallback

- Ajout de `MissingNo.` en dur comme espèce de secours toujours disponible.
- Stats de base : 33 / 136 / 0 / 1 / 1 / 29, niveau 100, types Vol/Normal.
- MissingNo. n'a aucun talent ni objet et reçoit quatre attaques aléatoires distinctes du catalogue.
- Une espèce inconnue ou un Pokémon de combat invalide est remplacé par MissingNo. au lieu de bloquer le programme.
- Un `pokemon_species.json` absent ou invalide ne bloque plus le démarrage.
- Un `battle_pokemon.json` absent/invalide ou un preset inconnu retombe également sur MissingNo.
- Les erreurs restent signalées sur stderr pour faciliter le diagnostic.

# v0.7.1 - Data-driven Pokemon

- Migration des espèces globales vers `data/pokemon_species.json`.
- Ajout des stats de base, types, movepools et talents possibles dans le JSON d'espèces.
- Ajout de `data/battle_pokemon.json` pour les Pokémon individuels prêts au combat.
- Ajout des champs `nickname` et `form` dans `Pokemon` et `PokemonConfig`.
- `name()` utilise désormais le surnom lorsqu'il existe, sinon le nom de l'espèce.
- `form` est persisté mais n'a aucun effet mécanique dans cette version.
- Validation des attaques uniques et obligatoirement présentes dans le movepool.
- Validation du talent : non nul et autorisé par l'espèce.
- Ajout de `BattlePokemonData`, `JsonLite` et `DataCodec`.
- Ajout des talents de données `Static` et `Adaptability` pour Pikachu et Évoli.
- Ajout des tests v0.7 de chargement JSON et de validation des presets.

# v0.6.0 - Game

- Ajout de `GameApp` et d'un menu principal.
- Ajout du Team Builder via `PokemonConfig` et `TeamBuilder`.
- Ajout des modes Joueur vs IA, Joueur vs Joueur et IA vs IA.
- Ajout de `TacticalAI`, capable d'envisager un remplacement.
- Catalogue enrichi : évolutions des starters, Pikachu et Evoli.
- `GameData` expose désormais les listes d'espèces/attaques et des méthodes `has*`.
- Finalisation des météos ajoutées sur GitHub : Mer Primaire, Terre Finale et Souffle Delta.
- Conservation des tests v0.5 et ajout des tests v0.6.

# v0.5.0 - Mechanics

- Ajout des statuts persistants : brûlure, poison, paralysie, sommeil et gel.
- Ajout de la priorité des attaques avant comparaison de la Vitesse.
- Ajout du recul générique, du drain et des attaques multi-coups.
- Ajout des objets Restes et Orbe Vie.
- Ajout des talents Brasier, Torrent, Engrais, Lévitation et Cran.
- Ajout des immunités de statut élémentaires et des dégâts résiduels de fin de tour.
- Ajout de nouvelles attaques de démonstration pour chaque mécanique.
- Ajout de `Mechanics.hpp` / `Mechanics.cpp`.
- Ajout de tests v0.5 dédiés aux nouvelles règles.
- Conservation de la convention documentaire lisible introduite en v0.4.3.

# v0.4.3 - Documentation lisible

- Remplacement des mini-docs compactes par des blocs de documentation structurés.
- Chaque API décrit maintenant clairement son but, ses entrées et sa sortie.
- Ajout des sections `Effets` et `Erreurs` lorsque le comportement le nécessite.
- Documentation détaillée des champs importants des structs, enums et classes.
- Documentation conservée dans les headers pour favoriser le hover de VS Code.
- Aucun changement fonctionnel du moteur de combat.

# v0.4.2 - Mini-doc hover

- Remplacement des gros blocs Doxygen par des mini-docs `///` dans les headers.
- Chaque API indique maintenant son but, ses entrees et sa sortie ou son effet.
- Suppression des `@copydoc` et de la duplication documentaire dans les `.cpp`.
- Conservation de la compatibilite Doxygen pour generer une documentation HTML si souhaite.
- Aucun changement fonctionnel du moteur de combat.

# Changelog

## v0.4.0 — Documentation

Release non fonctionnelle : aucune nouvelle mécanique majeure n'est introduite.

### Ajouté

- documentation Doxygen de l'ensemble des headers publics ;
- documentation des classes, structs, enums, méthodes et membres importants ;
- en-têtes documentaires dans les fichiers d'implémentation ;
- `Doxyfile` ;
- cible CMake optionnelle `docs` ;
- `docs/ARCHITECTURE.md` ;
- `docs/DOCUMENTATION_GUIDE.md` ;
- `docs/ROADMAP.md` ;
- README v0.4 complet ;
- documentation des tests de non-régression.

### Modifié

- version CMake : `0.3.0` -> `0.4.0` ;
- test : `test_v03.cpp` -> `test_v04.cpp` ;
- l'ancienne v0.4 Mechanics devient v0.5 Mechanics.

### Compatibilité

Le comportement du moteur reste celui de la v0.3. Les tests de non-régression v0.4 reprennent les garanties de la v0.3.

## v0.4.1 - Documentation complete

- Ajout de blocs Doxygen au-dessus des definitions dans les fichiers `.cpp`.
- Utilisation de `@copydoc` pour garder les declarations et implementations synchronisees.
- Documentation explicite des helpers internes avec `@brief`, `@param` et `@return`.
- Conservation de la documentation detaillee dans les headers publics.
- Aucune modification fonctionnelle du moteur de combat.
