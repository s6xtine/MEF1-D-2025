#ifndef AVL_H
#define AVL_H
#include <stdio.h>
#include "noeud.h" 

typedef struct arbre{
    char* id_station;
    struct arbre* gauche;
    struct arbre* droit;
    int equilibre;
    
    // Champs pour les histogrammes
    long int capacite_total; 
    long int conso_total;    
    
    // Champ pour l'indexation du réseau (mode leaks)
    Noeud *ptr_noeud_reseau; // Pointeur vers la structure Noeud du réseau
    
}Arbre;




Arbre* insertionAVL (Arbre* a, char* id_station ,int *h, long int capa, long int conso, Noeud *noeud_a_indexer);

void verificationalloc();
void parcoursprefixe(Arbre* a, FILE* fichier);
void freeAVL(Arbre *racine);
void parcoursInverse(Arbre *a, FILE *fichier);


Arbre* rechercherArbre(Arbre* racine, const char* id); 
void libererReseauIndexe(Arbre *racine); 
#endif