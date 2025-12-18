#ifndef HISTO_H
#define HISTO_H

// Fonctions d'histogramme
void histo_max(const char *csv);
void histo_src(const char *csv);
void histo_real(const char *csv);

// Fonctions de fuites
void histo_leaks(const char *csv, const char *id_usine_cible);
// Wrapper pour appeler le traitement correspondant
int traitement_histo(const char *type, const char *csv, const char *id_usine);

// Fonctions utilitaires pour conversion
long str_to_long(const char *s);
double str_to_double(const char *s);

#endif
