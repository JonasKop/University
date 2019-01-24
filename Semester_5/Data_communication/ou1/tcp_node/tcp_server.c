#include "tcp_server.h"
#include "thread_options.h"

#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <netdb.h>

#define terror(s) ({perror(s); exit(1);})

int winning = 0;

int compare_string(char c1, char c2) {
    int upper1 = isupper(c1), upper2 = isupper(c2);

    if (c1 != c2) {
        if (c1 < 64 || c2 < 64) {
            return c1 > c2 ? 0 : 1;
        } else if (!upper1 && !upper2) {
            return c1 > c2 ? 0 : 1;
        } else if (upper1 && upper2) {
            return c1 > c2 ? 0 : 1;
        } else {
            if (isupper(c1)) {
                return c1 >= c2 - 32 ? 0 : 1;
            } else {
                return c2 >= c1 - 32 ? 1 : 0;
            }
        }
    }
    return -1;
}

int alpha_string(char *str1, char *str2) {
    size_t strlen1 = strlen(str1);
    size_t strlen2 = strlen(str2);
    size_t smallest_size = strlen1 < strlen2 ? strlen1 : strlen2;

    if (strcmp(str1, str2) == 0) return 0;

    for (int i = 9; i < smallest_size; ++i) {
        char c1 = str1[i], c2 = str2[i];

        if (c1 == '.') {
            return 0;
        } else if (c2 == '.') {
            return 1;
        }

        int ret = compare_string(c1, c2);
        if (ret != -1) return ret;
    }
    return 0;
}

int start_server(char *port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) terror("socket()");

    int reuseaddr = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
                   sizeof(reuseaddr)) == -1) terror("setsockopt");

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons((uint16_t) strtol(port, NULL, 10));

    if (bind(server_socket, (struct sockaddr *) &addr, sizeof(addr)) < 0)
        terror("bind()");

    int backlog = 1;
    listen(server_socket, backlog);

    return server_socket;
}

/*
 * connect_to_client()
 * ----------------------------------------------------------------------------
 * Description: Connects to a client through a socket
 * ----------------------------------------------------------------------------
 * INPUT:   server_socket: the servers socket to connect to.
 *          options: the given thread options, containing queue, mutex etc
 * ----------------------------------------------------------------------------
 * OUTPUT: returns the buffer
 */
int connect_to_client(int server_socket) {
    int client_socket = accept(server_socket, NULL, NULL);

    if (client_socket < 0)
        terror("accept");

    return client_socket;
}

/*
 * insrt_msg()
 * ----------------------------------------------------------------------------
 * Description: Inserts a message in the queue and signals the client if needed
 * ----------------------------------------------------------------------------
 * INPUT:   buf: the buffer containing the message that should be inserted to
 *               the queue
 *          options: the given thread options, containing queue, mutex etc
 * ----------------------------------------------------------------------------
 * OUTPUT: returns the buffer
 */
char *insrt_msg(char *buf, struct thread_options *options) {
    char *buffer = calloc(BUFSIZE, 1);
    strcpy(buffer, buf);

    pthread_mutex_lock(options->m);
    if (queue_is_empty(options->q)) {
        queue_push(options->q, buffer);
        pthread_cond_signal(options->p_cond);
    }
    pthread_mutex_unlock(options->m);

    return buf;
}

/*
 * determine_state()
 * ----------------------------------------------------------------------------
 * Description: Determines the state of the server.
 * ----------------------------------------------------------------------------
 * INPUT:   buf: the buffer that should be returned with new msg
 * ----------------------------------------------------------------------------
 * OUTPUT:  change buf pointer
 */
int determine_state(char *buf, char *first_msg) {
    if (strcmp(first_msg, buf) == 0) {
        printf("Winning\n");
        winning = 1;
        return 1;
    } else if (strncmp(buf, "ELECTION_OVER", strlen("ELECTION_OVER")) == 0) {
        return 1;
    } else if (strncmp(buf, "ELECTION", strlen("ELECTION")) != 0) {
        return 1;
    }
    return 0;
}

/*
 * tcp_server()
 * ----------------------------------------------------------------------------
 * Description: main function for tcp_server. Recieves messages and handles
 * them. Signals to Client when it can send a message.
 * ----------------------------------------------------------------------------
 * INPUT:   arg: The thread options for the thread. Including first_msg,
 *               localport etc.
 * ----------------------------------------------------------------------------
 * OUTPUT:  NULL
 */
void *tcp_server(void *arg) {

    struct thread_options *options = arg;

    insrt_msg(options->domain, options);

    winning = 0;
    time_t time1 = 0;
    int state = 1;

    char last_sent[100];
    strcpy(last_sent, options->domain);
    int messages_sent = 0;

    int server_socket = start_server(options->localport);
    int client_socket = connect_to_client(server_socket);

    while (state < 5) {
        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);

        if (read(client_socket, buf, BUFSIZE) < 0) terror("read()");

        if ((!winning && state < 3) || (winning && state < 4)) {
            state += determine_state(buf, options->domain);
        }

        switch (state) {
            case 1:
                if (alpha_string(buf, last_sent) == 0) {
                    strcpy(last_sent, insrt_msg(buf, options));
                } else {
                    insrt_msg(last_sent, options);
                }
                break;
            case 2:
                if (winning) {
                    strcpy(buf, "ELECTION_OVER");
                    char *new_msg = strchr(last_sent, '\n');
                    strcat(buf, new_msg);
                    strcpy(last_sent, insrt_msg(buf, options));
                } else {
                    strcpy(last_sent, insrt_msg(buf, options));
                }

                break;
            case 3:
                if (winning) {
                    strcpy(buf, "MESSAGE\nJonas and Linus won!\n");
                    strcpy(last_sent, buf);
                    time1 = clock();
                }
                strcpy(last_sent, insrt_msg(buf, options));
                break;
            case 4:
                if (winning) {
                    messages_sent++;
                    if (messages_sent == 10000) {
                        messages_sent = 0;
                        time1 = clock() - time1;
                        double lapsed_time = ((double) time1) / (3600 * 10);
                        printf("10000 laps, average time per lap: %.3lfms\n", lapsed_time);
                        time1 = clock();
                    }

                }
                strcpy(last_sent, insrt_msg(buf, options));

                break;
            default:
                return NULL;
        }
    }
    printf("Exiting server...\n");
    close(server_socket);
    return NULL;
}
