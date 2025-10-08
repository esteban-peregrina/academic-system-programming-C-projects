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
# Laisse make chercher (avec wildcard) et récupérer tous les fichiers .c de SRC_DIR/
# i.e. : SRCS = src/fichier1.c src/fichier2.c (voir la règle debug)
SRCS = $(wildcard $(SRC_DIR)/*.c) 
# Créé dans BIN_DIR/ un fichier .o par .c de SRC_DIR/
# i.e. : BUILDS = bin/fichier1.o bin/fichier2.o (voir la règle debug)
BUILDS = $(SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) 

TARGET = $(BIN_DIR)/$(EXEC)

all: $(TARGET)
	@echo "Création de l'executable terminée."

# Compile chaque fichier .c $< en un fichier .o $@ (règle résolue par la suivante !)
# Cette règle sera typiquement lancée lorsque le timestamp des .o cible sera plus ancien que celui des dépendances .c, 
# grâce à la wildcard % (tous les % d'une même règle sont remplacés par la même chose)
# $@ : la cible 
# $< : la première dépendance venue (ici il n'y en a qu'une seule)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compilation de $< en $@..."
	$(CC) $(CFLAGS) -c $< -o $@ 
	@echo "Compilation de $< en $@ terminée."

# Lie les fichiers .o ensemble en un executable avec le compilateur (règle résolue par "all" !)
# On n'utilise par de wildcards car il faut la liste des fichiers, par fichier par fichier
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
	rm -r -f $(BUILD_DIR)/* $(BIN_DIR)/*
	@echo "Nettoyage terminé."

# Recompile
rebuild: clean all

# Précise quels cibles ne sont jamais des fichiers pour systématiquement exécuter la cible 
# (sans se baser sur le timestamp d'un eventuel fichier de même nom) 
.PHONY: all clean rebuild setup