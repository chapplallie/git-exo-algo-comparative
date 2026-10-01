# Exercice 5 - La dépendance au fichier d'en-tête

## Fichiers

- [`Makefile`](Makefile) : la version **fautive**, gardée pour l'étape 4 : `main.o: ../main.c`, sans `liste.h`. Se lance depuis `exercice5/`.
- Le Makefile complet de la fin de l'exercice (avec `CC`, `CFLAGS`, `OBJ`, `$@`, `$^`, `$<` et la règle `%.o: %.c liste.h`) est celui de la racine : [`../Makefile`](../Makefile). C'est lui qui sert pour la suite. J'ai juste ajouté `demo.exe` au `clean` parce que je suis sous Windows.

## Ce que make recompile

Manip : `make` une première fois, puis `touch liste.h`, puis `make`.

| Étape | Ce que make recompile |
| --- | --- |
| 2, avec la dépendance (Makefile de `seance-7/`) | `main.o` et `liste.o`, puis le lien : tout le projet |
| 4, sans la dépendance (Makefile de `exercice5/`) | seulement `liste.o`, puis le lien : `main.o` n'est pas recompilé |

Sortie de l'étape 4 :

```
gcc -Wall -Wextra -std=c11 -g -c ../liste.c
gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o
```

## Questions

| Question | Réponse |
| --- | --- |
| A | Avec la dépendance, toucher `liste.h` recompile les deux `.c` qui l'incluent. Sans elle, `make` ne sait pas que `main.o` dépend de `liste.h`, donc il garde le vieux `main.o`, compilé avec l'ancienne version du `.h`. J'ai compris que `make` ne lit pas le C : il ne voit pas les `#include`, il connaît seulement ce qu'on écrit dans le Makefile. |
| B | On obtient un exécutable à moitié faux. `liste.o` est recompilé avec la nouvelle structure (par exemple 24 octets avec un champ en plus), alors que `main.o` utilise encore l'ancienne (16 octets). Les deux parties du programme ne sont plus d'accord sur la taille de `Maillon` ni sur l'endroit où sont les champs. Et le pire, c'est que le lien marche quand même, parce que l'éditeur de liens vérifie seulement les noms des fonctions, pas les types. Résultat : comportement indéfini (mauvaises valeurs lues, plantage...) sans aucun message d'erreur. Ce genre de bug doit être horrible à trouver. |

Après l'étape 4, j'ai remis la dépendance et je suis passé au Makefile complet. J'ai vérifié qu'il donne exactement la même sortie que celui de l'exercice 4.
