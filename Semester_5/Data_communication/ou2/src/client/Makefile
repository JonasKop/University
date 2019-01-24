CC=@ gcc
CFLAGS= -pthread -std=gnu11 -pthread -g -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -Wcast-qual -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code
all: client

PROTOCOLS=../protocols/protocols.o ../protocols/serialize.o ../protocols/deserialize.o 

client: client.o queue.o modules.o
	make -C ../protocols all
	$(CC) -o client client.o queue.o modules.o $(PROTOCOLS) $(CFLAGS) 
client.o: client.c client.h ../protocols/protocols.h modules.h
	$(CC) -c client.c modules.c $(CFLAGS)
queue.o: ../server/queue.c ../server/queue.h ../protocols/protocols.h
	$(CC) -c ../server/queue.c $(CFLAGS)
modules.o: modules.c modules.h
	$(CC) -c modules.c $(CFLAGS)
clean: 
	find -name '*.o' -delete && rm -f send_msg
