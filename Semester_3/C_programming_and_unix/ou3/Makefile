CC=gcc
CFLAGS=-std=c11 -Wall -pedantic -ansi -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code -g

all: mish

mish: mish.o parser.o execute.o sighant.o
	$(CC) -o mish mish.o parser.o execute.o sighant.o $(CFLAGS)
mish.o: mish.c mish.h execute.h sighant.h
	$(CC) -c mish.c $(CFLAGS)
parser.o: parser.c parser.h
	$(CC) -c parser.c $(CFLAGS)
execute.o: execute.c execute.h
	$(CC) -c execute.c $(CFLAGS)
sighant.o: sighant.c sighant.h mish.h
	$(CC) -c sighant.c $(CFLAGS)
clean: 
	rm -rf *o mish
