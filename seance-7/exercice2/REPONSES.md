# Exercice 2 - Compiler en deux temps, à la main

Pas de fichier modifié dans cet exercice : on compile le code commun de `seance-7/` à la main, sans le Makefile.

## Commandes tapées (dans `seance-7/`)

```bash
gcc -Wall -Wextra -c main.c      # -> main.o
gcc -Wall -Wextra -c liste.c     # -> liste.o
ls *.o                           # main.o  liste.o
gcc -o demo main.o liste.o       # edition de liens -> demo.exe sous Windows
./demo
```

## Sortie du programme

```
liste     : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
longueur  : 5
contient 30 : oui
liberee
```

C'est bien la sortie attendue. Les valeurs sont à l'envers (50 en premier) parce que `liste_inserer` ajoute en tête.

## Questions

| Question | Réponse |
| --- | --- |
| A | J'obtiens 2 fichiers `.o` : `main.o` et `liste.o`. Il n'y a pas de `liste.h.o`, et au début je ne voyais pas pourquoi. En fait le `.h` n'est jamais compilé tout seul : le `#include` est fait par le préprocesseur, qui copie-colle le texte du `.h` dans chaque `.c` avant de compiler. Et de toute façon, il n'y a que des déclarations dans le `.h`, donc pas de code machine à produire. Un `.c` donne un `.o`, c'est tout. |
| B | Le résultat est le même exécutable, mais cette commande recompile tous les `.c` à chaque fois, même ceux que je n'ai pas touchés, et elle ne garde pas les `.o`. Avec 2 fichiers on ne voit pas la différence, mais sur un vrai projet avec des centaines de fichiers, changer une ligne obligerait à tout recompiler. En deux temps, on peut recompiler seulement le `.c` modifié puis refaire le lien. C'est justement ce que `make` va faire pour nous (exercice 4). |
