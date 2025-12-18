// AVL.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVL.h"

Arbre* creation(char* s){ //creation d'un nouveau noeud dans l'arbre 
    Arbre* noeud=malloc(sizeof(Arbre));
    if(noeud==NULL){
        exit(1);
    }
    noeud->id_station=s;
    noeud->gauche=NULL;
    noeud->droit=NULL;
    noeud->equilibre=0;
    noeud->capacite_total=0;
    noeud->conso_total=0;
    
    // NOUVEAU : Initialisation du pointeur de réseau
    noeud->ptr_noeud_reseau = NULL; 
    
    return noeud;
}

int max(int a, int b) { return a > b ? a : b; }
int min(int a, int b) { return a < b ? a : b; }

Arbre* rotationgauche(Arbre* a) {
    if (!a || !a->droit) return a;
    Arbre* pivot = a->droit;
    a->droit = pivot->gauche;
    pivot->gauche = a;

    // recalcul des équilibres
    a->equilibre = max(a->droit ? a->droit->equilibre : 0, a->gauche ? a->gauche->equilibre : 0) + 1;
    pivot->equilibre = max(pivot->droit ? pivot->droit->equilibre : 0, pivot->gauche ? pivot->gauche->equilibre : 0) + 1;
    return pivot;
}

Arbre* rotationdroite(Arbre* a) {
    if (!a || !a->gauche) return a;
    Arbre* pivot = a->gauche;
    a->gauche = pivot->droit;
    pivot->droit = a;

    a->equilibre = max(a->droit ? a->droit->equilibre : 0, a->gauche ? a->gauche->equilibre : 0) + 1;
    pivot->equilibre = max(pivot->droit ? pivot->droit->equilibre : 0, pivot->gauche ? pivot->gauche->equilibre : 0) + 1;
    return pivot;
}

Arbre* doublerotationgauche(Arbre* a) {
    if (!a) return NULL;
    a->droit = rotationdroite(a->droit);
    return rotationgauche(a);
}

Arbre* doublerotationdroite(Arbre* a) {
    if (!a) return NULL;
    a->gauche = rotationgauche(a->gauche);
    return rotationdroite(a);
}

Arbre* equilibrage(Arbre* a) {
    if (!a) return NULL;

    if (a->equilibre >= 2) {
        if (a->droit && a->droit->equilibre >= 0)
            return rotationgauche(a);
        else
            return doublerotationgauche(a);
    } else if (a->equilibre <= -2) {
        if (a->gauche && a->gauche->equilibre <= 0)
            return rotationdroite(a);
        else
            return doublerotationdroite(a);
    }
    return a;
}

Arbre* insertionAVL (Arbre* a, char* id_station ,int *h, long int capa, long int conso, Noeud *noeud_a_indexer){ 
    if (a==NULL){
        *h=1;

        a = creation(id_station);
        a->capacite_total = capa;
        a->conso_total = conso;
        a->ptr_noeud_reseau = noeud_a_indexer; // Attribution du pointeur si fourni
        a->equilibre = 0; 
        a->gauche = a->droit = NULL;

        return a;
    }

    int cmp = strcmp(id_station, a->id_station);

    if(cmp == 0){ // id déjà existant : Mise à jour
        // Mise à jour des valeurs pour HISTO
        a->capacite_total += capa; 
        a->conso_total += conso;
        
        // Mise à jour du pointeur si c'est une insertion de Nœud
        if (noeud_a_indexer != NULL) {
            a->ptr_noeud_reseau = noeud_a_indexer;
        }

        *h = 0;
        return a;
    }
    // ... (Le reste de la fonction d'insertion et d'équilibrage reste inchangé) ...
    // Note : Pensez à ajuster les appels récursifs pour inclure le nouveau paramètre !
    if(cmp < 0){ 
        a->gauche = insertionAVL(a->gauche, id_station, h, capa, conso, noeud_a_indexer); // ATTENTION : le paramètre doit être inclus
        *h = -*h; 
    }
    else{ 
        a->droit = insertionAVL(a->droit, id_station, h, capa, conso, noeud_a_indexer); // ATTENTION : le paramètre doit être inclus
    }
    
    // ... (Reste de la logique d'équilibrage inchangée) ...
    return a;
}
void parcoursInverse(Arbre* a, FILE* f) {
    if (!a) return;
    parcoursInverse(a->droit, f);
    fprintf(f, "%s;%ld\n", a->id_station, a->capacite_total + a->conso_total);    parcoursInverse(a->gauche, f);
}

Arbre* rechercherArbre(Arbre* racine, const char* id) {
    if (racine == NULL) return NULL;

    int cmp = strcmp(id, racine->id_station);

    if (cmp < 0) {
        return rechercherArbre(racine->gauche, id);
    } else if (cmp > 0) {
        return rechercherArbre(racine->droit, id);
    } else {
        return racine; // Trouvé
    }
}

void freeAVL(Arbre *racine) { 
    if (racine != NULL) { 
       freeAVL(racine->gauche); // Libère le sous-arbre gauche 
       freeAVL(racine->droit); // Libère le sous-arbre droit 
       
       // --- NOUVELLE LIGNE AJOUTÉE ---
       if (racine->id_station != NULL) { 
           free(racine->id_station); // Libère la chaîne de caractères allouée par strdup
       }
       // ------------------------------
       
       free(racine); // Libère le nœud courant 
    } 
}
// Dans AVL.c (vers la fin du fichier)
void libererReseauIndexe(Arbre *racine) {
    if (racine == NULL) return;
    
    // Parcours récursif pour couvrir tout l'AVL
    libererReseauIndexe(racine->gauche);
    libererReseauIndexe(racine->droit);
    
    // Si ce nœud AVL indexe une structure Noeud (acteur du réseau), on la libère.
    if (racine->ptr_noeud_reseau != NULL) {
        // libererNoeud gère la libération de l'Usine et de la liste Fils
        libererNoeud(racine->ptr_noeud_reseau);
    }
}