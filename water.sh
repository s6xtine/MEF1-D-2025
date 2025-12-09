#!/bin/bash	# le script s’exécute avec bash


# Initialisation des fichiers

inputFile='./data/c-wildwater_v0.dat'		# fichier de données en entrée
# Ne pas créer le fichier s'il existe déjà
if [ ! -f "$inputFile" ]; then
    echo "Erreur : fichier $inputFile introuvable"
    exit 1
fi		

outputFile='./sortie.csv'
> "$outputFile"

tempFile='./tmp.csv'
> "$tempFile"

histoFile=""	# variable vide initialisée
leaksFile="./leaks.dat"
> "$leaksFile"

start_time=$(date +%s)	# mémorise l’heure du début en seconde

# vérification de l'installation gnuplot 

GNUPLOT_AVAILABLE=false
if command -v gnuplot >/dev/null 2>&1; then
    GNUPLOT_AVAILABLE=true
    echo "✓ gnuplot détecté"
else
    echo "⚠ gnuplot non installé (histogrammes PNG ne seront pas générés)"
fi


# Fonction d'aide (qui affiche l’utilisation du programme shell)

aide(){
    echo -e "Utilisation : $0 <commande> <option> <identifiant> <aide>\n"
    echo "Commandes :"
    echo "- histo <type> : Génère un histogramme"
    echo "- leaks <identifiant usine> : Calcul les pertes d'une usine"
    echo "Type histogramme : max / src / real"
    echo "-h : affiche cette aide"
}

# Vérification des arguments

verification_arg(){
    if [ -z "$1" ]; then
        echo "Erreur : commande manquante"
        return 1
    fi

    # option aide
   if [ "$2" = "-h" ] || [ "$3" = "-h" ] || [ "$4" = "-h" ] || [ "$5" = "-h" ]; then
   aide
        exit 0
    fi

    case $1 in

        "histo")
            if [ -z "$2" ]; then
                echo "Erreur : type histo manquant"
                return 2
            fi
            if [ $# -ne 2 ]; then
                echo "Erreur : nombre d'arguments incorrect"
                return 6
            fi
            case $2 in
                "max"|"src"|"real")
                    echo "Type histogramme OK"
                    ;;
                *)
                    echo "Erreur : type histogramme invalide"
                    return 3
                    ;;
            esac
        ;;

        "leaks")
            if [ -z "$2" ]; then
                echo "Erreur : identifiant manquant"
                return 4
            fi
            if [ $# -ne 2 ]; then
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


verification_arg $@
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
    echo "Erreur : compilation échoué"
    exit 10
fi


# CAS HISTOGRAMME


if [ "$1" = "histo" ]; then

    typeH=$2

    echo "Génération histogramme ($typeH)"

    ./wildwater histo "$typeH"

    if [ $? -ne 0 ]; then
        echo "Erreur exécution programme C"
        exit 11
    fi

    histoFile="vol_${typeH}.dat"

    if [ ! -f "$histoFile" ]; then
        echo "Erreur : $histoFile manquant"
        exit 12
    fi

    if [ "$GNUPLOT_AVAILABLE" = true ]; then
        echo "Génération image ..."
        gnuplot <<EOF	// lance gnuplot pour créer un PNG
set terminal png size 1200,800
set output "vol_${typeH}.png"
set title "Histogramme ($typeH)"
set xlabel "Usines"
set ylabel "Volume (k.m3)"
set xtics rotate by -45
plot "$histoFile" using 2:xtic(1) with boxes title "$typeH"
EOF
        echo "✓ Image créée : vol_${typeH}.png"
    else
        echo "⚠ gnuplot non disponible → pas d'image PNG générée"
        echo "   Données disponibles dans : $histoFile"
    fi

fi


# CAS LEAKS

if [ "$1" = "leaks" ]; then

    usine="$2"

    echo "Calcul des fuites pour : $usine"

    ./wildwater leaks "$usine"	# exécution du fichier

    if [ $? -ne 0 ]; then
        echo "Erreur programme C"
        exit 13
    fi

    echo "Ajout dans leaks.dat effectué"

fi

# Temps total (calcul et affiche la durée en secondes)

end_time=$(date +%s)
duration=$(( end_time - start_time ))

echo "Temps total : ${duration}.0 sec"

exit 0

