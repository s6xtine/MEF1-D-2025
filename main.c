#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "AVL.h"

int main(){
    Arbre* racine = NULL;
    FILE* sortie = fopen("sortie.txt","w"); //ouverture du fichier de sortie trier par le shell
    if(sortie==NULL){
        exit(1);
    }
    int id;
    long int capa, conso;
    int h=-1;
    while(scanf("%d:%ld:%ld\n", &id, &capa, &conso)==3){
        racine=insertionAVL(racine, id, &h, capa, conso); //fonction qui insert et calcule la somme
        //la somme est calculée dans la fonction insertionAVL
    }
    parcoursprefixe(racine, sortie); //appel de la fonction qui fait un parcours prefixe
    freeAVL(racine);
    
    return 0;
}