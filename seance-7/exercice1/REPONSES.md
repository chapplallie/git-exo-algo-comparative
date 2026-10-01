# Exercice 1 - Découper la liste chaînée en trois fichiers

## Ce que j'ai fait

J'ai repris l'exercice 6 de `revisions.c` (le type `Maillon` et les 5 fonctions) et je l'ai coupé en trois :

| Fichier | Ce que j'ai mis dedans |
| --- | --- |
| [`liste.h`](../liste.h) | la garde `LISTE_H`, le `typedef struct Maillon`, les 5 prototypes |
| [`liste.c`](../liste.c) | le code des 5 fonctions, rien d'autre (pas de `main`) |
| [`main.c`](../main.c) | le programme de test qui utilise la liste |

J'ai renommé toutes les fonctions avec `liste_` devant (`liste_inserer`, `liste_longueur`, `liste_contient`, `liste_afficher`, `liste_liberer`) et j'ai enlevé les `static` qu'il y avait dans `revisions.c`, sinon `main.c` ne pourrait pas les appeler.

Les trois fichiers sont à la racine de `seance-7/` parce que c'est le code commun à tout le TP. Pour lancer : `make` puis `./demo` dans `seance-7/` (sous Windows c'est `mingw32-make`).

Pour les trous du squelette, j'ai mis :

```c
#ifndef LISTE_H
#define LISTE_H
...
bool     liste_contient(const Maillon *tete, int valeur);
void     liste_afficher(const Maillon *tete);
void     liste_liberer(Maillon *tete);

#endif /* LISTE_H */
```

et `#include "liste.h"` dans `liste.c` et dans `main.c`.

## Questions

| Question | Réponse |
| --- | --- |
| A | Parce que le compilateur compile chaque `.c` tout seul, sans regarder les autres. Quand il compile `main.c`, il ne sait pas ce qu'il y a dans `liste.c`. Or `main.c` utilise des `Maillon *`, donc il doit savoir ce que c'est. En mettant le type dans le `.h`, chaque `.c` qui en a besoin l'inclut et tout le monde a la même définition. Si `Maillon` était seulement dans `liste.c`, `main.c` ne compilerait pas (on le voit à l'exercice 3, cas 2 : `unknown type name 'Maillon'`). |
| B | Déjà parce que `liste.c` a besoin lui aussi du type `Maillon` et de `bool`. Mais surtout, ça permet au compilateur de vérifier que les prototypes du `.h` correspondent bien aux fonctions écrites dans le `.c`. Si je me trompe dans une signature (par exemple un `int` au lieu d'un `bool`), il me met une erreur `conflicting types`. Sans l'include, personne ne le verrait, même pas l'éditeur de liens qui ne regarde que les noms, et le programme planterait de façon bizarre. |
