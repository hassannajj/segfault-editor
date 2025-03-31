# Default file is test.txt if no file is provided
FILE ?= test.txt

INCLUDE_DIR = safe_memory/include
LIB_DIR = safe_memory/lib

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(INCLUDE_DIR)
LDFLAGS = -L$(LIB_DIR) -lsafe_memory

build:
	$(CC) $(CFLAGS) main.c $(LDFLAGS) -o main 

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main $(FILE)

run:
	./main $(FILE) 

clean:
	rm -f main

