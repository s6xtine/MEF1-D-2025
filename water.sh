#!/bin/bash

# INITIALISATION ET CHRONOMÈTRE 
# Utilisation de Python pour obtenir le temps en ms car date n'est pas fiable sur tous les systèmes(Mac/Linux)
start_time=$(python3 -c 'import time; print(int(time.time() * 1000))')

# Fichiers temporaires (qui serviront pour la génération des histogrammes)
topFile='./top10.dat'
bottomFile='./bottom50.dat'
> "$topFile"
> "$bottomFile"

# VÉRIFICATION GNUPLOT 
GNUPLOT_AVAILABLE=false # variable booléenne par défaut
if command -v gnuplot >/dev/null 2>&1; then # vérifie si gnuplot est installé et supprime les sorties
    GNUPLOT_AVAILABLE=true
    echo "✓ gnuplot détecté"
else
    echo "⚠ gnuplot non installé (histogrammes PNG ne seront pas générés)"
fi

# FONCTION D'AIDE 
aide(){ 
    echo "Utilisation :"
    echo " $0 <fichier.csv> histo <max|src|real>" #$0 est le nom du script et ça explique les modes d'utilsation 
    echo " $0 <fichier.csv> leaks <identifiant_usine>"
    echo " $0 -h : Affiche cette aide"
}

# VÉRIFICATION DES ARGUMENTS
verification_arg(){
    # Option aide
    if [[ "$1" == "-h" ]]; then
        aide
        exit 0
    fi

    # Nombre d'arguments strict (exactement 3)
    if [ $# -ne 3 ]; then
        echo "Erreur : nombre d'arguments incorrect"
        return 1
    fi

    # Vérification de l'existence du fichier de données
    if [ ! -f "$1" ]; then
        echo "Erreur : fichier de données introuvable : $1"
        return 2
    fi

    # Vérification des commandes
    case "$2" in
        histo)
            if [[ "$3" != "max" && "$3" != "src" && "$3" != "real" ]]; then # le 3ème argument doit être max, src ou real
                echo "Erreur : type histogramme invalide (max|src|real)"
                return 3
            fi
            ;;
        leaks)
            if [ -z "$3" ]; then # -z correspond à une chaîne vide
                echo "Erreur : identifiant usine manquant"
                return 4
            fi
            ;;
        *)
            echo "Erreur : commande inconnue"
            return 5
            ;;
    esac
    return 0
}

# Appel de la vérification
verification_arg "$@" # vérifie tous les arguments du script 
ret=$? # récupère le code de retour de la dernière commande
if (( ret != 0 )); then
    # Affichage du temps même en cas d'erreur avant de quitter
    end_time=$(python3 -c 'import time; print(int(time.time() * 1000))')
    echo "Durée totale : $((end_time - start_time)) ms"
    exit $ret
fi

# Variables de travail
dataFile="$1"
commande="$2"
option="$3"

# COMPILATION 
if [ ! -f histo ]; then #vérifie si l'exécutable histo existe
    echo "Exécutable absent. Compilation via make..."
    make re # lance la compilation
    if [ $? -ne 0 ]; then
        echo "Erreur : compilation échouée"
        exit 10
    fi
fi

# PRÉPARATION DES SORTIES ET EN-TÊTES 
if [ "$commande" == "histo" ]; then
    case $option in
        max)  OUTPUT="vol_max.dat";  HEADER="identifier / max volume (k.m3.year-1)";;
        src)  OUTPUT="vol_src.dat";  HEADER="identifier / source volume (k.m3.year-1)";;
        real) OUTPUT="vol_real.dat"; HEADER="identifier / real volume (k.m3.year-1)";;
    esac
    echo "$HEADER" > "$OUTPUT"
else
    OUTPUT="leaks_history.dat"
    # On ne crée l'en-tête que si le fichier n'existe pas 
    if [ ! -f "$OUTPUT" ]; then
        echo "identifier / Leak volume (M.m3.year-1)" > "$OUTPUT"
    fi
fi

# EXÉCUTION DU PROGRAMME C 
./histo "$dataFile" "$commande" "$option" # lance le programme C
C_RET=$? # récupère son code retour

if [ $C_RET -ne 0 ]; then
    echo "Erreur : Le programme C a échoué (Code $C_RET)"
    exit 11
fi

# GÉNÉRATION DES GRAPHIOUES (CAS HISTO)
if [ "$commande" == "histo" ] && [ "$GNUPLOT_AVAILABLE" = true ]; then
    echo "Traitement des données pour gnuplot..."
    
    grep -v ";-" "$OUTPUT" | tail -n +2 | sort -t';' -k2 -nr > tmp_sorted.dat # supprime les valeurs négatives, ignore l'en-tête et tri par volume décroissant

# Top 10
    tail -n +2 "$OUTPUT" | grep -v ";-" | head -10 > "$topFile"
# Bottom 50
    tail -n 50 tmp_sorted.dat > "$bottomFile"

    rm tmp_sorted.dat
    echo "Lancement de gnuplot..."
    gnuplot <<EOF 
# Le séparateur des colonnes est le ;
set datafile separator ";" 
# Génère une image PNG de 1200x800 pixels
set terminal png size 1200,800
# Style des barres : pleines à 50 % avec bordure fine
set style fill solid 0.5 border -1
# Incline les labels de l’axe X pour qu’ils soient lisibles
set xtics rotate by -45 font ",8"
# Ajoute une grille horizontale
set grid y
# Nom de l’axe vertical
set ylabel "Volume"

# Fichier image de sortie pour le Top 10
set output "vol_${option}_top10.png"
# Titre du graphique
set title "Top 10 - $HEADER"
# Trace l’histogramme à partir du fichier top10
plot "$topFile" using 2:xtic(1) title "Top 10" with histograms

# Fichier image de sortie pour le Bottom 50
set output "vol_${option}_bottom50.png"
# Titre du graphique
set title "50 plus petites - $HEADER"
# Trace l’histogramme à partir du fichier bottom50
plot "$bottomFile" using 2:xtic(1) title "Bottom 50" with histograms
EOF
    echo "✓ Images créées : vol_${option}_top10.png et vol_${option}_bottom50.png"
    rm "$topFile" "$bottomFile" # supprime les fichiers temporaires
fi

# TEMPS TOTAL 
end_time=$(python3 -c 'import time; print(int(time.time() * 1000))')
duration=$(( end_time - start_time ))

echo "--------------------------------------"
echo "Temps total d'exécution : ${duration} ms"
echo "--------------------------------------"

exit 0