#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Fonctions pour la partie de l'arbre binaire equilibrer 
Arbre* creation(int s){ //creation d'un nouveau noeud dans l'arbre 
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
    return noeud;
}
int estVide(Arbre* racine){ //verifie si l'arbre a des fils ou est vide 
    if(racine == NULL){
        return 1;
    }
    return 0;
}
int existeGauche(Arbre* racine){ //verifie si le fils gauche existe 
    if(!estVide(racine)){
        if(racine->gauche != NULL){
            return 1;
        }
    }
    return 0;
}
int existeDroit(Arbre* racine){ //verifie si le fils droit exiiste 
    if(!estVide(racine)){
        if(racine->droit!=NULL){
            return 1;
        }
    }
    return 0;
}
int estFeuille(Arbre* racine){ //verifie si c'est une feuille 
    if(!estVide(racine)){
        if(!existeDroit(racine) && !existeGauche(racine)){
            return 1;
        }
    }
    return 0;
}
int min(int a, int b){
   return (a<b)? a : b;
}
int max(int a, int b){
   return (a>b)? a : b;
}
Arbre* rotationgauche(Arbre* a){ //rotation a gauche en cas de desequillibrage 
    Arbre* pivot;
    int eq_a;
    int eq_p;
    pivot=a->droit;
    a->droit=pivot->gauche;
    pivot->gauche=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-max(eq_p, 0)-1;
    pivot->equilibre = min( min(eq_a-2, eq_a+eq_p-2), eq_p-1 );
    a=pivot;
    return a;
}
Arbre* rotationdroite(Arbre* a){ //rotation a droite en cas de desequillibrage
    Arbre* pivot;
    int eq_a;
    int eq_p;
    pivot=a->gauche;
    a->gauche=pivot->droit;
    pivot->droit=a;
    eq_a=a->equilibre;
    eq_p=pivot->equilibre;
    a->equilibre=eq_a-min(eq_p, 0)+1;
    pivot->equilibre = max( max(eq_a+2, eq_a+eq_p+2), eq_p+1 );
    a=pivot;
    return a;
}
int hauteur(Arbre* racine){ //retourne la hauteur de l'arbre
    if(estVide(racine)){
        exit(1);
    }
    if(estFeuille(racine)){
        return 0;
    }
    return 1 + max(hauteur(racine->gauche), hauteur(racine->droit));
}
int facteur(Arbre* racine){ //retourne le facteur d'equilibre 
    if(!estVide(racine)){
        racine->equilibre = hauteur(racine->droit) - hauteur(racine->gauche);
    }
    return racine->equilibre;
}
Arbre* doublerotationdroite(Arbre* a){
    a->gauche=rotationgauche(a->gauche);
    return rotationdroite(a);
}
Arbre* doublerotationgauche(Arbre* a){
    a->droit=rotationdroite(a->droit);
    return rotationgauche(a);
}
Arbre* equilibrage(Arbre *a){    //reequilibre en cas de desequillibrage
    if(a->equilibre >= 2){
        if((a->droit->equilibre) >= 0){
            return rotationgauche(a);
        }
        else{
            return doublerotationgauche(a);
        }
    }
    else if (a->equilibre <= -2){
        if((a->gauche->equilibre) <= 0){
            return rotationdroite(a);
        }
        else{
            return doublerotationdroite(a);
        }
    }
    return a; 
}
Arbre* insertionAVL (Arbre* a, char* id_station ,int *h, long int capa, long int conso){ //insert un nouveau noeud et fait la somme
    if (a==NULL){
        *h=1;

        a = creation(id_station); //si l'arbre n'existe pas on cree un nouveau noeud
        a->capacite_total = capa;
        a->conso_total = conso;
        a->equilibre = 0; //equilibre initial
        a->gauche = a->droit = NULL;

        return a;
    }

    int cmp = strcmp(id_station, a->id_station);

    if(cmp == 0){ //id déjà existant
        a->capacite_total += capa; //si le noeud existe on fait la somme
        a->conso_total += conso;
        *h = 0; //on change pas la hauteur
        return a;
    }
    if(cmp < 0){ //inser à gauche
        a->gauche = insertionAVL(a->gauche, id_station, h, capa, conso);
        *h = -*h; //on inverse la hauteur
    }
    else{ //inser à droite
        a->droit = insertionAVL(a->droit, id_station, h, capa, conso);
    }

    if(*h != 0){ //maj de l'équilibre
        a->equilibre += *h;
        a = equilibrage(a);

        if(a->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return a;
}
void verificationalloc() { // alloue la place et verifie si l'allocation ait reussi A ou sinn message d'erreur
    Arbre* pnew=malloc(sizeof(Arbre));
    if(pnew==NULL) {
        printf("erreur d'allocation\n");
        exit(10);
    }
    free(pnew);
}
void parcoursprefixe(Arbre* a, FILE* fichier){ //fait un parcours prefixe sur le AVL
    if(a!=NULL){
    fprintf(fichier, "%d:%ld:%ld\n", a->id_station, a->capacite_total, a->conso_total);
    parcoursprefixe(a->gauche, fichier);
    parcoursprefixe(a->droit, fichier);
    }
}
// Fonction pour libérer récursivement l'arbre  
void freeAVL(Arbre *racine) { 
 	if (racine != NULL) { 
	 freeAVL(racine->gauche); // Libère le sous-arbre gauche 
	 freeAVL(racine->droit); // Libère le sous-arbre droit 
	 free(racine); // Libère le nœud courant 
	 } 
}

void parcoursInverse(Arbre* a, FILE* f){ //fait un parcours inverse sur le AVL
    if(a==NULL){
        return;
    }
    parcoursInverse(a->droit, f);
    fprintf(f, "%d:%ld:%ld\n", a->id_station, a->capacite_total, a->conso_total);
    parcoursInverse(a->gauche, f);
}