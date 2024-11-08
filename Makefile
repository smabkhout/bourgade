# Définition des variables
MAX_X ?= 4
MAX_Y ?= 3
NUM_RESOURCES ?= 5
BOURGADE_FLAGS = -DMAX_X=$(MAX_X) -DMAX_Y=$(MAX_Y) -DNUM_RESOURCES=$(NUM_RESOURCES)
CFLAGS = -Wall -Wextra -std=c99 -g3 $(BOURGADE_FLAGS) -I src -I test
OBJ_DIR = obj
SRC_DIR = src
TEST_DIR = test

# Liste des fichiers sources et objets
SRC_FILES = $(SRC_DIR)/color.c $(SRC_DIR)/position.c $(SRC_DIR)/resource.c
OBJ_FILES = $(OBJ_DIR)/color.o $(OBJ_DIR)/position.o $(OBJ_DIR)/resource.o

# Fichiers de test et exécutables de test
TEST_FILES = $(TEST_DIR)/test_color.c $(TEST_DIR)/test_position.c $(TEST_DIR)/test_resource.c
TEST_EXECUTABLES = test_color test_position test_resource

# Cible par défaut
all: project $(TEST_EXECUTABLES)

# Compilation des sources du projet
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Compilation des fichiers de test
$(OBJ_DIR)/%.o: $(TEST_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Lien du projet principal
project: $(OBJ_FILES) $(OBJ_DIR)/project.o
	$(CC) $(CFLAGS) $(OBJ_FILES) $(OBJ_DIR)/project.o -o project

# Lien des fichiers de test en exécutables séparés
test_color: $(OBJ_FILES) $(OBJ_DIR)/test_color.o
	$(CC) $(CFLAGS) $(OBJ_FILES) $(OBJ_DIR)/test_color.o -o test_color

test_position: $(OBJ_FILES) $(OBJ_DIR)/test_position.o
	$(CC) $(CFLAGS) $(OBJ_FILES) $(OBJ_DIR)/test_position.o -o test_position

test_resource: $(OBJ_FILES) $(OBJ_DIR)/test_resource.o
	$(CC) $(CFLAGS) $(OBJ_FILES) $(OBJ_DIR)/test_resource.o -o test_resource

# Nettoyage des fichiers objets et des exécutables
clean:
	@rm -f project $(TEST_EXECUTABLES) $(OBJ_DIR)/*.o *~ $(SRC_DIR)/*~ $(TEST_DIR)/*~

# Dépendances
$(OBJ_DIR)/project.o: $(SRC_DIR)/project.c
$(OBJ_DIR)/test_color.o: $(TEST_DIR)/test_color.c $(SRC_DIR)/color.h
$(OBJ_DIR)/test_position.o: $(TEST_DIR)/test_position.c $(SRC_DIR)/position.h
$(OBJ_DIR)/test_resource.o: $(TEST_DIR)/test_resource.c $(SRC_DIR)/resource.h
$(OBJ_DIR)/color.o: $(SRC_DIR)/color.c $(SRC_DIR)/color.h
$(OBJ_DIR)/position.o: $(SRC_DIR)/position.c $(SRC_DIR)/position.h
$(OBJ_DIR)/resource.o: $(SRC_DIR)/resource.c $(SRC_DIR)/resource.h

# Cible pour les tests
test: $(TEST_EXECUTABLES)
	@echo "Running test_color:"
	./test_color
	@echo "Running test_position:"
	./test_position
	@echo "Running test_resource:"
	./test_resource

.PHONY: all clean test
