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
