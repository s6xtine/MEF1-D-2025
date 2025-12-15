#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Pour la gestion des caractères
#include "noeud.h"
#include "AVL.h" // Inclusion pour utiliser les fonctions AVL
#include "histo.h" // Inclusion pour utiliser les fonctions d'histogramme

// Fonction pour comparer les identifiants en ordre alphabétique inverse
int comparerInverse(const void *a, const void *b) {
    const char *idA = *(const char **)a;
    const char *idB = *(const char **)b;
    return strcmp(idB, idA); // Ordre alphabétique inverse
}

int main() {
    int choix;
    printf("Quel histogramme voulez-vous générer ?\n");
    printf("1. Capacité maximale de traitement\n");
    printf("2. Volume total capté\n");
    printf("3. Volume total traité\n");
    printf("Entrez votre choix (1-3) : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("Génération de l'histogramme pour la capacité maximale...\n");
            histo_max("data/c-wildwater_v0.dat");
            break;
        case 2:
            printf("Génération de l'histogramme pour le volume capté...\n");
            histo_captation("data/c-wildwater_v0.dat");
            break;
        case 3:
            printf("Génération de l'histogramme pour le volume traité...\n");
            histo_traitement("data/c-wildwater_v0.dat");
            break;
        default:
            printf("Choix invalide. Veuillez entrer un nombre entre 1 et 3.\n");
            return EXIT_FAILURE;
    }

    printf("Histogramme généré avec succès. Consultez les fichiers de sortie correspondants.\n");
    return 0;
}