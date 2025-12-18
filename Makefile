# Nom de l'exécutable final
TARGET = histo

# Compilateur et options
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Dossier pour les fichiers objets
BUILD = build

# Liste des fichiers source et des objets correspondants
SRC = main.c histo.c AVL.c noeud.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)

# Cible par défaut : construit l'exécutable
all: $(TARGET)

# Édition de liens : crée l'exécutable à partir des objets
$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

# Compilation : crée les fichiers .o dans le dossier build/
$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Crée le dossier build s'il n'existe pas
$(BUILD):
	mkdir -p $(BUILD)

# --------- Cibles utilitaires ---------

# Nettoyage des fichiers temporaires et de l'exécutable
clean:
	rm -rf $(BUILD) $(TARGET)
	rm -f *.dat *.png

# Reconstruction complète
re: clean all

# Exécute le script principal avec les graphiques
water: $(TARGET)
	chmod +x water.sh
	./water.sh data/c-wildwater_v0.dat histo max

# Exécute gnuplot via votre script de plot
plot:
	chmod +x plot.sh
	./plot.sh

.PHONY: all clean re water plot