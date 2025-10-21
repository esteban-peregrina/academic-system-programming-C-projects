# Compilateur et ses arguments
CC = gcc
CFLAGS = -Wall -g 

# Nom de l'executable final
EXEC = program

# Répertoires du projet
SRC_DIR = src
BIN_DIR = bin
BUILD_DIR = build

###### Fichiers ######
SRCS = $(wildcard $(SRC_DIR)/*.c) 
BUILDS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) 

TARGET = $(BIN_DIR)/$(EXEC)

all: setup $(TARGET)
	@echo "Création de l'executable terminée."

# Compile chaque fichier .c $< en un fichier .o $@ (règle résolue par la suivante !)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilation de $< en $@..."
	$(CC) $(CFLAGS) -c $< -o $@ 
	@echo "Compilation de $< en $@ terminée."

# Lie les fichiers .o ensemble en un executable avec le compilateur (règle résolue par "all" !)
$(TARGET): $(BUILDS)
	@echo "Liaison des fichiers .o..."
	$(CC) $(BUILDS) -o $(TARGET) -lm
	@echo "Liaison des .o terminée."

# Créée les dossiers
setup:
	@echo "Création des dossiers du projet..."
	mkdir -p $(BUILD_DIR) $(BIN_DIR)
	@echo "Création des dossiers terminée."

debug:
	@echo "$(SRCS) : $(BUILDS)"

# Supprime les fichiers compilés
clean:
	@echo "Nettoyage..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Nettoyage terminé."

# Recompile
rebuild: clean all

# Précise quels cibles ne sont jamais des fichiers pour systématiquement exécuter la cible 
.PHONY: all clean rebuild setup