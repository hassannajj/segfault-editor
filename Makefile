# === Configurable Defaults ===
FILE ?= test1.txt

# === Directories ===
BUILD_DIR = build
SAFE_MEMORY_DIR = lib/safe_memory
INCLUDE_DIR = include
UNITY_DIR = lib/unity


# === Compiler & Flags ===
CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(SAFE_MEMORY_DIR) -I$(INCLUDE_DIR) -I$(UNITY_DIR)
LDFLAGS = -L$(SAFE_MEMORY_DIR) -lsafe_memory -lncurses

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

$(BUILD_DIR)/pt: pt.c src/pt_core.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) pt.c src/pt_core.c $(LDFLAGS) -o $@

pt-run: pt-build
	./$(BUILD_DIR)/pt

pt-valgrind: pt-build
	$(VALGRIND) ./$(BUILD_DIR)/pt

# === Ncurses ===
NCURSES_SRC = src/nc.c 
PT_SRC = src/pt_core.c
INPUT_SRC = src/input.c
NCURSES_BIN = $(BUILD_DIR)/nc

nc-build: $(BUILD_DIR)/nc 

$(BUILD_DIR)/nc: $(NCURSES_SRC)| $(BUILD_DIR)
	$(CC) $(CFLAGS) $(NCURSES_SRC) $(PT_SRC) $(INPUT_SRC) $(LDFLAGS) -o $@

nc-run:
	./build/nc



# === Unit Testing with Unity ===
UNITY_SRC = $(UNITY_DIR)/unity.c
UNITY_INC = lib/unity

TEST_SRC = tests/test_pt.c
CORE_SRC = src/pt_core.c
TEST_BIN = $(BUILD_DIR)/test_runner

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) $(UNITY_SRC) $(CORE_SRC) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

test-valgrind: $(TEST_BIN)
	$(VALGRIND) ./$(TEST_BIN)

# === Cleanup ===
clean:
	rm -f $(BUILD_DIR)/*

