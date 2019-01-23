CC=gcc
CFLAGS= -pthread -std=gnu11 -pthread -g -Wall -pedantic -ansi -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code

all: mfind

mfind: mfind.o stack.o list.o
	$(CC) -o mfind mfind.o stack.o list.o $(CFLAGS)
mfind.o: mfind.c mfind.h
	$(CC) -c mfind.c $(CFLAGS) -D_DEFAULT_SOURCE
stack.o: stack.c stack.h
	$(CC) -c stack.c $(CFLAGS)
list.o: list.c list.h 
	$(CC) -c list.c $(CFLAGS)
clean: 
	rm -rf *o mfind
