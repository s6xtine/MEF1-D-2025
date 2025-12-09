#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noeud.h"

int main() {
    Noeud *racine = NULL;
    FILE *entree = fopen("data/c-wildwater_v0.dat", "r"); // Ouvre le fichier de données
    if (!entree) {
        perror("Erreur lors de l'ouverture du fichier de données");
        return EXIT_FAILURE;
    }

    FILE *sortie = fopen("sortie.txt", "w"); // Ouvre le fichier de sortie
    if (!sortie) {
        perror("Erreur lors de l'ouverture du fichier de sortie");
        fclose(entree);
        return EXIT_FAILURE;
    }

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), entree)) {
        char id[50];
        double fuite;
        if (sscanf(ligne, "-;%49[^;];%*[^;];%*d;%lf", id, &fuite) == 2) {
            Noeud *nouveau = creationNoeud(id, fuite);
            if (!racine) {
                racine = nouveau;
            } else {
                ajouterNoeud(racine, nouveau);
            }
        }
    }

    // Exemple de traitement : écrire les données dans le fichier de sortie
    fprintf(sortie, "ID\tVolume Amont\tFuite\n");
    Liste *enfant = racine->enfant;
    while (enfant) {
        fprintf(sortie, "%s\t%.2f\t%.2f\n", enfant->enfant->usine->id, enfant->enfant->volume_amont, enfant->enfant->fuite);
        enfant = enfant->suiv;
    }

    // Libération de la mémoire
    libererNoeud(racine);
    fclose(entree);
    fclose(sortie);

    return 0;
}