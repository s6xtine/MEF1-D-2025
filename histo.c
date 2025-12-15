#include "noeud.h"
#include "AVL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//tcheker le nom des fichiers de sortie si ils sont bons ou pas
//tcheker si je dois return des -1 ??? voir dans le pdf de consignes
//histo_max: Implémentation permissive : ignore les lignes mal formées (peut etre changer ca ?) jsp si c'est tcheker
//tcheker les paramètres de insertion AVL : y'a un char* au lieu d'un int : strdup(col3) dans les 2 derniers histo
//tcheker si y'a une histoire de complexité

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


void histo_src(const char *csv){
	if(csv == NULL){
		return;
	}

	FILE *entree = fopen(csv, "r");  //ouvre le fichier csv en lecture
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;  //initialise la racine de l'AVL des usines
	int h = 0;  //sert à l'équilibrage de l'AVL, hauteur initiale
	char ligne[2048];  //stocke les lignes lues du fichier

	while (fgets(ligne, sizeof(ligne), entree)){  //lecture du fichier ligne par ligne

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

		//Source -> Usine
		if(strcmp(col1, "-") != 0 && strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0 && strcmp(col4, "-") != 0 && strcmp(col5, "-") != 0){
			long volume = atol(col4);

			racine = insertionAVL(racine, strdup(col3), volume, &h); //insertion/maj AVL
		}
	}
	fclose(entree);

	FILE *sortie = fopen("vol_source.dat", "w");  //ouverture du fichier vol_source.dat en ecriture
	if (sortie == NULL) {
		perror("fopen vol_source.dat");
		freeAVL(racine);
		return;
	}

	fprintf(sortie, "identifiant;source volume\n");

	parcoursInverse(racine, sortie);

	fclose(sortie);
	freeAVL(racine);
}

void histo_real(const char *csv){
	if(csv == NULL){
		return;
	}
	FILE *entree = fopen(csv, "r");  //ouvre le fichier csv en lecture
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;  //initialise la racine de l'AVL des usines
	int h = 0;  //sert à l'équilibrage de l'AVL, hauteur initiale
	char ligne[2048];  //stocke les lignes lues du fichier

	while(fgets(ligne, sizeof(ligne), entree)){  //lecture du fichier ligne par ligne

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

		//Source -> Usine
		if(strcmp(col1, "-") != 0 && strcmp(col2, "-") != 0 && strcmp(col3, "-") != 0 && strcmp(col4, "-") != 0 && strcmp(col5, "-") != 0){
			long volume = atol(col4);
			double fuite = atof(col5);

			long volume_reel = (long)(volume * (1.0 - fuite / 100.0)); //calcul du volume réel après fuite avec la formule : volume_reel = V × (1 − F / 100) (V :volume capté, F : fuite en pourcentage)

			racine = insertionAVL(racine, strdup(col3), &h, 0, volume_reel);
		}
	}
	fclose(entree);

	FILE *sortie = fopen("vol_real.dat", "w");
	if (sortie == NULL) {
		perror("fopen vol_real.dat");
		freeAVL(racine);
		return;
	}

	fprintf(sortie, "identifiant;real volume\n");
	parcoursInverse(racine, sortie);
	fclose(sortie);
	freeAVL(racine);
}