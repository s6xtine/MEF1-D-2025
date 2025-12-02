#!/bin/bash

#initialisations des fichiers

outputFile='./sortie.txt'
> "$outputFile"

inputFile='./entree.txt'
> "$inputFile"


#fonction qui vérifie s'il y a le bon nombre d'arguments et s'ils sont cohérents
verification_arg(){
    #signale un probleme si les arguments 1, 2 et 3 sont manquants
    if [ -z $1 ] || [ -z $2 ] || [ -z $3 ]; then
        echo "arguments manquants"
        return 1
    else 
    	#signale un probleme si le fichier n'existe pas
        if [ ! -f "$1" ]; then 
            echo "Le fichier n'existe pas"
            return 2
        else
            echo "Fichier existant, OK"
        fi
        a='indiv'
        b='all'
        c="comp"
        #verifie que les arguemnts sont cohérents
        case $2 in
            'hvb'|'hva') 
                echo "Argument 2 valide"
                if [ "$3" == "$c" ]; then
                    echo "Argument 3 valide"
                else
                    echo "Erreur : argument 3 invalide"
                    return 3
                fi
           ;;
            'lv')
                echo "Argument 2 valide"
                if [ "$3" == "$a" ] || [ "$3" == "$b" ] || [ "$3" == "$c" ]; then
                    echo "Argument 3 valide"
                else
                    echo "Erreur : argument 3 invalide"
                    return 4
                fi
           ;;
            *)
                echo "Erreur : Argument 2 invalide"
                return 5
          ;;
        esac 
    fi
    #verifie s'il y a un 5eme argument pour afficher l'aide
    if [ -z $5 ]; then
       echo "pas d'aide"
    else
    	#affiche l'adie lorsque demandé
        d="-h"
        if [ "$5" != "$d" ]; then
            echo "argument 5 invalide"
            return 6
        else
            aide
        fi  
    fi
    #verifie s'il y a un 4eme argument pour l''identifiant de la centrale
    if [ -z $4 ]; then
        echo "pas d'id de centrale"
    else
        e=5
        if (( "$4" > "$e" )); then
            echo "argument 4 invalide"
            return 7
        else
            echo 'argumnt 4 valide'
        fi
        return 0
    fi
}

#fonction qui affiche une aide détaillée sur l'utilisation du script
aide(){
    echo -e "Utilisation : $0  <arg 1>  <arg2 >  <arg 3>  <arg4>  <arg5>\n"
    echo "Description : L'utilisation de ce script nécessite 3 à 5 arguments :"
    echo "-argument 1 : le chemin du fichier de données, il indique l'endroit où se trouve le fichier d'entrée"
    echo "-argument 2 : le type de station à traiter (hvb, hva ou lv)"
    echo "-argument 3 : le type de consommateur à traiter (comp pour entreprises, indiv pour particuliers ou all pour tous)"
    echo "-argument 4 : OPTIONEL - identifiant de la centrale, permet de filtrer les résultats pour une centrale spécifique"
    echo "-argument 5 : OPTIONEL - (-h) permet d'afficher une aide détaillée sur l'utilisation du script"
    echo "Attention! Dans le cas des stations hvb et hva, la seule option de consommateur est comp"
}

verification_arg $1 $2 $3 $4 $5
res1=$?
#s'il y a un probleme, on affiche l'aide
if (( res1 != 0 )); then
    echo "$res1"
    aide
else
    echo 'OK'
fi

#cd codeC
make -C ./codeC
#gcc -o programme codeC.c
#verifie l'existance du fichier c sinon elle le compile
#if [ ! -f test ]; then 
   # echo "l'executable programme.c est introuvable. compilation ..."
    if [ $? -ne 0 ]; then 
        echo 'erreur : echec de la compilation'
        exit 1
    fi
#fi

#on débute le chronometre pour avoir le temps d'exécution
start_time=$(date +%s)

# Si un identifiant de centrale est fourni, on fait le filtrage pour la centrale demandée ainsi que pour les autres parametres
if [ -n "$4" ]; then 
 case $2 in
 
