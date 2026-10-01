# Exercice 7 - Installer valgrind, et reprendre la même fuite

## Fichiers

Le code C ne change pas : on reprend celui de l'exercice 6 (compteur + fuite activable avec `-DFUITE`). J'ai seulement écrit un [`Makefile`](Makefile) qui va chercher les sources dans `../exercice6/` et qui ajoute des cibles pour lancer les outils :

| Cible | Ce qu'elle fait |
| --- | --- |
| `make valgrind` | compile la version sans fuite et lance `valgrind --leak-check=full ./demo` |
| `make valgrind-fuite` | pareil avec la 2e liste pas libérée |
| `make asan` / `make asan-fuite` | pareil avec `-fsanitize=address` à la place de valgrind |

`CFLAGS` contient bien `-g`, sinon le rapport ne donnerait pas les numéros de ligne.

## Le problème chez moi

Je n'ai pas réussi à faire tourner l'outil sur mon PC :

- valgrind n'existe pas sous Windows ;
- WSL ne démarre pas (`Code d'erreur : Wsl/Service/E_UNEXPECTED`) ;
- avec gcc MinGW, `-fsanitize=address` ne marche pas non plus : `cannot find -lasan`.

Donc les valeurs ci-dessous sont celles que j'attends sur Linux 64 bits, calculées à la main, pas mesurées. **À vérifier en salle de TP** avec `make valgrind` et `make valgrind-fuite`. Ce qui est vérifié chez moi, c'est que la version avec la fuite compile et que mon compteur affiche bien 3.

Pour le calcul : `sizeof(Maillon)` = 16 octets sur Linux 64 bits (un `int` de 4, 4 octets de remplissage pour aligner, puis un pointeur de 8).

## Mesures (attendues)

| Mesure | Sans la fuite | Avec la fuite |
| --- | --- | --- |
| `definitely lost` | 0 bytes in 0 blocks | 16 bytes in 1 blocks |
| `indirectly lost` | 0 bytes in 0 blocks | 32 bytes in 2 blocks |
| `total heap usage` | 6 allocs, 6 frees, 1,104 bytes allocated | 9 allocs, 6 frees, 1,152 bytes allocated |
| Mon compteur | 0 | 3 (mesuré) |

Sans fuite, les deux dernières lignes du rapport devraient être `All heap blocks were freed -- no leaks are possible` et `ERROR SUMMARY: 0 errors from 0 contexts`.

Pile d'appels attendue pour le bloc perdu (avec mes fichiers) :

```
by malloc
by suivi_malloc (liste.c:12)
by liste_inserer (liste.c:29)
by main (main.c:14)
```

## Questions

| Question | Réponse |
| --- | --- |
| A | La pile remonte le chemin de l'**allocation** : `malloc` ← `suivi_malloc` (`liste.c:12`) ← `liste_inserer` (`liste.c:29`) ← `main`. La ligne de `main.c` qui apparaît est la ligne 14 : `for (int i = 1; i <= 3; i++) autre = liste_inserer(autre, i);`, la boucle qui construit la 2e liste. Ce n'est pas là que j'ai oublié le `free`, et en fait ça ne peut pas l'être : un `free` oublié n'existe pas dans le code, donc il n'a pas de numéro de ligne. valgrind montre d'où vient le bloc perdu. La ligne fautive, c'est celle où il aurait fallu écrire `liste_liberer(autre);`, à la fin du `main` avant le `return 0`. |
| B | 1 bloc `definitely lost` et 2 blocs `indirectly lost`, ce qui fait bien les 3 de mon compteur. À la fin du `main`, la variable `autre` disparaît : plus rien ne pointe sur la tête de la 2e liste, donc elle est **définitivement** perdue. Les 2 maillons suivants sont encore pointés, mais seulement par le champ `suivant` d'un bloc qui est lui-même perdu, donc ils sont perdus **indirectement**. Ça montre qu'il suffit de corriger la cause directe : libérer la tête avec `liste_liberer` libère les 3 maillons d'un coup. |
| C | Sans fuite N = 6 et M = 6, avec la fuite N = 9 et M = 6. N n'est pas égal au nombre de maillons (5 ou 8) parce que valgrind compte **toutes** les allocations du programme, pas seulement les miennes. Au premier `printf`, la bibliothèque C alloue un tampon de 1 024 octets pour `stdout`, ça fait l'allocation en plus (5 × 16 + 1 024 = 1 104 octets, et 8 × 16 + 1 024 = 1 152). Ce tampon est libéré par la bibliothèque à la fin. Et N − M = 3, ce qui redonne exactement les 3 maillons non libérés. |

## Correspondance avec le désinfecteur d'adresses

Si j'utilise `make asan-fuite` au lieu de valgrind : `definitely lost` s'appelle `Direct leak` (16 octets, 1 objet) et `indirectly lost` s'appelle `Indirect leak` (32 octets, 2 objets). En plus, le programme sort avec le code 1 quand il y a une fuite, alors que valgrind laisse le code de sortie normal.

Fuite corrigée : la version normale (`make valgrind`, sans `-DFUITE`) libère tout, et mon compteur revient à 0.
