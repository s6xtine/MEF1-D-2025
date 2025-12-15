#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noeud.h"
#include "AVL.h" // Inclusion pour utiliser les fonctions AVL
#include "histo.h" // Inclusion pour utiliser les fonctions d'histogramme

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



    // Libération de la mémoire
    libererNoeud(racine);
    fclose(entree);
    fclose(sortie);

    int choix;
    printf("Quel histogramme voulez-vous afficher ?\n");
    printf("1. Histogramme des 10 plus gros volumes\n");
    printf("2. Histogramme des 10 plus grandes fuites\n");
    printf("3. Histogramme personnalisé\n");
    printf("Entrez votre choix (1-3) : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("Génération de l'histogramme des 10 plus gros volumes...\n");
            histo_max("data/c-wildwater_v0.dat");
            break;
        case 2:
            printf("Génération de l'histogramme des 10 plus grandes fuites...\n");
            histo_fuites("data/c-wildwater_v0.dat");
            break;
        case 3:
            printf("Génération de l'histogramme personnalisé...\n");
            // Appeler une fonction pour un histogramme personnalisé si nécessaire
            break;
        default:
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
            return EXIT_FAILURE;
    }

    printf("Histogramme généré avec succès. Consultez les fichiers de sortie correspondants.\n");
    return 0;
}