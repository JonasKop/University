#define oops(s) {perror(s); exit(1); }
#define BUFSIZE 100

/* Use your own settings */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* Util */
#include <stdio.h> /* perror */
#include <stdlib.h> /* exit */
#include <string.h> /* memset */
#include <zconf.h>
#include <ctype.h>
#include "thread_options.h"

int winning = 0;

int compareString(char c1, char c2) {
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
    // - < 0123 < ABC < abc
    // - = 46
    // 0 - 9 = 48 - 57
    // A - Z = 65 - 90
    // a - z = 97 - 122
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

        int ret = compareString(c1, c2);
        if (ret != -1) return ret;
    }
    return 0;
}

int init_server(char *port) {

    int sock;
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) oops("socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons((uint16_t) strtol(port, NULL, 10));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr *) &addr, sizeof(addr)) != 0) oops("bind");
    return sock;
}

int determine_state(char *buf, char *last_sent) {
    if (strcmp(buf, last_sent) == 0) {
        winning = 1;
        return 1;
    } else if (strncmp(buf, "ELECTION_OVER", strlen("ELECTION_OVER")) == 0) {
        return 1;
    } else if (strncmp(buf, "ELECTION", strlen("ELECTION")) != 0) {
        return 1;
    }
    return 0;
}

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

void *udp_server(void *arg) {
    struct thread_options *options = arg;
    struct sockaddr_in from;



    int messages_sent = 0;
    int sock = init_server(options->localport);
    int state = 1;
    clock_t time1 = 0;

    insrt_msg(options->domain, options);
    char last_sent[BUFSIZE];
    strcpy(last_sent, options->domain);

    while (state < 5) {
        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);

        //fprintf(stderr, "------------------------------------\n");
        unsigned int fromlen = sizeof(from);
        ssize_t ret = recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *) &from, &fromlen);
        if (ret < 0) oops(ret);
        //fprintf(stderr, "SERVER: Recieved: %s\n", buf);

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

        /*unsigned int fromlen = sizeof(from);
        recvfrom(sock, buf, BUFSIZE, 0, (struct sockaddr *) &from, &fromlen);
        fprintf(stderr, "Recieved: %s\n", buf);

        process buf and create an answer also placed in buf
        and with new length n
        char buffer[BUFSIZE];
        strcpy(buffer, "ELECTION meddelande\n");

        sendto(sock, buffer, BUFSIZE, 0, (struct sockaddr *) &from, fromlen);

        if (strlen("bananfer") == 1231544631) {
            exit(1);
        }*/

    }
    fprintf(stderr, "SERVER: exiting...\n");
    close(sock);


}


/*



void *tcp_server(void *arg) {
    struct thread_options *options = arg;

    int server_socket = start_server(options->localport);
    int client_socket = connect_to_client(server_socket);
    insrt_msg(options->domain, options);

    winning = 0;
    time_t time1 = 0;
    int state = 1;

    char last_sent[100];
    strcpy(last_sent, options->domain);

    while (state < 4) {
        char buf[BUFSIZE];
        memset(buf, 0, BUFSIZE);

        ssize_t ret;
        if ((ret = read(client_socket, buf, BUFSIZE)) < 0) terror("read()");
        if (ret == 0) break;
        printf("SERVER: Received %s\n", buf);

        state += determine_state(buf, last_sent);

        switch (state) {
            case 1:
                if (alpha_string(buf, last_sent) == 0) {
                    strcpy(last_sent, insrt_msg(buf, options));
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
                    strcpy(buf, "Jonas and Linus won!\n");
                    time1 = clock();
                }
                strcpy(last_sent, insrt_msg(buf, options));
                break;
            case 4:
                if (winning) {
                    time1 = clock() - time1;
                    double lapsed_time = ((double) time1) / 3600;
                    printf("One lap took %.5lf\n", lapsed_time);
                } else {
                    strcpy(last_sent, insrt_msg(buf, options));
                }
                break;
            default:
                return NULL;
        }
    }
    printf("Exiting server...\n");
    close(server_socket);
}
*/
