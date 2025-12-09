#include "noeud.h"
#include "AVL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


//faut que j'enleve ca a la fin mais belek
void histo_max(const char *csv);
void histo_src(const char *csv); 
void histo_real(const char *csv);


/*
 * histo_max:
 *   Les séparateurs acceptés sont ',' ou ';' ou espace/tab (tcheker les \t\r\n ???)
 *   Implémentation permissive : ignore les lignes mal formées (peut etre changer ca ?)
*/

void histo_max(const char *csv){
	if (csv == NULL){
        return;
    }

    FILE *entree = fopen(csv, "r");  //ouvre le fichier csv en lecture
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;  //initialise la racine de l'AVL des usines
	char ligne[2048];  //stocke les lignes lues du fichier
    int h = 0;  //sert à l'équilibrage de l'AVL, hauteur initiale

	while (fgets(ligne, sizeof(ligne), entree)){  //lecture du fichier ligne par ligne par ligne

		char *sauveptr = NULL;

        //colonnes du fichier csv
		char *col1 = strtok_r(ligne, ";", &sauveptr);
		char *col2 = strtok_r(NULL, ";", &sauveptr);
		char *col3 = strtok_r(NULL, ";", &sauveptr);
        char *col4 = strtok_r(NULL, ";", &sauveptr);
        char *col5 = strtok_r(NULL, ";", &sauveptr);

        if(col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL || col5 == NULL){
            continue; 
        }

        //
        if(strcmp(col1, "-") == 0 && strcmp(col3, "-") == 0 && strcmp(col5, "-") == 0){
            char *id = strdup(col2);
            if(id == NULL){
                continue;  
            }
            long capacite = atol(col4);
            racine = insertionAVL(racine, id, capacite, &h);
        }
	}

	fclose(entree);

	FILE *sortie = fopen("vol_max.dat", "w");  //ouverture du fichier vol_max.dat en ecriture
	if (sortie == NULL) {
		perror("fopen vol_max.dat");
		freeAVL(racine);
		return;
	}
    fprintf(sortie, "identifiant;max volume\n");

	parcoursInverse(racine, sortie);
	
    fclose(sortie);
	freeAVL(racine);
}
