# Default file is test.txt if no file is provided
FILE ?= test.txt

main:
	gcc -g -o main main.c -Wall -Wextra -pedantic -std=c99

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main $(FILE)

clean:
	rm -f main

