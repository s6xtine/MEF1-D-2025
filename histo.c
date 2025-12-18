#include "noeud.h"
#include "AVL.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h> // Pour utiliser round() ou pour les calculs float
#include "histo.h" // Assurez-vous d'inclure votre propre .h

// --- Fonction récursive pour le mode 'leaks' ---

// Fonction récursive pour propager l'eau dans le réseau en aval et sommer les fuites
// Retourne la fuite totale générée sur ce tronçon et tous ses tronçons en aval (en k.m³).
static double calculerFuitesRecursif(Noeud *noeud, double volume_entrant) {
    if (noeud == NULL) return 0.0;

    // Le pourcentage de fuite est celui du TRONÇON AMONT -> NOEUD
    double fuite_pct = noeud->fuite; 

    // 1. Calcul de la fuite absolue sur le tronçon menant à ce nœud (en k.m³)
    double fuite_absolue = volume_entrant * (fuite_pct / 100.0);
    double volume_sortant = volume_entrant - fuite_absolue;
    
    // 2. Initialisation de la somme des fuites en aval
    double fuites_aval = 0.0;

    // 3. Répartition et propagation vers les enfants
    Fils *courant = noeud->enfants;
    int nombre_enfants = 0;
    
    // Compter le nombre d'enfants
    while(courant != NULL) {
        nombre_enfants++;
        courant = courant->suivant;
    }
    
    // Si des enfants existent, on répartit le volume sortant
    if (nombre_enfants > 0) {
        // Correction de l'erreur d'identifiant non déclaré (volume_pour_enfant)
        // La quantité d'eau qui le traverse est répartie équitablement par les nœuds enfants.
        double volume_pour_enfant = volume_sortant / nombre_enfants; 
        
        courant = noeud->enfants;
        while(courant != NULL) {
            // Appel récursif pour chaque enfant.
            fuites_aval += calculerFuitesRecursif(courant->enfant, volume_pour_enfant);
            courant = courant->suivant;
        }
    }

    // Le résultat total est la fuite de ce tronçon + les fuites en aval
    return fuite_absolue + fuites_aval;
}

// --- Fonctions d'Histogramme ---

void histo_max(const char *csv){
	if (csv == NULL){
        return;
    }

    FILE *entree = fopen(csv, "r");
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;
	char ligne[2048];
    int h = 0;

	// En-tête pour la sortie CSV
	const char *HEADER = "identifier;max volume (k.m³.year⁻¹)\n";
    
	while (fgets(ligne, sizeof(ligne), entree)){
		char *sauveptr = NULL;

		char *col1 = strtok_r(ligne, ";", &sauveptr);
		char *col2 = strtok_r(NULL, ";", &sauveptr); // ID Usine
		char *col3 = strtok_r(NULL, ";", &sauveptr);
        char *col4 = strtok_r(NULL, ";", &sauveptr); // Capacité Max
        char *col5 = strtok_r(NULL, ";", &sauveptr);

        if(col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL || col5 == NULL){
            continue; 
        }

        // Ligne USINE (décrivant le noeud) : - ; [ID Usine] ; - ; [Capacité] ; -
        if(strcmp(col1, "-") == 0 && strcmp(col3, "-") == 0) {
            
            long capacite = atol(col4); // En milliers de m³ (k.m³)

            if (capacite > 0) {
                char *id = strdup(col2); // Allocation de l'identifiant
                if(id == NULL){
                    continue; // Erreur d'allocation
                }
                
                // Insertion dans l'AVL : capacite est dans capa, pas d'indexation (NULL)
                racine = insertionAVL(racine, id, &h, capacite, 0, NULL );
            }
        }
	}

	fclose(entree);

	FILE *sortie = fopen("vol_max.dat", "w");
	if (sortie == NULL) {
		perror("fopen vol_max.dat");
		freeAVL(racine);
		return;
	}
    fprintf(sortie, "%s", HEADER);

	// Le parcours inverse réalise le tri alphabétique inverse
	parcoursInverse(racine, sortie);
	
    fclose(sortie);
	freeAVL(racine);
}


