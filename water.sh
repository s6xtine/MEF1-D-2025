#!/bin/bash	# le script s’exécute avec bash

# Initialisation des fichiers

minmaxFile='./lv_all_minmax.csv'
> "$minmaxFile"
outputFile='./sortie.csv'
> "$outputFile"

dataFile="$1"
inputFile='./entree.csv'
> "$inputFile"

tempFile='./tmp.csv'
> "$tempFile"
start_time=$(date +%s%3N)	# mémorise l’heure du début en milliseconde

# vérification de l'installation gnuplot 

GNUPLOT_AVAILABLE=false
if command -v gnuplot >/dev/null 2>&1; then # teste si gnuplot est installé
    GNUPLOT_AVAILABLE=true
    echo "✓ gnuplot détecté"
else
    echo "⚠ gnuplot non installé (histogrammes PNG ne seront pas générés)"
fi


# Fonction d'aide (qui affiche l’utilisation du programme shell)

aide(){
    echo "Utilisation :"
    echo " $0 histo <max|src|real>"
    echo " $0 leaks <identifiant_usine>"
    echo " $0 -h : Affiche cette aide"
    echo
    echo "Commandes :"
    echo "- histo <type> : Génère un histogramme"
    echo "- leaks <identifiant usine> : Calcul les pertes d'une usine"
   
}

# Vérification des arguments

verification_arg(){

    # option aide
   if [ "$1" = "-h" ] || [ "$2" = "-h" ] || [ "$3" = "-h" ]; then
   aide
        exit 0
    fi

    # vérification des arguments 

    if [ $# -lt 2 ]; then
    echo "Erreur : arguments insuffisants"
    aide
    exit 1
fi

commande="$2"
option="$3"

if [ ! -f "$inputFile" ]; then
    echo "Erreur : fichier de données introuvable : $inputFile"
    exit 2
fi


    case "$commande" in

        histo)
            if [ -z "$option" ]; then
                echo "Erreur : type histo manquant"
                return 2
            fi
            if [ $# -ne 3 ]; then
                echo "Erreur : nombre d'arguments incorrect"
                return 6
            fi
            if [[ "$option" != "max" && "$option" != "src" && "$option" != "real" ]]; then
                echo "Erreur : type histogramme invalide"
                return 3
            fi
            ;;


        leaks)
            if [ -z "$option" ]; then
                echo "Erreur : identifiant manquant"
                return 4
            fi
            if [ $# -ne 3 ]; then
                echo "Erreur : nombre d'arguments incorrect"
                return 7
            fi
        ;;

        *)
            echo "Erreur : commande inconnue"
            return 5
        ;;
    esac

    return 0
}


verification_arg "$@"
ret=$?	# récupère le code de retour de la fonction

if (( ret != 0 )); then	# si différent on affiche l’aide et on sort
    aide
    exit $ret
else
    echo "Arguments OK"
fi

# Compilation (faire le lien dans le code c)

make
if [ $? -ne 0 ]; then
    echo "Erreur : compilation échouée"
    exit 10
fi


# CAS HISTOGRAMME


if [ "$commande" = "histo" ]; then

    typeH="$option"

    echo "Génération histogramme ($typeH)"

    ./wildwater histo "$typeH" "$dataFile"

    if [ $? -ne 0 ]; then
        echo "Erreur exécution programme C"
        exit 11
    fi

    histoFile="vol_${typeH}.dat"

    if [ ! -f "$histoFile" ]; then
        echo "Erreur : $histoFile manquant"
        exit 12
    fi

    tail -n +2 "$histoFile" | sort -t';' -k2 -nr | head -10 > top10.dat
    tail -n +2 "$histoFile" | sort -t';' -k2 -n  | head -50 > bottom50.dat


    if [ "$GNUPLOT_AVAILABLE" = true ]; then
        echo "Génération image ..."
        gnuplot <<EOF	# lance gnuplot pour créer un PNG
set datafile separator ";"
set terminal png size 1800,1000
set style data histograms
set style fill solid border -1
set boxwidth 0.9
set grid
set xtics rotate by -45 font ",8"
set key off

set output "vol_${typeH}_top10.png"
set title "Top 10 des plus grandes usines ($typeH)"
set xlabel "Usines"
set ylabel "Volume (M.m³ / an)"
plot "top10.dat" using 2:xtic(1)

set output "vol_${typeH}_bottom50.png"
set title "50 plus petites usines ($typeH)"
set xlabel "Usines"
set ylabel "Volume (M.m³ / an)"
plot "bottom50.dat" using 2:xtic(1)
EOF
       echo "✓ Images créées :"
       echo "  - vol_${typeH}_top10.png"
       echo "  - vol_${typeH}_bottom50.png"

    else
        echo "⚠ gnuplot non disponible → pas d'image PNG générée"
        echo "   Données disponibles dans : $histoFile"
    fi

fi


# CAS LEAKS

if [ "$commande" = "leaks" ]; then

    usine="$option"

    echo "Calcul des fuites pour : $usine"

    ./wildwater leaks "$usine" "$dataFile"	# exécution du fichier

    if [ $? -ne 0 ]; then
        echo "Erreur programme C"
        exit 13
    fi

    echo "Ajout dans leaks.dat effectué"

fi

# Temps total (calcul et affiche la durée en millisecondes)

end_time=$(date +%s%3N)
duration=$(( end_time - start_time ))

echo "Temps total : ${duration} ms"

exit 0

