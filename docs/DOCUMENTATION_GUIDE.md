# Convention de documentation lisible

La v0.4.3 utilise des blocs de documentation directement au-dessus des déclarations dans les headers.

L'objectif est double : pouvoir comprendre rapidement le code en ouvrant un `.hpp` et obtenir un hover utile dans VS Code lorsque l'IntelliSense C++ affiche les commentaires associés à une déclaration. Le format reste compatible avec Doxygen.

## Format demandé

Chaque classe, struct, enum, fonction ou méthode importante doit expliquer clairement :

- ce qu'elle représente ou ce qu'elle fait ;
- ses entrées et leur rôle ;
- sa sortie ou sa valeur de retour ;
- ses effets de bord lorsque la fonction modifie un objet ;
- les erreurs ou exceptions importantes lorsqu'il y en a.

Il n'est pas nécessaire de limiter la documentation à une seule ligne. La priorité est la lisibilité.

Exemple :

```cpp
/**
 * Change le Pokémon actif d'un joueur.
 *
 * Entrées:
 *   player (int): Index du joueur concerné.
 *   index (std::size_t): Position du nouveau Pokémon dans l'équipe.
 *
 * Sortie:
 *   bool: true si le remplacement a réussi, sinon false.
 *
 * Effets:
 *   Réinitialise les boosts temporaires du joueur lors d'un switch valide.
 */
bool switchPokemon(int player, std::size_t index);
```

Pour une classe ou une structure, les entrées décrivent les données principales qui la composent ou celles nécessaires à sa construction. La sortie décrit le rôle de l'objet produit.

Les fichiers `.cpp` ne recopient pas toute cette documentation. Ils conservent seulement les commentaires utiles pour expliquer un algorithme, une formule ou une décision d'implémentation non évidente.

## Documentation HTML

Ces commentaires sont compatibles avec Doxygen. Si Doxygen est installé :

```bash
cmake -S . -B build
cmake --build build --target docs
```

La documentation générée se trouve dans `docs/generated/html/index.html`.
