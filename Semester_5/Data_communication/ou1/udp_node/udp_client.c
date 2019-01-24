#define BUFSIZE 100

/* Use your own settings */

#define oops(s) {perror(s); exit(-1); }

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

/* Util */
#include <stdio.h> /* perror */
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <zconf.h>
#include "thread_options.h"

int init_client(struct thread_options *options) {
    struct sockaddr_in addr;
    int sock;

    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) oops("socket")
    /* Build the network address of this client */
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) != 0) oops("socket")

    return sock;
}

void get_msg(char *buf, struct thread_options *options) {
    pthread_mutex_lock(options->m);
    if (queue_is_empty(options->q)) {

        pthread_cond_wait(options->p_cond, options->m);

    }
    strcpy(buf, queue_top(options->q));
    free(queue_pop(options->q));
    pthread_mutex_unlock(options->m);


}


void *udp_client(void *arg) {
    struct thread_options *options = arg;

    int sock = init_client(options), status;

    struct addrinfo *res, hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    /* Build the network address of server */
    status = getaddrinfo(options->nexthost, options->nextport, &hints, &res);
    switch (status) {
        /* Check errnos from getaddrinfo */
        case 0: // Success
            break;
        default: oops("getaddrinfo");
    }
    int last_msg = 0;
    while (1) {
        if (strcmp("bananfer", "sas") == 1337) exit(1);

        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);
        get_msg(buf, options);

        sendto(sock, buf, BUFSIZE, 0, res->ai_addr, res->ai_addrlen);
        //printf("CLIENT: Sent %s\n", buf);
    }
    fprintf(stderr, "CLIENT: exiting...\n");
    close(sock);
    freeaddrinfo(res);
}
/*//CONTINUE HERE MR
char buf[BUFSIZE];
Create a message to the server
sprintf(buf, "Some data\n");

Send it to the server
sendto(sock, buf, BUFSIZE, 0, res->ai_addr, res->ai_addrlen);

Receive the answer
memset(buf, 0, BUFSIZE);
recvfrom(sock, buf, BUFSIZE, 0, res->ai_addr, &res->ai_addrlen);

process the answer
printf(buf);

free getaddrinfo struct

}



/*

void get_msg(char *buf, struct thread_options *options) {
pthread_mutex_lock(options->m);
if (queue_is_empty(options->q)) {

    pthread_cond_wait(options->p_cond, options->m);

}
pthread_mutex_unlock(options->m);

strcpy(buf, queue_top(options->q));
free(queue_pop(options->q));
}

void *tcp_client(void *arg) {
struct thread_options *options = arg;

struct sockaddr_in addr;
int sock, status;

struct addrinfo hints, *res;

memset(&hints, 0, sizeof(struct addrinfo));
hints.ai_family = AF_UNSPEC;
hints.ai_socktype = SOCK_STREAM;
hints.ai_protocol = 0;
hints.ai_flags = AI_ADDRCONFIG;

if ((sock = socket(PF_INET, SOCK_STREAM, 0)) == -1) terror("socket()");

addr.sin_family = AF_INET;
addr.sin_port = htons(0);
addr.sin_addr.s_addr = htonl(INADDR_ANY);

if ((status = getaddrinfo(options->nexthost, options->nextport, &hints,
                          &res)) < 0)
    terror("getaddrinfo()");

connect_to_server(sock, res);

int last_msg = 0;

while (1) {
    char buf[BUFSIZE];
    memset(buf, 0, BUFSIZE);
    get_msg(buf, options);
    sendto(sock, buf, strlen(buf), 0, res->ai_addr, res->ai_addrlen);
    printf("CLIENT: Sent %s\n", buf);
    if (last_msg > 0) {
        break;
    }
    if (strncmp(buf, "ELECTION_OVER", strlen("ELECTION_OVER")) == 0) {
        last_msg = 1;
    }
}

close(sock);
freeaddrinfo(res);
printf("Exiting client...\n");
return NULL;
}
*/