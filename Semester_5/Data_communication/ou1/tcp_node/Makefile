CC=gcc
CFLAGS= -pthread -std=gnu11 -pthread -g #-Wall -pedantic -ansi -std=gnu11 -Wall -Wextra -Werror -Wmissing-declarations -Wmissing-prototypes -Werror-implicit-function-declaration -Wreturn-type -Wparentheses -Wunused -Wold-style-definition -Wundef -Wshadow -Wstrict-prototypes -Wswitch-default -Wunreachable-code
DIRS=tcp 

all: tcp_node

tcp_node: main.o queue.o thread_options.o tcp_client.o tcp_server.o
	$(CC) -o tcp_node main.o queue.o thread_options.o tcp_server.o tcp_client.o $(CFLAGS) 

#Main
main.o: main.c queue.h tcp_server.h tcp_client.h thread_options.h
	$(CC) -c main.c $(CFLAGS)

#Other
queue.o: queue.c queue.h
	$(CC) -c queue.c $(CFLAGS)
thread_options.o: thread_options.c thread_options.h
	$(CC) -c thread_options.c $(CFLAGS)

#TCP
tcp_client.o: tcp_client.c tcp_client.h
	$(CC) -c tcp_client.c $(CFLAGS)
tcp_server.o: tcp_server.c tcp_server.h
	$(CC) -c tcp_server.c $(CFLAGS)
clean: 
	find -name '*.o' -delete && rm tcp_node
