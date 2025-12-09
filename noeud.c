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

    // Initialisation de l'usine comme NULL par défaut
    n->usine = NULL;

    // Si un identifiant est fourni, créer une usine associée
    if (id) {
        n->usine = malloc(sizeof(Usine));
        if (!n->usine) {
            free(n);
            return NULL;
        }
        n->usine->id = strdup_safe(id);
        n->usine->capacite_max = 0.0;
        n->usine->volume_capte = 0.0;
        n->usine->volume_reel = 0.0;
    }

    n->fuite = fuite;
    n->volume_amont = 0.0;
    n->fg = NULL;
    n->fd = NULL;
    n->hauteur = 1;
    return n;
}

void libererNoeud(Noeud *racine) {
    if (!racine) return;

    // Libération récursive des sous-arbres
    libererNoeud(racine->fg);
    libererNoeud(racine->fd);

    // Libération de l'usine associée (si présente)
    if (racine->usine) {
        free(racine->usine->id);
        free(racine->usine);
    }

    free(racine);
}