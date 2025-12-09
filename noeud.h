#ifndef NOEUD_H
#define NOEUD_H

typedef struct Noeud Noeud; // Forward declaration

typedef struct Liste {
    struct Noeud *enfant;
    struct Liste *suiv;
} Liste;

typedef struct Usine {
    char *id;
    double capacite_max; // capacité maximale de traitement (max)
    double volume_capte; // volume total capté par les sources (src)
    double volume_reel; // volume réellement traité (real)
} Usine;

typedef struct Noeud {
    Usine *usine;
    double fuite; // pourcentage sur le tronçon parent->this
    double volume_amont; // en milliers de m3 (k.m3)
    Liste *enfant;
} Noeud;

Noeud *creationNoeud(const char *id, double fuite);
void ajouterNoeud(Noeud *parent, Noeud *enfant);
void libererNoeud(Noeud *racine);

void histo_max(const char *csv);
void histo_src(const char *csv); 
void histo_real(const char *csv);

#endif // NODE_H