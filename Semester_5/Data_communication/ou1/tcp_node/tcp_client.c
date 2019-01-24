#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "thread_options.h"

int connect_to_server(int client_socket, struct addrinfo *res) {
    while (1) {

        int status = connect(client_socket, res->ai_addr, res->ai_addrlen);
        if (status == 0) {
            printf("Connection successful!\n");
            return client_socket;
        }
        printf("Connection failed, retrying...\n");
        sleep(1);
    }
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

void *tcp_client(void *arg) {
    struct thread_options *options = arg;

    struct sockaddr_in addr;
    int sock;

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

    if (getaddrinfo(options->nexthost, options->nextport, &hints,
                              &res) < 0)
        terror("getaddrinfo()");

    connect_to_server(sock, res);


    while (1) {
        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);
        get_msg(buf, options);
        sendto(sock, buf, BUFSIZE, 0, res->ai_addr, res->ai_addrlen);
    }
    close(sock);
    freeaddrinfo(res);
    printf("Exiting client...\n");
    return NULL;
}
