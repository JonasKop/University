CC=gcc
CFLAGS= -pthread -std=gnu11 -pthread -g #-Wall -pedantic -ansi -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code
DIRS=udp 

all: udp_node

udp_node: main.o queue.o thread_options.o udp_client.o udp_server.o
	$(CC) -o udp_node main.o queue.o thread_options.o udp_server.o udp_client.o $(CFLAGS) 

#Main
main.o: main.c queue.h udp_server.h udp_client.h thread_options.h
	$(CC) -c main.c $(CFLAGS)

#Other
queue.o: queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)
thread_options.o: thread_options.c thread_options.h
	$(CC) -c thread_options.c $(CFLAGS)

#UDP
udp_client.o: udp_client.c udp_client.h
	$(CC) -c udp_client.c $(CFLAGS)
udp_server.o: udp_server.c udp_server.h
	$(CC) -c udp_server.c $(CFLAGS)
clean: 
	find -name '*.o' -delete && rm udp_node
