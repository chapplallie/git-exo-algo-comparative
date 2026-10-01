# Exercice 6 - Trouver une fuite sans aucun outil

## Fichiers modifiés

| Fichier | Modification par rapport à `seance-7/` |
| --- | --- |
| [`liste.c`](liste.c) | ajout du compteur `blocs` et de `suivi_malloc` / `suivi_free` (en `static`) ; `liste_inserer` appelle `suivi_malloc` et `liste_liberer` appelle `suivi_free` |
| [`liste.h`](liste.h) | ajout de `int liste_blocs_en_circulation(void);` |
| [`main.c`](main.c) | affiche le compteur après la construction et après la libération ; la fuite (2e liste de 3 éléments jamais libérée) est entre `#ifdef FUITE` / `#endif` |
| [`Makefile`](Makefile) | le même que celui de la racine, plus une cible `fuite` qui compile avec `-DFUITE` |

Pour lancer depuis `exercice6/` : `make` puis `./demo` (version corrigée), `make fuite` puis `./demo-fuite` (version avec la fuite).

J'ai mis la fuite dans un `#ifdef` pour garder les deux versions sans avoir à commenter et décommenter du code à chaque fois.

Squelette complété :

```c
if (p != NULL) blocs++;
if (p != NULL) { blocs--; free(p); }
return blocs;
```

## Mesures

| Mesure | Sans la fuite | Avec la fuite |
| --- | --- | --- |
| Compteur après construction | 5 | 5 (8 après la construction de la 2e liste) |
| Compteur après libération | 0 | 3 |

Sortie avec la fuite :

```
blocs apres construction : 5
blocs apres la 2e liste  : 8
liste     : 50 -> 40 -> 30 -> 20 -> 10 -> NULL
longueur  : 5
contient 30 : oui
liberee
blocs apres liberation   : 3
```

Le programme marche parfaitement à l'écran, il n'y a que le compteur qui montre qu'il manque 3 `free`.

## Questions

| Question | Réponse |
| --- | --- |
| A | `static` veut dire que c'est visible seulement dans `liste.c` (c'est l'encapsulation en C). Si `blocs` était dans le `.h`, n'importe quel fichier pourrait le modifier et fausser le compte. Et si `suivi_malloc` / `suivi_free` étaient publiques, quelqu'un pourrait allouer avec l'une et libérer avec un `free` normal, et le compteur serait faux. Le module donne seulement le droit de **lire** le compteur avec `liste_blocs_en_circulation()`, c'est lui qui garde le contrôle. Bonus : ça évite les conflits de noms si un autre module a aussi une variable `blocs`. |
| B | Sans le test dans `suivi_malloc`, un `malloc` raté (qui renvoie `NULL`) serait compté alors qu'aucun bloc n'existe : le compteur ne reviendrait jamais à 0, fausse alerte. Sans le test dans `suivi_free`, un `free(NULL)` (qui est permis et ne fait rien) ferait baisser le compteur : il pourrait devenir négatif, ou pire, cacher une vraie fuite (1 fuite + 1 `free(NULL)` = 0, et on croit que tout va bien). Avec les deux tests, le compteur suit exactement les blocs vraiment alloués. |
| C | Non, il dit **combien** de blocs manquent mais pas **où**. Pour trouver la fuite avec juste ça, j'afficherais le compteur à plusieurs endroits du programme : s'il vaut 0 à un endroit et 3 un peu plus loin, la fuite est entre les deux, et je resserre petit à petit (un peu comme une recherche dichotomique). Ou alors j'ajoute les `liste_liberer` un par un jusqu'à ce que le compteur final revienne à 0. Sur un petit programme ça va, sur un gros ça serait très long. |

Fuite corrigée : la version compilée par `make` (sans `-DFUITE`) n'a pas la 2e liste, et le compteur revient à 0.
