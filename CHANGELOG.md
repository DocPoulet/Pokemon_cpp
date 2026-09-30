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
- Les équipes sont désormais sauvegardées en `POKEMON_TEAM_V2` avec surnom + forme.
- Lecture conservée des anciennes sauvegardes `POKEMON_TEAM_V1`.
- Ajout des tests v0.7 de chargement JSON et de validation des presets.

# v0.6.0 - Game

- Ajout de `GameApp` et d'un menu principal.
- Ajout du Team Builder via `PokemonConfig` et `TeamBuilder`.
- Ajout des sauvegardes et chargements d'équipes avec `TeamIO` (`POKEMON_TEAM_V1`).
- Ajout des modes Joueur vs IA, Joueur vs Joueur et IA vs IA.
- Ajout de `TacticalAI`, capable d'envisager un remplacement.
- Catalogue enrichi : évolutions des starters, Pikachu et Evoli.
- `GameData` expose désormais les listes d'espèces/attaques et des méthodes `has*`.
- Finalisation des météos ajoutées sur GitHub : Mer Primaire, Terre Finale et Souffle Delta.
- Conservation des tests v0.5 et ajout des tests v0.6.
- Documentation du format de sauvegarde.

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
