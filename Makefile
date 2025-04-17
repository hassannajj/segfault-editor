# Default file if none is provided
FILE ?= test1.txt

# Directories
LIB_DIR = lib
INCLUDE_DIR = include
BUILD_DIR = build

# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsafe_memory

# valgrind
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# Ensure build/ dir exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Default target
all: main-build

# === Main program ===
main-build: $(BUILD_DIR)/main

$(BUILD_DIR)/main: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

run: main-build
	./$(BUILD_DIR)/main $(FILE)

valgrind: main-build
	 $(VALGRIND) ./$(BUILD_DIR)/main $(FILE)

# === Piece Table Binary ===
pt-build: $(BUILD_DIR)/pt

$(BUILD_DIR)/pt: pt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

pt-run: pt-build
	./$(BUILD_DIR)/pt

pt-valgrind: pt-build
	$(VALGRIND) ./$(BUILD_DIR)/pt

# === Cleanup ===
clean:
	rm -f $(BUILD_DIR)/*

