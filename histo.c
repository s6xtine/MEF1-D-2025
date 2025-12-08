#include "noeud.h"
#include "AVL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



void histo_max(const char *csv);
void histo_src(const char *csv); 
void histo_real(const char *csv);


/*
 * histo_max:
 *   Les séparateurs acceptés sont ',' ou ';' ou espace/tab (tcheker les \t\r\n ???)
 *   Implémentation permissive : ignore les lignes mal formées (peut etre changer ca ?)
*/

void histo_max(const char *csv) {
	if (csv == NULL){
        return;
    }

    FILE *entree = fopen(csv, "r");  //ouvre le fichier csv en lecture
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;  //initialise la racine de l'AVL des usines
	char ligne[1024];  //stocke les lignes lues du fichier

	while (fgets(ligne, sizeof(ligne), entree)){  //lecture du fichier ligne par ligne par ligne

		char *sauveptr = NULL;
        const char *delimitateurs = ",; \t\r\n";
		char *tok = strtok_r(ligne, delimitateurs, &sauveptr);
		if (tok == NULL){
            continue;
        } 
		int id = atoi(tok);

		tok = strtok_r(NULL, delimitateurs, &sauveptr);
		if (tok == NULL){
            continue;
        }
		long capacite = atol(tok);

		tok = strtok_r(NULL, delimitateurs, &sauveptr);
		if (tok == NULL){
            continue;
        }
		long conso = atol(tok);

		int h = 0;  //sert à l'équilibrage de l'AVL, hauteur initiale
		racine = insertionAVL(racine, id, &h, capacite, conso);
	}

	fclose(entree);

	FILE *sortie = fopen("max.dat", "w");  //ouverture du fichier max.dat en ecriture
	if (sortie == NULL) {
		perror("fopen max.dat");
		freeAVL(racine);
		return;
	}

	parcoursprefixe(racine, sortie);  //parcours l’AVL en préfixe et écrit les données dans max.dat
	fclose(sortie);

	freeAVL(racine);
}