#dans le cas des stations hvb et hva, le consommateur ne peut être que une entrerpise
        'hvb')
            grep -E "^[0-9]+;[0-9]+;-;-;" "$1" | grep -E "$4;" | cut -f 2,7,8 -d ';' | tr '-' '0' | tr ';' ':' > "$inputFile"
            	end_time=$(date +%s)
    		duration=$(( end_time - start_time ))
    		echo "temps d'exucation : ${duration}.0 sec"
            if [ $? != 0 ]; then
            	echo "execution echouee"
            fi
        ;;
        'hva')
            grep -E "^[0-9]+;[^;]*;[0-9]+;-;" "$1" | grep -E "$4;" | cut -f 3,7,8 -d ';' | tr '-' '0' | tr ';' ':' > "$inputFile"
            end_time=$(date +%s)
    	duration=$(( end_time - start_time ))
    	echo "temps d'exucation : ${duration}.0 sec"
        ;;
        'lv')
        case $3 in
        	'comp')
        		grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;[^;]*;-;" "$1" | grep -E "$4;" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        		end_time=$(date +%s)
    			duration=$(( end_time - start_time ))
    			echo "temps d'exucation : ${duration}.0 sec"
        	;;
        	'indiv')
        		grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;-;[^;]*;" "$1" | grep -E "$4;" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        		end_time=$(date +%s)
    			duration=$(( end_time - start_time ))
    			echo "temps d'exucation : ${duration}.0 sec"
    		;;
        	'all')
        		grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;[^;]*;[^;]*;" "$1" | grep -E "$4;" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        		end_time=$(date +%s)
    			duration=$(( end_time - start_time ))
    			echo "temps d'exucation : ${duration}.0 sec"
        	;;
        	*)
        		echo 'erreur'
        	;;
        esac
        ;;
        *)
            echo "Option non reconnue pour la centrale $4."
        ;;
    esac
else
#si aucun identifiant de centrale n'est spécifié, on effectue le filtrage sur les autres arguments
	case $2 in
    		'hvb')
        		grep -E "^[0-9]+;[0-9]+;-;-;" "$1" | cut -f 2,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        		end_time=$(date +%s)
        		duration=$(( end_time - start_time ))
        		echo "temps d'exucation : ${duration}.0 sec"
    		;;
    		'hva')
    		#cd codeC

#make 
        		grep -E "^[0-9]+;[^;]*;[0-9]+;-;" "$1" | cut -f 3,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        		end_time=$(date +%s)
        		duration=$(( end_time - start_time ))
        		echo "temps d'exucation : ${duration}.0 sec"
    		;;
    		'lv')
        		case $3 in
        			'comp')
        				grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;[^;]*;-;" "$1" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        				end_time=$(date +%s)
    					duration=$(( end_time - start_time ))
    					echo "temps d'exucation : ${duration}.0 sec" 
        			;;
        			'indiv')
        				grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;-;[^;]*;" "$1" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        				end_time=$(date +%s)
        				duration=$(( end_time - start_time ))
        				echo "temps d'exucation : ${duration}.0 sec"
        			;;
        			'all')
        				grep -E "^[^;]*;[^;]*;[^;]*;[0-9]+;[^;]*;[^;]*;" "$1" | cut -f 4,7,8 -d ';' | tr '-' '0' | tr ';' ':'  > "$inputFile"
        				end_time=$(date +%s)
        				duration=$(( end_time - start_time ))
        				echo "temps d'exucation : ${duration}.0 sec"
        			;;
        			*)
        				echo 'erreur'
        			;;
        		esac
    		;;
    		*)
        		echo 'test'
    		;;
	esac
fi

start_time2=$(date +%s)
        		./codeC/test < "$inputFile"
        		end_time2=$(date +%s)
    			duration2=$(( end_time - start_time ))
    			echo "temps d'exucation : ${duration2}.0 sec"

