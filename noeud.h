#ifndef NOEUD_H
#define NOEUD_H


typedef struct Noeud Noeud;
typedef struct Liste Liste;


struct Liste {
Noeud *enfant;
Liste *suiv;
};


struct Noeud {
char *id;
double fuite; // pourcentage sur le tronçon parent->this
double volume_amont; // en milliers de m3 (k.m3)
Liste *enfant;
};


Noeud *creationNoeud(const char *id, double fuite);
void ajouterNoeud(Noeud *parent, Noeud *enfant);
void libererNoeud(Noeud *racine);


#endif // NODE_H