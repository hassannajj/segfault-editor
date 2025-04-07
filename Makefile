.PHONY: build run valgrind clean

#
# Default file is test.txt if no file is provided
FILE ?= test/test1.txt

SAFE_MEM_INCLUDE = include/safe_memory/
SAFE_MEM_LIB = lib/safe_memory/

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -I$(SAFE_MEM_INCLUDE)
LDFLAGS = -L$(SAFE_MEM_LIB) -lsafe_memory -lncurses

# main 
build:
	$(CC) $(CFLAGS) src/main.c $(LDFLAGS) -o ./build/main 

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/main "$(FILE)"

run:
	./build/main "$(FILE)" 

clean:
	rm -f build/main


# ncurses development
nc-build:
	$(CC) $(CFLAGS) src/nc.c $(LDFLAGS) -o ./build/nc

nc-run:
	./build/nc 

nc-clean:
	rm -f build/nc

nc-test-build:
	$(CC) $(CFLAGS) src/nc-test.c $(LDFLAGS) -o ./build/nc-test

nc-test-run:
	./build/nc-test	



