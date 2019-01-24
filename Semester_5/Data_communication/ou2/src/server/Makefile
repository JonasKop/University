CC=@ gcc

CFLAGS= -pthread -std=gnu11 -pthread -g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code

all: server

PROTOCOLS=../protocols/protocols.o ../protocols/serialize.o ../protocols/deserialize.o

server: server.o queue.o 
	make -C ../protocols all
	$(CC) -o server server.o queue.o $(PROTOCOLS) $(CFLAGS) 
server.o: server.c server.h ../protocols/protocols.h
	$(CC) -c server.c $(CFLAGS)
queue.o: queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)	
clean: 
	find -name '*.o' -delete && rm -f server
