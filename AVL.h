#ifndef AVL_H
#define AVL_H
#include <stdio.h>
#include "noeud.h" // Inclusion de la structure Noeud pour utilisation dans AVL

typedef struct arbre{
    int id_station;
    struct arbre* gauche;
    struct arbre* droit;
    int equilibre;
    long int capacite_total;
    long int conso_total;
}Arbre;

Arbre* creation(int s);

int estVide(Arbre* racine);

int existeGauche(Arbre* racine);

int existeDroit(Arbre* racine);

int estFeuille(Arbre* racine);

int min(int a, int b);

int max(int a, int b);

Arbre* rotationgauche(Arbre* a);

Arbre* rotationdroite(Arbre* a);

int hauteur(Arbre* racine);

int facteur(Arbre* racine);

Arbre* doublerotationdroite(Arbre* a);

Arbre* doublerotationgauche(Arbre* a);

Arbre* equilibrage(Arbre *a);

Arbre* insertionAVL (Arbre* a, int id_station ,int *h, long int capa, long int conso);

void verificationalloc();

void parcoursprefixe(Arbre* a, FILE* fichier);

void freeAVL(Arbre *racine);

void parcoursInverse(Arbre *a, FILE *fichier);

#endif