void histo_src(const char *csv){
	if(csv == NULL){
		return;
	}

	FILE *entree = fopen(csv, "r");
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;
	int h = 0;
	char ligne[2048];
    
	const char *HEADER = "identifier;source volume (k.m³.year⁻¹)\n";

	while (fgets(ligne, sizeof(ligne), entree)){
		char *sauveptr = NULL;

		char *col1 = strtok_r(ligne, ";", &sauveptr);
		char *col2 = strtok_r(NULL, ";", &sauveptr); // ID Source
		char *col3 = strtok_r(NULL, ";", &sauveptr); // ID Usine
		char *col4 = strtok_r(NULL, ";", &sauveptr); // Volume Capté
		char *col5 = strtok_r(NULL, ";", &sauveptr); // Fuite

		if(col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL || col5 == NULL){
			continue; 
		}
		if (col2 == NULL || strcmp(col2, "-") == 0) {
			continue; // On ignore les lignes sans identifiant valide
		}

		// Ligne SOURCE -> USINE : - ; [ID Source] ; [ID Usine] ; [Volume] ; [Fuite]
		if(strcmp(col1, "-") == 0 && strcmp(col4, "-") != 0 && strcmp(col5, "-") != 0){
			long volume = atol(col4); // Volume en k.m³
            
            if (volume > 0) {
                char *id = strdup(col3); // L'ID de l'usine (col3) est la clé
                if(id == NULL){
                    continue;
                }
			    
                // Insertion/Maj AVL : le volume capté est cumulé dans capacite_total
			    racine = insertionAVL(racine, id, &h, volume, 0, NULL ); 
            }
		}
	}
	fclose(entree);

	FILE *sortie = fopen("vol_src.dat", "w");
	if (sortie == NULL) {
		perror("fopen vol_src.dat");
		freeAVL(racine);
		return;
	}

	fprintf(sortie, "%s", HEADER);

	parcoursInverse(racine, sortie);

	fclose(sortie);
	freeAVL(racine);
}

void histo_real(const char *csv){
	if(csv == NULL){
		return;
	}
	FILE *entree = fopen(csv, "r");
	if (entree == NULL) {
		perror("fopen csv");
		return;
	}

	Arbre *racine = NULL;
	int h = 0;
	char ligne[2048];
    
	const char *HEADER = "identifier;real volume (k.m³.year⁻¹)\n";

	while(fgets(ligne, sizeof(ligne), entree)){
		char *sauveptr = NULL;

		char *col1 = strtok_r(ligne, ";", &sauveptr);
		char *col2 = strtok_r(NULL, ";", &sauveptr);
		char *col3 = strtok_r(NULL, ";", &sauveptr); // ID Usine
		char *col4 = strtok_r(NULL, ";", &sauveptr); // Volume Capté
		char *col5 = strtok_r(NULL, ";", &sauveptr); // Fuite

		if(col1 == NULL || col2 == NULL || col3 == NULL || col4 == NULL || col5 == NULL){
			continue; 
		}

		// Ligne SOURCE -> USINE
		if(strcmp(col1, "-") == 0 && strcmp(col4, "-") != 0 && strcmp(col5, "-") != 0){
			long volume = atol(col4);
			double fuite = atof(col5); 

            if (volume > 0) {
                // Calcul du volume réel qui atteint l'usine : V × (1 − F / 100)
                long volume_reel = (long)(volume * (1.0 - fuite / 100.0)); 

                char *id = strdup(col3); // L'ID de l'usine est la clé
                if(id == NULL){
                    continue;
                }

                // Insertion/Maj AVL : le volume réel est cumulé dans conso_total
                racine = insertionAVL(racine, id, &h, 0, volume_reel, NULL);
            }
		}
	}
	fclose(entree);

	FILE *sortie = fopen("vol_real.dat", "w");
	if (sortie == NULL) {
		perror("fopen vol_real.dat");
		freeAVL(racine);
		return;
	}

	fprintf(sortie, "%s", HEADER);
    
	parcoursInverse(racine, sortie);
	fclose(sortie);
	freeAVL(racine);
}

// --- Fonction principale 'leaks' ---

