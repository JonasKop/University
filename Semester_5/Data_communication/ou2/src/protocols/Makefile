CC=@ gcc
CFLAGS= -pthread -std=gnu11 -pthread -g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

all: serialize.o deserialize.o protocols.o

serialize.o: serialize.c serialize.h deserialize.h protocols.o
	$(CC) -c serialize.c $(CFLAGS)
deserialize.o: deserialize.c deserialize.h serialize.h protocols.o
	$(CC) -c deserialize.c $(CFLAGS)
protocols.o: protocols.c protocols.h
	$(CC) -c protocols.c $(CFLAGS)

test: testpdu.c deserialize.o serialize.o protocols.o
	$(CC) -o testpdu testpdu.c protocols.o serialize.o deserialize.o $(CFLAGS)

rec_msg: deserialize.o serialize.o protocols.o
	$(CC) -c protocols.o serialize.o deserialize.o $(CFLAGS)

clean: 
	find -name '*.o' -delete && rm -f testpdu
