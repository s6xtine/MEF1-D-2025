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

void histo_src(const char *csv){







// Helper structure for aggregating by id (simple dynamic array)
typedef struct {
	char *id;
	long volume;
} AggEntry;

static int find_entry(AggEntry *arr, int n, const char *id) {
	for (int i = 0; i < n; ++i) {
		if (arr[i].id && strcmp(arr[i].id, id) == 0) return i;
	}
	return -1;
}

void histo_src(const char *csv){
	if (!csv) return;

	FILE *fp = fopen(csv, "r");
	if (!fp) {
		perror("fopen csv");
		return;
	}

	AggEntry *arr = NULL;
	int capacity = 0;
	int count = 0;

	char line[2048];
	while (fgets(line, sizeof(line), fp)) {
		char *saveptr = NULL;
		char *col1 = strtok_r(line, ";", &saveptr);
		char *col2 = strtok_r(NULL, ";", &saveptr);
		char *col3 = strtok_r(NULL, ";", &saveptr);
		char *col4 = strtok_r(NULL, ";", &saveptr);
		char *col5 = strtok_r(NULL, ";", &saveptr);

		if (!col2) continue; // need an identifier to attribute to

		// Heuristic: lines representing sources usually have a non '-' in col1
		// and a numeric value in col4 representing captured volume. We aggregate
		// col4 per usine id (col2) when col4 is present and col1 != "-".
		if (col4 == NULL) continue;
		// trim whitespace
		while (*col2 == ' ' || *col2 == '\t') ++col2;
		while (*col4 == ' ' || *col4 == '\t') ++col4;

		if (strcmp(col1 ? col1 : "", "-") == 0) continue;

		long vol = atol(col4);
		if (vol == 0) continue; // ignore zero volumes

		int idx = find_entry(arr, count, col2);
		if (idx >= 0) {
			arr[idx].volume += vol;
		} else {
			if (count + 1 > capacity) {
				int newcap = capacity == 0 ? 16 : capacity * 2;
				AggEntry *tmp = realloc(arr, newcap * sizeof(AggEntry));
				if (!tmp) break; // allocation failure -> stop
				arr = tmp;
				capacity = newcap;
			}
			arr[count].id = strdup(col2);
			arr[count].volume = vol;
			count++;
		}
	}

	fclose(fp);

	FILE *out = fopen("vol_src.dat", "w");
	if (!out) {
		perror("fopen vol_src.dat");
		for (int i = 0; i < count; ++i) free(arr[i].id);
		free(arr);
		return;
	}

	fprintf(out, "identifiant;volume_capte\n");
	for (int i = 0; i < count; ++i) {
		fprintf(out, "%s;%ld\n", arr[i].id, arr[i].volume);
	}

	fclose(out);

	for (int i = 0; i < count; ++i) free(arr[i].id);
	free(arr);
}

}