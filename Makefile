# === Configurable Defaults ===
FILE ?= test1.txt

# === Directories ===
BUILD_DIR = build
SAFE_MEMORY_DIR = lib/safe_memory
UNITY_DIR = lib/unity

# === Compiler & Flags ===
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(SAFE_MEMORY_DIR) -I$(UNITY_DIR)
LDFLAGS = -L$(SAFE_MEMORY_DIR) -lsafe_memory

# === Valgrind Settings ===
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# === Build Directory Rule ===
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Default Target ===
all: main-build

# === Main Program ===
main-build: $(BUILD_DIR)/main

$(BUILD_DIR)/main: main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

run: main-build
	./$(BUILD_DIR)/main $(FILE)

valgrind: main-build
	$(VALGRIND) ./$(BUILD_DIR)/main $(FILE)

# === Piece Table Program ===
pt-build: $(BUILD_DIR)/pt

$(BUILD_DIR)/pt: pt.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

pt-run: pt-build
	./$(BUILD_DIR)/pt

pt-valgrind: pt-build
	$(VALGRIND) ./$(BUILD_DIR)/pt

# === Unit Testing with Unity ===
TEST_SRC = tests/test_pt.c
UNITY_SRC = $(UNITY_DIR)/unity.c
SRC = pt.c
TEST_BIN = $(BUILD_DIR)/test_runner

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(UNITY_SRC) $(SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

test-valgrind: $(TEST_BIN)
	$(VALGRIND) ./$(TEST_BIN)

# === Cleanup ===
clean:
	rm -f $(BUILD_DIR)/*

