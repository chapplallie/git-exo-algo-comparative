# Exercice 4 - Écrire un premier Makefile

## Fichiers

- [`Makefile`](Makefile) : mon premier Makefile, écrit règle par règle à partir du squelette. Comme les sources sont dans `seance-7/`, j'ai mis `../` devant les noms de fichiers. Il se lance depuis `exercice4/`.
- [`Makefile-espaces`](Makefile-espaces) : le même, mais avec 4 espaces au lieu de la tabulation sur la première commande (question B). Lancer avec `mingw32-make -f Makefile-espaces`.

Le Makefile « définitif », celui qui sert pour la suite du module, est à la racine de `seance-7/` (voir exercice 5).

Dépendances complétées : `main.o: main.c liste.h` et `liste.o: liste.c liste.h` (avec les `../` dans mon fichier).

## Ce que j'ai observé

`make clean` puis `make` :

```
rm -f main.o liste.o demo demo.exe
gcc -Wall -Wextra -std=c11 -g -c ../main.c
gcc -Wall -Wextra -std=c11 -g -c ../liste.c
gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o
```

On voit bien les 3 commandes : les deux compilations, puis le lien.

Deuxième `make` sans rien changer :

```
gcc -Wall -Wextra -std=c11 -g -o demo main.o liste.o
```

Ça ne recompile aucun `.o`, mais chez moi il refait quand même le lien à chaque fois. J'ai mis un moment à comprendre : sous Windows, gcc crée `demo.exe` et pas `demo`. Du coup, pour `make`, le fichier `demo` n'existe jamais, donc il relance la règle. Sous Linux il afficherait `make: 'demo' is up to date.`

## Questions

| Question | Réponse |
| --- | --- |
| A | `make` regarde les dates de modification des fichiers. Il ne refait une cible que si elle n'existe pas ou si une de ses dépendances est plus récente qu'elle. Après le premier `make`, les `.o` sont plus récents que les `.c` et `.h`, et l'exécutable est plus récent que les `.o`, donc il n'y a rien à refaire (sauf le lien chez moi, à cause du `.exe` expliqué au-dessus). |
| B (message exact) | `Makefile-espaces:5: *** missing separator.  Stop.` (ligne 5 dans mon fichier parce qu'il y a des commentaires au-dessus ; avec le squelette tel quel c'est `Makefile:2`). Les espaces et la tabulation se ressemblent à l'écran, mais `make` veut absolument une tabulation. J'ai vérifié avec `cat -A` : la tabulation s'affiche `^I`, les espaces non. |
