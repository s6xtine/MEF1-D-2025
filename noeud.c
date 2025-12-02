#include "noeud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static char *strdup_safe(const char *s) {
if (!s) return NULL;
size_t len = strlen(s) + 1;
char *r = malloc(len);
if (!r) return NULL;
memcpy(r, s, len);
return r;
}


Noeud *creationNoeud(const char *id, double fuite) {
Noeud *n = malloc(sizeof(Noeud));
if (!n) return NULL;
n->id = strdup_safe(id ? id : "");
n->fuite = fuite;
n->volume_amont = 0.0;
n->enfant = NULL;
return n;
}


void ajouterNoeud(Noeud *parent, Noeud *enfant) {
if (!parent || !enfant) return;
Liste *c = malloc(sizeof(Liste));
if (!c) return;
c->enfant = enfant;
c->suiv = parent->enfant;
parent->enfant = c;
}


void libererNoeud(Noeud *racine) {
if (!racine) return;
// free children subtrees
Liste *c = racine->enfant;
while (c) {
Liste *suiv = c->suiv;
libererNoeud(c->enfant);
free(c);
c = suiv;
}
free(racine->id);
free(racine);
}