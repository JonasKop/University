CC=gcc
CFLAGS=-Wall -pedantic -ansi -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code -g

all: mpasswdsort 

mpasswdsort: mpasswdsort.o list.o
	$(CC) -o mpasswdsort mpasswdsort.o list.o $(CFLAGS);
	clear
mpasswdsort.o: mpasswdsort.c
	$(CC) -c mpasswdsort.c $(CFLAGS)

list.o: list.c
	$(CC) -c list.c $(CFLAGS)

clean:
	rm -rf *o mpasswdsort
