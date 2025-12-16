CC = gcc
CFLAGS = -Wall -Wextra -std=c11
BUILD = build
TARGET = histo

SRC = main.c histo.c AVL.c noeud.c
OBJ = $(SRC:%.c=$(BUILD)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

$(BUILD)/%.o: %.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD):
	mkdir -p $(BUILD)

# --------- Cibles utilitaires ---------

run: $(TARGET)
	./$(TARGET)

water: $(TARGET)
	chmod +x water.sh
	./water.sh

plot:
	chmod +x plot.sh
	./plot.sh

clean:
	rm -rf $(BUILD) $(TARGET)

re: clean all

.PHONY: all clean re run water plot
