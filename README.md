# MEF1-D-2025

## Description
Ce projet est une application permettant de gérer et d'analyser des données liées à un réseau d'eau. Il inclut des fonctionnalités pour :
- Générer des histogrammes basés sur les volumes d'eau captés et les fuites.
- Utiliser des structures AVL pour organiser et traiter efficacement les données.
- Visualiser les résultats sous forme de graphiques.

## Structure du projet
- **`main.c`** : Point d'entrée du programme. Permet à l'utilisateur de choisir les analyses à effectuer.
- **`noeud.c` / `noeud.h`** : Gestion des structures de nœuds représentant les points du réseau.
- **`AVL.c` / `AVL.h`** : Implémentation des arbres AVL pour organiser les données.
- **`histo.c` / `histo.h`** : Génération des fichiers de données pour les histogrammes.
- **`plot.sh`** : Script pour générer des graphiques avec `gnuplot`.
- **`water.sh`** : Script shell pour automatiser la compilation, l'exécution et la génération des graphiques.
- **`data/`** : Contient les fichiers de données d'entrée, comme `c-wildwater_v0.dat`.
- **`build/`** : Répertoire pour les fichiers compilés.

## Prérequis
- **Système d'exploitation** : macOS ou Linux.
- **Compilateur** : GCC (GNU Compiler Collection).
- **Outils supplémentaires** :
  - `gnuplot` pour la génération des graphiques.

## Instructions

### Compilation
Pour compiler le projet, exécutez la commande suivante :
```bash
make
```

### Exécution
Pour exécuter le programme principal :
```bash
./main
```

### Génération des graphiques
Pour générer les graphiques, utilisez le script `water.sh` :
```bash
bash water.sh
```

### Options disponibles
Lors de l'exécution du programme, vous pouvez choisir parmi les options suivantes :
1. Générer un histogramme des 10 plus gros volumes captés.
2. Générer un histogramme des 10 plus grandes fuites.
3. Générer un histogramme personnalisé.

### Exemple de flux complet
1. Exécutez le programme principal pour générer les fichiers de données nécessaires :
   ```bash
   ./main
   ```
2. Utilisez `plot.sh` pour générer les graphiques :
   ```bash
   bash plot.sh
   ```

## Structure des fichiers de données
- **`c-wildwater_v0.dat`** : Fichier d'entrée contenant les données du réseau d'eau.
- **`vol_max.dat`** : Fichier généré pour les volumes maximaux.
- **`fuites_max.dat`** : Fichier généré pour les fuites maximales.

## Dépendances
- `gnuplot` : Assurez-vous que `gnuplot` est installé pour générer les graphiques.

## Auteur
- Projet réalisé par Deschamps, Lechevallier, 

