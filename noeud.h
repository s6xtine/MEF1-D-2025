#ifndef NOEUD_H
#define NOEUD_H



typedef struct Usine {
    char *id;              // Identifiant unique de l'usine
    double capacite_max;   // Capacité maximale de traitement
    double volume_capte;   // Volume total capté
    double volume_reel;    // Volume réellement traité
} Usine;


typedef struct Noeud Noeud;

// Structure pour lier un Noeud à ses enfants (liste chaînée)
typedef struct Fils {
    Noeud *enfant;         // Pointeur vers le noeud enfant
    struct Fils *suivant;  // Pointeur vers le maillon suivant de la liste
} Fils;


// Structure représentant un noeud dans le réseau
// Les champs fg, fd, hauteur sont utilisés pour l'AVL d'indexation
struct Noeud {
    Usine *usine;          // Usine associée (peut être NULL)
    double fuite;          // Pourcentage de fuite sur le tronçon parent -> ce noeud
    double volume_amont;   // Volume d'eau en amont (en milliers de m3)
    
    Fils *enfants;         //  Liste des nœuds enfants (sorties)
    
    // Champs AVL pour l'indexation rapide
    Noeud *fg;             // Sous-arbre gauche (AVL)
    Noeud *fd;             // Sous-arbre droit (AVL)
    int hauteur;
};

// Fonctions associées aux noeuds
Noeud *creationNoeud(const char *id, double fuite);
void libererNoeud(Noeud *racine);

void ajouterEnfant(Noeud *parent, Noeud *enfant);

void libererEnfants(Fils *liste);

#endif // NOEUD_H