#ifndef NOEUD_H
#define NOEUD_H

// Structure représentant une usine de traitement d'eau
// Une usine est optionnelle et contient :
// - Sa capacité maximale de traitement
// - Le volume capté et traité
typedef struct Usine {
    char *id;              // Identifiant unique de l'usine
    double capacite_max;   // Capacité maximale de traitement
    double volume_capte;   // Volume total capté
    double volume_reel;    // Volume réellement traité
} Usine;

// Structure représentant un noeud dans le réseau
// Un noeud peut contenir :
// - Une usine (optionnelle)
// - Un pourcentage de fuite
// - Le volume d'eau en amont
// - Les sous-arbres gauche et droit pour AVL
// - La hauteur pour AVL
typedef struct Noeud {
    Usine *usine;          // Usine associée (peut être NULL)
    double fuite;          // Pourcentage de fuite sur le tronçon parent -> ce noeud
    double volume_amont;   // Volume d'eau en amont (en milliers de m3)

    struct Noeud *fg;    // Sous-arbre gauche (AVL)
    struct Noeud *fd;   // Sous-arbre droit (AVL)
    int hauteur;
} Noeud;

// Fonctions associées aux noeuds
Noeud *creationNoeud(const char *id, double fuite);
void libererNoeud(Noeud *racine);

#endif // NOEUD_H