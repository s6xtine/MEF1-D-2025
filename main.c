#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "histo.h" // Pour les fonctions histo_*

// Codes de retour d'erreur
#define ARGUMENT_ERROR 1
#define FILE_ERROR 2
#define MEMORY_ERROR 3

// Le comparateur n'est pas utilisé dans main.c, nous pouvons le supprimer ici ou le laisser
// si d'autres parties du code en ont besoin.

int main(int argc, char *argv[]) {
    
    // --- 1. Vérification du nombre minimal d'arguments ---
    if (argc < 3) {
        fprintf(stderr, "Erreur: Nombre d'arguments insuffisant.\n");
        fprintf(stderr, "Usage: %s <fichier_csv> <histo|leaks> [parametre]\n", argv[0]);
        return ARGUMENT_ERROR;
    }

    const char *csv_file = argv[1];
    const char *action = argv[2];

    // --- 2. Gérer l'action "histo" (Histogrammes) ---
    if (strcmp(action, "histo") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Erreur: Argument manquant pour l'action 'histo'.\n");
            fprintf(stderr, "Usage: %s <fichier_csv> histo <max|src|real>\n", argv[0]);
            return ARGUMENT_ERROR;
        }

        const char *histo_param = argv[3];

        if (strcmp(histo_param, "max") == 0) {
            histo_max(csv_file);
        } else if (strcmp(histo_param, "src") == 0) {
            histo_src(csv_file);
        } else if (strcmp(histo_param, "real") == 0) {
            histo_real(csv_file);
        } else {
            fprintf(stderr, "Erreur: Paramètre 'histo' inconnu: %s. Utilisez 'max', 'src', ou 'real'.\n", histo_param);
            return ARGUMENT_ERROR;
        }

    } 
    
    // --- 3. Gérer l'action "leaks" (Fuites) ---
    else if (strcmp(action, "leaks") == 0) {
        if (argc != 4) {
            fprintf(stderr, "Erreur: Identifiant d'usine manquant pour l'action 'leaks'.\n");
            fprintf(stderr, "Usage: %s <fichier_csv> leaks \"<ID Usine>\"\n", argv[0]);
            return ARGUMENT_ERROR;
        }
        
        const char *usine_id = argv[3];
        
        // La fonction histo_leaks gère le cas où l'ID n'est pas trouvé 
        // en retournant -1 dans le fichier de sortie, comme demandé[cite: 168, 193].
        histo_leaks(csv_file, usine_id);
        
    } 
    
    // --- 4. Action inconnue ---
    else {
        fprintf(stderr, "Erreur: Action inconnue: %s. Utilisez 'histo' ou 'leaks'.\n", action);
        return ARGUMENT_ERROR;
    }

    // Le programme C retourne 0 si l'exécution s'est déroulée normalement
    return 0;
}