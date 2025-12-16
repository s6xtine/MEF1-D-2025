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
    
    return 0;
}