void histo_leaks(const char *csv, const char *id_usine_cible) {
    if (csv == NULL || id_usine_cible == NULL) return;

    FILE *entree = fopen(csv, "r");
    if (entree == NULL) {
        perror("fopen csv");
        return;
    }

    Arbre *index_racine = NULL;
    int h = 0;
    char ligne[2048];
    char *sauveptr = NULL;

    // PHASE 1 : Construction silencieuse du réseau
    while (fgets(ligne, sizeof(ligne), entree)) {
        char temp_ligne[2048];
        strcpy(temp_ligne, ligne);

        char *token = strtok_r(temp_ligne, ";", &sauveptr); 
        char *col2 = strtok_r(NULL, ";", &sauveptr);
        char *col3 = strtok_r(NULL, ";", &sauveptr); 
        char *col4 = strtok_r(NULL, ";", &sauveptr);        
        char *col5 = strtok_r(NULL, ";", &sauveptr);        

        if (col2 == NULL || col3 == NULL|| strcmp(col2, "-") == 0) continue;

        // CAS 1 : Définition Usine
        if (strcmp(col3, "-") == 0 && col4 && strcmp(col4, "-") != 0) {
            Arbre *index = rechercherArbre(index_racine, col2);
            if (index == NULL) {
                index_racine = insertionAVL(index_racine, strdup(col2), &h, 0, 0, creationNoeud(col2, 0.0));
                index = rechercherArbre(index_racine, col2);
            }
            if (index && index->ptr_noeud_reseau->usine) {
                index->ptr_noeud_reseau->usine->capacite_max = atof(col4);
            }
        } 
        // CAS 2 : Apport Source -> Usine
        else if (strcmp(token, "-") == 0 && col4 && strcmp(col4, "-") != 0 && col5 && strcmp(col5, "-") != 0) {
            double volume_atteint = atof(col4) * (1.0 - atof(col5) / 100.0);
            Arbre *index = rechercherArbre(index_racine, col3);
            if (index == NULL) {
                index_racine = insertionAVL(index_racine, strdup(col3), &h, 0, 0, creationNoeud(col3, 0.0));
                index = rechercherArbre(index_racine, col3);
            }
            if (index) index->ptr_noeud_reseau->volume_amont += volume_atteint;
        }
        // CAS 3 : Distribution (Graphe)
        else if (strcmp(col3, "-") != 0 && col5 && strcmp(col5, "-") != 0) {
            double fuite_troncon = atof(col5);
            Arbre *idx_aval = rechercherArbre(index_racine, col3);
            if (idx_aval == NULL) {
                index_racine = insertionAVL(index_racine, strdup(col3), &h, 0, 0, creationNoeud(NULL, fuite_troncon));
                idx_aval = rechercherArbre(index_racine, col3);
            } else {
                idx_aval->ptr_noeud_reseau->fuite = fuite_troncon;
            }
            
            Arbre *idx_amont = rechercherArbre(index_racine, col2);
            if (idx_amont == NULL) {
                index_racine = insertionAVL(index_racine, strdup(col2), &h, 0, 0, creationNoeud(NULL, 0.0));
                idx_amont = rechercherArbre(index_racine, col2);
            }

            if (idx_amont && idx_aval) {
                ajouterEnfant(idx_amont->ptr_noeud_reseau, idx_aval->ptr_noeud_reseau);
            }
        }
    }
    fclose(entree);

    // PHASE 2 : Calcul des fuites
    Arbre *index_cible = rechercherArbre(index_racine, id_usine_cible);
    double fuite_finale = -1.0; 

    if (index_cible != NULL) {
        double vol_init = index_cible->ptr_noeud_reseau->volume_amont;
        Fils *c = index_cible->ptr_noeud_reseau->enfants;
        int n = 0;
        while(c) { n++; c = c->suivant; }

        if (n > 0 && vol_init > 0) {
            fuite_finale = 0.0;
            c = index_cible->ptr_noeud_reseau->enfants;
            while(c) {
                fuite_finale += calculerFuitesRecursif(c->enfant, vol_init / n);
                c = c->suivant;
            }
        } else if (vol_init > 0) {
            fuite_finale = 0.0; 
        }
    }

    // PHASE 3 : Sortie historique
    FILE *f_out = fopen("leaks_history.dat", "a");
    if (f_out) {
        if (fuite_finale < 0) fprintf(f_out, "%s;-1\n", id_usine_cible);
        else fprintf(f_out, "%s;%.3f\n", id_usine_cible, fuite_finale / 1000.0);
        fclose(f_out);
    }

    // PHASE 4 : Libération propre
    libererReseauIndexe(index_racine); 
    freeAVL(index_racine); 
}