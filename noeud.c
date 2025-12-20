#include "noeud.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>




Noeud *creationNoeud(const char *id, double fuite) {
    Noeud *nouv = malloc(sizeof(Noeud));
    if (!nouv) return NULL;

    // Initialisation des champs du Noeud
    nouv->volume_amont = 0.0;
    nouv->fuite = fuite;
    nouv->enfants = NULL;
    nouv->fg = NULL;
    nouv->fd = NULL;
    nouv->hauteur = 1;

    if (id != NULL) {
        nouv->usine = malloc(sizeof(Usine));
        if (nouv->usine) {
            //  Utilisation de ->id au lieu de ->id_usine
            nouv->usine->id = strdup(id); 
            nouv->usine->capacite_max = 0.0;
            nouv->usine->volume_capte = 0.0;
            nouv->usine->volume_reel = 0.0;
        }
    } else {
        nouv->usine = NULL;
    }

    return nouv;
}
void ajouterEnfant(Noeud* parent, Noeud* enfant) {
    if (!parent || !enfant) return;

    // On crée un maillon de la liste chaînée des enfants
    Fils* nouv_fils = malloc(sizeof(Fils));
    if (!nouv_fils) return;

    nouv_fils->enfant = enfant;
    
    // Insertion en tête de liste
    nouv_fils->suivant = parent->enfants;
    parent->enfants = nouv_fils;
}
// Pour libérer la liste des enfants (mais pas les noeuds eux-mêmes)
void libererEnfants(Fils *liste) {
    Fils *courant = liste;
    Fils *temp;
    while (courant != NULL) {
        temp = courant;
        courant = courant->suivant;
        // On libère le maillon de la liste, PAS le Noeud pointé par 'enfant'
        free(temp); 
    }
}

// Modification de libererNoeud pour libérer aussi la liste des enfants
void libererNoeud(Noeud *racine) {
    if (!racine) return;

    // Libération récursive des sous-arbres (pour la partie AVL d'indexation, si elle est utilisée)
    libererNoeud(racine->fg);
    libererNoeud(racine->fd);

    // Libération de la liste des enfants (Fils)
    libererEnfants(racine->enfants);

    // Libération de l'usine associée (si présente)
    if (racine->usine) {
        free(racine->usine->id);
        free(racine->usine);
    }

    free(racine);
}