#ici, on nomme les fichiers et on effectue le traitement supplémentzire pour les station lv all
if [ -n "$4" ]; then
	if [ "$2" = "lv" ] && [ "$3" = "all" ]; then
		#on trie d'abord le fichier par ordre croissant de la consommation
		sort -t ':' -k 3,3n "$outputFile" > "$2_$3_$4.txt"
		#puis on extrait les 10 plus petites consommations dans un fichier temporaire
        	head -n 5 "$2_$3_$4.txt" > "$tempFile"
        	#puis on extrait les 10 plus grandes consommations dans un fichiers temporaire
        	tail -n 5 "$2_$3_$4.txt" >> "$tempFile"
        	#on lit ligne par ligne les colonnes de la conso et de la capacité pour en faire la différence qu'on ajoute dans une nouvelle colonne
        	while IFS= read -r ligne; do
    			capa=$(echo "$ligne" | cut -d':' -f2)
    			conso=$(echo "$ligne" | cut -d':' -f3)
    			qaec=$((conso - capa))
    			echo "$ligne:$qaec" >> "$minmaxFile"
		done < "$tempFile"
		cp "$minmaxFile" "$tempFile"
		#puis on trie par ordre décroissant de la quantité absolue d'énergie consommée, puis on retire la colonne
		sort -t ':' -k 4,4nr "$tempFile" | cut -f 1,2,3 -d ':' > "$minmaxFile"
		#on rajoute en premiere ligne les informations 
		texte="Station LV : Capacité : Consommation (tous)"
        	sed -i "1i$texte" "$minmaxFile"
        else
        	#on trie par ordre croissant 
		sort -t ':' -k 2,2n "$outputFile" > "$2_$3_$4.txt"
		case $2 in
			'hvb')
				texte="Station HVB : Capacité : Consommation (entreprises)"
        			sed -i "1i$texte" "$2_$3_$4.txt"
        		;;
        		'hva')
        			texte="Station HVA : Capacité : Consommation (entreprises)"
        			sed -i "1i$texte" "$2_$3_$4.txt"
        		;;
        		'lv')
        			case $3 in
        				'comp')
        				texte="Station LV : Capacité : Consommation (entreprises)"
        				sed -i "1i$texte" "$2_$3_$4.txt"
        				;;
        				'indiv')
        					texte="Station LV : Capacité : Consommation (particuliers)"
        					sed -i "1i$texte" "$2_$3_$4.txt"
        				;;
        				*)
        					echo 'erreur'
        				;;
        			esac
        		;;
        		*)
        			echo 'erreur'
        		;;
		esac
        fi
else
#on fait la meme chose dans le  cas ou il n'y a pas d'identifiant de centrale
	if [ "$2" = "lv" ] && [ "$3" = "all" ]; then
		sort -t ':' -k 3,3n "$outputFile" > "$2_$3.txt"
        	head -n 10 "$2_$3.txt" > "$tempFile"
        	tail -n 10 "$2_$3.txt" >> "$tempFile"
        	while IFS= read -r ligne; do
    			capa=$(echo "$ligne" | cut -d':' -f2)
    			conso=$(echo "$ligne" | cut -d':' -f3)
    			qaec=$((conso - capa))
    			echo "$ligne:$qaec" >> "$minmaxFile"
		done < "$tempFile"
		cp "$minmaxFile" "$tempFile"
		sort -t ':' -k 4,4nr "$tempFile" | cut -f 1,2,3 -d ':' > "$minmaxFile"
        	texte="Station LV : Capacité : Consommation (tous)"
        	sed -i "1i$texte" "$minmaxFile"
        else
		sort -t ':' -k 2,2n "$outputFile" > "$2_$3.txt"
		case $2 in
		'hvb')
			texte="Station HVB : Capacité : Consommation (entreprises)"
        		sed -i "1i$texte" "$2_$3.txt"
        	;;
        	'hva')
        		texte="Station HVA : Capacité : Consommation (entreprises)"
        		sed -i "1i$texte" "$2_$3.txt"
        	;;
        	'lv')
        		case $3 in
        			'comp')
        				texte="Station LV : Capacité : Consommation (entreprises)"
        				sed -i "1i$texte" "$2_$3.txt"
        			;;
        			'indiv')
        				texte="Station LV : Capacité : Consommation (particuliers)"
        				sed -i "1i$texte" "$2_$3.txt"
        			;;
        			*)
        				echo 'erreur'
        			;;
        		esac
        	;;
        	*)
        		echo 'erreur'
        	;;
	esac
	fi
fi
