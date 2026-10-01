# Exercice 3 - Provoquer les trois erreurs classiques

Dans les sous-dossiers il y a seulement les fichiers que j'ai cassés exprès. Le reste, c'est le code commun de `seance-7/`.

| Cas | Fichiers modifiés | Comment reproduire |
| --- | --- | --- |
| 1 | aucun, le code est bon, c'est la commande qui oublie `liste.o` | depuis `exercice3/` : `gcc -Wall -Wextra -c ../main.c` puis `gcc -o demo main.o` |
| 2 | [`cas2/main.c`](cas2/main.c) : j'ai enlevé `#include "liste.h"` | depuis `cas2/` : `gcc -Wall -Wextra -c main.c` |
| 3 | [`cas3/liste.h`](cas3/liste.h) : sans `#ifndef` / `#define` / `#endif` ; [`cas3/main.c`](cas3/main.c) : `#include "liste.h"` écrit 2 fois | depuis `cas3/` : `gcc -Wall -Wextra -std=c11 -c main.c` |

## Messages relevés (gcc 16.2, MinGW, Windows)

| Cas | Premier message exact | Compilation ou lien |
| --- | --- | --- |
| 1 | ``ld.exe: main.o:main.c:(.text+0x34): undefined reference to `liste_inserer'`` | Lien |
| 2 | `main.c:6:5: error: unknown type name 'Maillon'` | Compilation |
| 3 | `liste.h:3:16: error: redefinition of 'struct Maillon'` (avec `-std=c11`) | Compilation |

Ce que j'ai remarqué en plus :

- **Cas 1** : le même `undefined reference` revient pour `liste_afficher`, `liste_longueur`, `liste_contient` et `liste_liberer`, puis à la fin `collect2.exe: error: ld returned 1 exit status`.
- **Cas 2** : après le premier message il y en a plein d'autres : `implicit declaration of function 'liste_inserer'`, `assignment to 'int *' from 'int' makes pointer from integer without a cast`, puis une `implicit declaration` pour chaque autre fonction. Ce sont des erreurs et pas des warnings parce que gcc 16 est plus strict qu'avant.
- **Cas 3** : piège ! Sans `-std=c11` **ça compile sans erreur**. J'ai cru que j'avais raté la manip, mais en fait gcc 16 compile en C23 par défaut, et le C23 accepte qu'on redéfinisse une structure si elle est exactement pareille. Avec `-std=c11` (l'option qu'on met dans le Makefile) on a bien l'erreur, suivie de `conflicting types for 'Maillon'` et d'un `conflicting types` pour chaque prototype. La garde reste quand même obligatoire : si le `.h` contenait une variable, ou si les deux structures n'étaient pas identiques, ça casserait même en C23.

Après chaque cas j'ai remis le code comme avant et vérifié que `make` passait.

## Questions

| Question | Réponse |
| --- | --- |
| 1 | Le cas 1. On le voit parce que le message commence par `ld.exe` (c'est le nom de l'éditeur de liens) au lieu d'un nom de fichier `.c`, qu'il finit par `ld returned 1 exit status`, et qu'il n'y a pas de numéro de ligne mais une position dans le `.o` (`(.text+0x34)`). C'est logique : `main.c` a bien compilé, car il connaissait les prototypes grâce à `liste.h`. C'est seulement au moment de tout assembler qu'il manque le vrai code des fonctions, qui est dans `liste.o`. |
| 2 | Seulement le premier : `unknown type name 'Maillon'`. Tous les autres viennent de la même cause, le `#include` manquant : sans lui le compilateur ne connaît ni le type ni les fonctions. Donc la bonne méthode c'est de corriger la première erreur et de recompiler, souvent les autres disparaissent toutes seules. Ça m'aurait évité de paniquer devant 8 erreurs. |
| 3 | Quand un `.h` se retrouve inclus deux fois dans le même `.c` sans qu'on l'ait fait exprès, donc dès qu'il y a plusieurs modules qui s'incluent entre eux. Par exemple un module `pile.h` qui fait `#include "liste.h"` parce qu'il utilise `Maillon`, et un `main.c` qui inclut `pile.h` et aussi `liste.h` : `main.c` reçoit deux fois le contenu de `liste.h`. Avec seulement 3 fichiers ça n'arrive pas, mais c'est pour ça qu'on met toujours une garde dans chaque `.h`. |
