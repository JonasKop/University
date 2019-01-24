//
//  main.c
//  DOD_OU1
//
//  Created by Jonas Sjödin on 2018-09-07.
//  Copyright © 2018 Jonas Sjödin Linus Ström. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "queue.h"
#include "udp_server.h"
#include "udp_client.h"
#include "thread_options.h"

#include <unistd.h>
#include <netdb.h>
#include <string.h>
#include <ctype.h>

int getFQDN(char *fqdn, size_t n) {
    char hostname[256];
    int r = gethostname(hostname, 256);
    if (r != 0) return 1;

    struct addrinfo h = {0}, *info;
    h.ai_family = AF_INET;
    h.ai_socktype = SOCK_STREAM;
    h.ai_flags = AI_CANONNAME;
    if (getaddrinfo(hostname, NULL, &h, &info) != 0) {
        return 2;
    }
    strncpy(fqdn, info->ai_canonname, n);
    freeaddrinfo(info);
    return 0;
}

int main(int argc, const char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage %s <localport> <host> <port>\n", argv[0]);
        return 1;
    }

    //Check return value
    struct thread_options *options = malloc(sizeof(*options));
    options->localport = (char *) argv[1];
    options->nexthost = (char *) argv[2];
    options->nextport = (char *) argv[3];

    options->q = queue_create();
    options->m = malloc(sizeof(pthread_mutex_t));
    options->p_cond = malloc(sizeof(pthread_cond_t));
    pthread_mutex_init(options->m, NULL);
    pthread_cond_init(options->p_cond, NULL);

    printf("----------------------------\n"
           "Listening to port: %s\n"
           "Sending to host: %s\n"
           "Sending to port: %s\n"
           "----------------------------\n", options->localport, options->nexthost, options->nextport);

    char domain[256];
    getFQDN(domain, 256);

    char hostname[100];
    strcpy(hostname, "ELECTION\n");
    strcat(hostname, domain);
    strcat(hostname, ",");
    strcat(hostname, options->localport);
    strcat(hostname, "\n");

    options->domain = calloc(BUFSIZE, 1);
    strcpy(options->domain, hostname);

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, &udp_server, (void *) options);
    pthread_create(&threads[1], NULL, &udp_client, (void *) options);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    pthread_cond_destroy(options->p_cond);
    pthread_mutex_destroy(options->m);

    queue_free(options->q);
    free(options->domain);
    free(options->p_cond);
    free(options->m);
    free(options);

    return 0;
}
