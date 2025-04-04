.PHONY: build run valgrind clean

#
# Default file is test.txt if no file is provided
FILE ?= test/test1.txt

INCLUDE_DIR = lib/safe_memory/include
LIB_DIR = lib/safe_memory/lib

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsafe_memory

build:
	$(CC) $(CFLAGS) src/main.c $(LDFLAGS) -o ./build/main 

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/main "$(FILE)"

run:
	./build/main "$(FILE)" 

clean:
	rm -f build/*

