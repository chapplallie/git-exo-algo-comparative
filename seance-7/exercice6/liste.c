#include <stdio.h>
#include <stdlib.h>
#include "liste.h"

/* --- compteur d'allocations, interne au module --- */
/* static = visible seulement dans ce fichier, main.c ne peut pas y toucher */

static int blocs = 0;

static void *suivi_malloc(size_t taille)
{
    void *p = malloc(taille);
    if (p != NULL) blocs++;           /* on compte seulement si malloc a marche */
    return p;
}

static void suivi_free(void *p)
{
    if (p != NULL) { blocs--; free(p); }   /* free(NULL) ne libere rien, on ne decompte pas */
}

int liste_blocs_en_circulation(void)
{
    return blocs;
}

Maillon *liste_inserer(Maillon *tete, int valeur)
{
    Maillon *m = suivi_malloc(sizeof(Maillon));
    if (m == NULL) { perror("malloc"); exit(EXIT_FAILURE); }
    m->valeur  = valeur;
    m->suivant = tete;      /* 1. il pointe l'ancienne tete */
    return m;               /* 2. il devient la nouvelle tete */
}

int liste_longueur(const Maillon *tete)
{
    int n = 0;
    for (const Maillon *m = tete; m != NULL; m = m->suivant) n++;
    return n;
}

bool liste_contient(const Maillon *tete, int valeur)
{
    for (const Maillon *m = tete; m != NULL; m = m->suivant)
        if (m->valeur == valeur) return true;
    return false;
}

void liste_afficher(const Maillon *tete)
{
    for (const Maillon *m = tete; m != NULL; m = m->suivant)
        printf("%d -> ", m->valeur);
    printf("NULL\n");
}

void liste_liberer(Maillon *tete)
{
    Maillon *m = tete;
    while (m != NULL) {
        Maillon *suiv = m->suivant;  /* on garde le suivant AVANT de liberer */
        suivi_free(m);
        m = suiv;
    }
}
