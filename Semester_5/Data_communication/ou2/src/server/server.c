/*
 * A server which can be connected to by tcp clients. The server connects to
 * a nameserver where its information can be gotten and connected to by
 * clients. Then it handles the messages according to the give specification
 * about PDUs etc in Assignment 2 of the course Computer communication and
 * computer networks given at Umeå University.
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#include <sys/ioctl.h>
#include "server.h"

typedef struct thread_t {
    int sock, state;
    char *client_name;
    pthread_mutex_t mutex;
} thread;

typedef struct nameserver_op_t {
    int socket, state;
    pthread_mutex_t *nameserver_mutex, *active_clients_mutex;
} nameserver_op;

typedef struct thread_op_t {
    int lower_range, higher_range;
    pthread_mutex_t *active_clients_mutex;
} thread_op;

char servername[256], port[6], nameserverhost[256], nameserverport[6];

queue *sending_queue;
thread clients[MAXCLIENTS];
pthread_mutex_t terminate_mutex, send_mutex;
pthread_cond_t send_cond;
int terminate, nameserver_id;
uint8_t active_clients = 0;

/**
 * Catches signal and releases all condition variables and sets terminate to 1.
 * Not all threads should terminate.
 *******************************************************************************
 * @param signo the signal which should be SIGINT
 */
void handle_signal(int signo) {

    if (signo == SIGINT) {
        pthread_mutex_lock(&terminate_mutex);
        terminate = 1;
        pthread_mutex_unlock(&terminate_mutex);

        pthread_mutex_lock(&send_mutex);
        pthread_cond_broadcast(&send_cond);
        pthread_mutex_unlock(&send_mutex);
    }
}

/**
 * Checks if the server should terminate and returns the result.
 *******************************************************************************
 * @return if thread should terminate
 */
int should_terminate(void) {
    pthread_mutex_lock(&terminate_mutex);
    int ret = terminate;
    pthread_mutex_unlock(&terminate_mutex);
    return ret;
}

/**
 * Checks the state for the nameserver and returns i1 == i2.
 *******************************************************************************
 * @param m the state mutex
 * @param i1 state
 * @param i2 other state
 * @return
 */
int check_state(pthread_mutex_t *m, int i1, int i2) {
    pthread_mutex_lock(m);
    int ret = i1 == i2;
    pthread_mutex_unlock(m);
    return ret;
}

/**
 * Creates a REG and sets its value to a given pdu
 *******************************************************************************
 * @param p the pdu where the REG should be stored at
 */
void create_REG(pdu *p) {
    REG_msg *reg = malloc(sizeof(REG_msg));
    reg->port = (uint16_t) strtol(port, NULL, 10);
    reg->len = (uint8_t) strlen(servername);
    reg->server_name = malloc((unsigned int) reg->len + 1);
    strcpy(reg->server_name, servername);

    p->msg = reg;
    p->op = REG;
}

/**
 * Creates a ALIVE and sets its value to a given pdu
 *******************************************************************************
 * @param p the pdu where the ALIVE should be stored at
 */
void create_ALIVE(pdu *p, pthread_mutex_t *name_m, pthread_mutex_t *active_m) {
    ALIVE_msg *alive = malloc(sizeof(ALIVE_msg));
    pthread_mutex_lock(name_m);
    alive->id = (uint16_t) nameserver_id;
    pthread_mutex_unlock(name_m);

    pthread_mutex_lock(active_m);
    alive->nr_of_clients = active_clients;
    pthread_mutex_unlock(active_m);
    p->msg = alive;
    p->op = ALIVE;
}

/**
 * Sends REG or ALIVE to nameserver via UDP depending on which state the
 * rec_nameserver is at.
 *******************************************************************************
 * @param arg the nameserver options which contains mutexes and sockets
 * @return NULL on completion
 */
void *send_nameserver(void *arg) {

    nameserver_op *op = arg;


    while (!should_terminate()) {
        pdu *p = malloc(sizeof(pdu));

        if (check_state(op->nameserver_mutex, op->state, 0))
            create_REG(p);
        else
            create_ALIVE(p, op->nameserver_mutex, op->active_clients_mutex);

        if (serialize_PDU(op->socket, p) == -1) {
            fprintf(stderr, "Could not send message which is weird\n");
            destroy_PDU(p);
            break;
        }
        destroy_PDU(p);
        sleep(1);
    }

    return NULL;
}

/**
 * Initiates the nameserver via UDP and binds the connection and connects so
 * read() and send() can be used
 *******************************************************************************
 * @return the socket for which the nameserver should connect to
 */
int init_nameserver(void) {
    int sock;
    struct addrinfo hints, *dstinfo, *srcinfo;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(nameserverhost, nameserverport, &hints, &dstinfo) <
        0) terror("getaddrinfo()");
    if ((sock = socket(dstinfo->ai_family, dstinfo->ai_socktype,
                       dstinfo->ai_protocol)) < 0) terror("sock()")
    if (getaddrinfo(NULL, port, &hints, &srcinfo) < 0) terror("getaddrinfo()");
    if (bind(sock, srcinfo->ai_addr, srcinfo->ai_addrlen) < 0) terror("bind()");
    if (connect(sock, dstinfo->ai_addr, dstinfo->ai_addrlen) < 0) terror(
            "connect()");

    freeaddrinfo(dstinfo);
    freeaddrinfo(srcinfo);
    return sock;
}

/**
 * Runs after an ACK has been recieved and sets id to ack.id and state to 1
 * which makes the sending thread send ALIVEs
 *******************************************************************************
 * @param p the pdu containing the ACK
 * @param name_m the nameserver mutex
 */
void rec_ACK(pdu *p, pthread_mutex_t *name_m, int *state) {

    pthread_mutex_lock(name_m);
    nameserver_id = ((ACK_msg *) p->msg)->id;
    fprintf(stderr, "Recieved: ACK with ID: %d\n", nameserver_id);
    *state = 1;
    pthread_mutex_unlock(name_m);
}

/**
 * Runs after a NOTREG has been recieved and sets state to 0 which makes the
 * sending thread send REGs
 *******************************************************************************
 * @param name_m the nameserver mutex
 */
void rec_NOTREG(pthread_mutex_t *name_m, int *state) {
    fprintf(stderr, "Recieved: NOTREG\n");
    pthread_mutex_lock(name_m);
    *state = 0;
    pthread_mutex_unlock(name_m);
}

/**
 * Runs the connection to the nameserver which listens for ACK and NOTREG and
 * sends REG and ALIVE every 8 seconds.
 *******************************************************************************
 * @param arg containing the active_clients_mutex
 * @return NULL
 */
void *run_nameserver(void *arg) {
    pthread_mutex_t name_m;
    pthread_mutex_init(&name_m, NULL);

    int sock = init_nameserver();

    pthread_t sender_thread;


    nameserver_op op = {
            .socket = sock,
            .nameserver_mutex = &name_m,
            .active_clients_mutex = arg,
            .state = 0
    };

    pthread_create(&sender_thread, NULL, send_nameserver, &op);


    fd_set readfds;
    while (!should_terminate()) {

        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        struct timeval tv = {0, 800000};
        int activity = select(sock + 1, &readfds, NULL, NULL, &tv);
        if (activity < 0) {
            terror("select()");
        } else if (activity == 0) {
            continue;
        }

        uint8_t buffer[BUFSIZE];
        memset(buffer, 0, BUFSIZE);
        int status, ind = 0;

        usleep(200000);

        if (read(sock, buffer, BUFSIZE) < 0) {
            terror("read()");
        }


        while (!should_terminate()) {
            pdu *p = retrieve_PDU(sock, buffer, &status, &ind);
            //  fprintf(stderr, "Recieved ns msg\n");
            if (p == NULL)
                fprintf(stderr, "Nameserver: Corrupt message\n");
            else if (p->op == ACK)
                rec_ACK(p, &name_m, &op.state);
            else if (p->op == NOTREG)
                rec_NOTREG(&name_m, &op.state);
            else
                fprintf(stderr, "Nameserver recieved an invalid pdu with op: "
                                "%d\n", p->op);

            destroy_PDU(p);

            if (status != -2)
                break;

        }
    }

    pthread_join(sender_thread, NULL);
    pthread_mutex_destroy(&name_m);
    close(sock);
    return NULL;
}

/**
 * Initiates the servers TCP master socket which listens for new incoming
 * connections which the server handles.
 *******************************************************************************
 * @param address The address which is initialized
 * @param addrlen Where the size of address will be stored
 * @return the socket where the server listens to new connections
 */
int init_connection(struct sockaddr_in *address, int *addrlen) {

    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) terror("sock()");

    int opt = 1;

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,
                   sizeof(opt)) < 0) terror("setsockopt()");

    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons((uint16_t) strtol(port, NULL, 10));

    if (bind(sock, (struct sockaddr *) address, sizeof(*address)) < 0) terror(
            "bind()");

    if (listen(sock, MAXCLIENTS) < 0) terror("listen()");

    *addrlen = sizeof(address);
    fprintf(stdout, "Finished setting up server\n");
    return sock;
}

/**
 * Creates a PJOIN which it returns as a pdu
 *******************************************************************************
 * @param i the client index
 * @return the pdu containing a PJOIN
 */
pdu *create_PJOIN(int i) {
    pdu *p = malloc(sizeof(pdu));
    p->op = PJOIN;
    PJOIN_msg *pjoin = malloc(sizeof(PJOIN_msg));
    p->msg = pjoin;

    pjoin->socket = clients[i].sock;
    if (clients[i].client_name == NULL) {
        pjoin->len = 0;
        pjoin->id = NULL;
    } else {
        pjoin->len = (uint8_t) strlen(clients[i].client_name);
        pjoin->id = malloc(strlen(clients[i].client_name) + 1);
        strcpy(pjoin->id, clients[i].client_name);
    }

    pjoin->timestamp = (uint32_t) clock();


    return p;
}

/**
 * Creates a PLEAVE which it returns as a pdu
 *******************************************************************************
 * @param i the client index
 * @return the pdu containing a PLEAVE
 */
pdu *create_PLEAVE(int i) {
    pdu *p = malloc(sizeof(pdu));
    p->op = PLEAVE;
    PLEAVE_msg *pleave = malloc(sizeof(PLEAVE_msg));
    p->msg = pleave;

    if (clients[i].client_name == NULL) {
        pleave->len = 0;
        pleave->id = NULL;
    } else {
        pleave->len = (uint8_t) strlen(clients[i].client_name);
        pleave->id = malloc(strlen(clients[i].client_name) + 1);
        strcpy(pleave->id, clients[i].client_name);
    }

    pleave->timestamp = (uint32_t) clock();


    return p;
}

/**
 * Creates a QUIT which it returns as a pdu
 *******************************************************************************
 * @return the pdu containing a QUIT
 */
pdu *create_QUIT(void) {
    pdu *p = malloc(sizeof(pdu));
    p->op = QUIT;
    p->msg = malloc(sizeof(QUIT_msg));
    return p;
}

/**
 * Creates a MESS which it returns as a pdu
 *******************************************************************************
 * @param p1 the pdu containing the recieved MESS
 * @param i the client index
 * @return the pdu containing a MESS
 */
pdu *create_MESS(pdu *p1, int i) {
    MESS_msg *mess_rec = p1->msg;
    MESS_msg *mess = malloc(sizeof(MESS_msg));
    pdu *p = malloc(sizeof(pdu));
    p->op = MESS;
    p->msg = mess;
    if (clients[i].client_name == NULL) {
        mess->id_len = 0;
        mess->client_id = NULL;
    } else {
        mess->id_len = (uint8_t) strlen(clients[i].client_name);
        mess->client_id = malloc((unsigned int) mess->id_len + 1);
        strcpy(mess->client_id, clients[i].client_name);
    }

    mess->timestamp = (uint32_t) clock();
    mess->msg_len = mess_rec->msg_len;
    mess->checksum = 0;

    mess->msg = malloc((unsigned int) mess->msg_len + 1);
    strcpy(mess->msg, mess_rec->msg);

    return p;
}

/**
 * Creates a PART which it returns as a pdu
 *******************************************************************************
 * @return the pdu containing a PART
 */
pdu *create_PART(void) {
    pdu *p = malloc(sizeof(pdu));
    p->op = PARTICIPANTS;
    PART_msg *part = malloc(sizeof(PART_msg));
    p->msg = part;
    uint8_t nr_of_clients = 0;
    for (int j = 0; j < MAXCLIENTS; ++j) {
        pthread_mutex_lock(&clients[j].mutex);
        if (clients[j].sock > 0 && clients[j].client_name != NULL) {
            nr_of_clients++;
        }
        pthread_mutex_unlock(&clients[j].mutex);
    }
	if (nr_of_clients != 0) {
	    nr_of_clients--;	
	}
    part->ids = calloc(1, sizeof(char *) * ((unsigned int) nr_of_clients + 10));
	
	int len = 0, ind = 0;
	for (int j = 0; j < MAXCLIENTS; ++j) {
		pthread_mutex_lock(&clients[j].mutex);
		if (clients[j].sock > 0) {
            if (clients[j].client_name != NULL) {
	            int length = 0;
		        if (clients[j].client_name != NULL) {
			        length = (int) strlen(clients[j].client_name) + 1;
					if (length == 1) {
						fprintf(stderr, "len is: %d\n", length);
					}
					len += length;
                    part->ids[ind] = calloc(1, (unsigned int) length);
	                strcpy(part->ids[ind], clients[j].client_name);
					ind++;
				}		
			}
		}
		pthread_mutex_unlock(&clients[j].mutex);
	}
    part->nr_of_ids = (uint8_t) ind;
	part->ids_len = (uint16_t) len;

    return p;
}

/**
 * Closes a client connection by closing the socket and setting it to 0,
 * decrements active_clients and if possible frees the client name and then sets
 * it to NULL.
 *******************************************************************************
 * @param i the client1 index
 * @param active_clients_mutex the active clients mutex
 */
void close_connection(int i, pthread_mutex_t *active_clients_mutex) {
    fprintf(stderr, "Closing connection with id: %-3d %s\n", i, clients[i]
            .client_name);
    pthread_mutex_lock(&clients[i].mutex);
    close(clients[i].sock);
    clients[i].sock = 0;
    pthread_mutex_lock(active_clients_mutex);
    active_clients--;
    pthread_mutex_unlock(active_clients_mutex);
    if (clients[i].client_name != NULL)
        free(clients[i].client_name);
    clients[i].client_name = NULL;
    pthread_mutex_unlock(&clients[i].mutex);
}

/**
 * Adds a pdu to the sending queue and signals the send condition variable
 *******************************************************************************
 * @param p the pdu which should be sent.
 */
void add_to_send(pdu *p) {
    pthread_mutex_lock(&send_mutex);
    queue_push(sending_queue, p);
    pthread_cond_signal(&send_cond);
    pthread_mutex_unlock(&send_mutex);
}

/**
 * Runs if new_connection has been recieved, if not 255 clients it is
 * accepted and the client gets a socket and client options are set.
 *******************************************************************************
 * @param sock the master socket
 * @param addrlen the length of address
 * @param address the adressinfo
 * @param active_mutex the active mutex
 */
void new_connection(int sock, int addrlen, struct sockaddr_in *address,
                    pthread_mutex_t *active_mutex) {

    //fprintf(stderr, "Client attempting to connect\n");


    int new_socket;
    if ((new_socket = accept(sock, (struct sockaddr *) address,
                             (socklen_t *) &addrlen)) < 0) terror("accept");


    pthread_mutex_lock(active_mutex);
    active_clients++;
    pthread_mutex_unlock(active_mutex);

    int thread_id = -1;


    for (int i = 0; i < MAXCLIENTS; ++i) {
        pthread_mutex_lock(&clients[i].mutex);
        if (clients[i].sock == 0) {
            clients[i].sock = new_socket;
            clients[i].state = 0;
            clients[i].client_name = NULL;
            thread_id = i;
            pthread_mutex_unlock(&clients[i].mutex);
            break;
        }
        if (i == MAXCLIENTS - 1) {
            fprintf(stderr, "FULL CLIENTS\n");
            close(new_socket);
            pthread_mutex_unlock(&clients[i].mutex);
            break;
        }
        pthread_mutex_unlock(&clients[i].mutex);
    }
}

/**
 * A client has disconnected and a PLEAVE is sent if a JOIN has been recieved
 * before and the connection is closed, else the connection is just closed
 *******************************************************************************
 * @param i the client index
 * @param active_mutex the active mutex
 */
void disconnected_client(int i, pthread_mutex_t *active_mutex) {
    fprintf(stderr, "Client %d has disconnected\n", i);
    if (clients[i].state == 1) {
        pdu *p1 = create_PLEAVE(i);
        fprintf(stderr, "Closing inside 'disconnected_client'\n");
        close_connection(i, active_mutex);
        add_to_send(p1);
    } else {
        fprintf(stderr, "Closing inside 'disconnected_client'\n");
        close_connection(i, active_mutex);
    }
}

/**
 * Creates a QUIT and sends it, if the client has joined with JOIN a PLEAVE
 * will be sent to all other users in the session and closes the connection,
 * else it just closes the connection.
 *******************************************************************************
 * @param i the client index
 * @param active_mutex the active mutex
 */
void bad_PDU(int i, pthread_mutex_t *active_mutex) {
    pdu *p1 = create_QUIT();
    serialize_PDU(clients[i].sock, p1);
    destroy_PDU(p1);

    if (clients[i].state == 1) {
		pdu *p2 = create_PLEAVE(i);
        fprintf(stderr, "Closing inside 'bad_PDU 1'\n");
        close_connection(i, active_mutex);
        add_to_send(p2);
    } else {
        fprintf(stderr, "Closing inside 'bad_PDU 2'\n");
        close_connection(i, active_mutex);
    }
}

/**
 * Creates a PLEAVE and closes the connection and the adds the PLEAVE to the
 * sending queue.
 *******************************************************************************
 * @param i the client index
 * @param active_mutex the active mutex
 */
void recieved_QUIT(int i, pthread_mutex_t *active_mutex) {
    pdu *p1 = create_PLEAVE(i);
    fprintf(stderr, "Closing inside 'revcieved_QUIT'\n");
    close_connection(i, active_mutex);
    add_to_send(p1);
}

/**
 * If a JOIN has not been recieved before it creates a PJOIN which it sends
 * to all users in the session. It also creates a PART which it sends to the
 * new user. Else a JOIN has been recieved after another JOIN which should
 * not happen and a QUIT is sent to the client and a PLEAVE to all users in
 * the active session.
 *******************************************************************************
 * @param join the join message which has been recieved
 * @param i the client index
 * @param active_mutex the active mutex
 */
void recieved_JOIN(JOIN_msg *join, int i, pthread_mutex_t *active_mutex) {
    //fprintf(stderr, "Recieved JOIN\n");
    pthread_mutex_lock(&clients[i].mutex);
    clients[i].client_name = malloc((size_t) join->len + 1);
    if (join->len == 0) {
        clients[i].client_name = NULL;

    } else {
        fprintf(stderr, "Client: %s joined the room\n", join->id);
        strcpy(clients[i].client_name, join->id);
    }
    clients[i].state = 1;
    int sock = clients[i].sock;
    pthread_mutex_unlock(&clients[i].mutex);
    add_to_send(create_PJOIN(i));
    pdu *p2 = create_PART();
    serialize_PDU(sock, p2);

    fprintf(stderr, "SENT PARTICIPANTS\n");
    destroy_PDU(p2);
}

void *client_connection(void *arg) {
    thread_op *op = arg;

    fd_set readfds;
    while (!should_terminate()) {
        int max_sd = -1;
        FD_ZERO(&readfds);
        for (int i = op->lower_range; i < op->higher_range; ++i) {
            pthread_mutex_lock(&clients[i].mutex);
            if (clients[i].sock > 0)
                FD_SET(clients[i].sock, &readfds);
            if (clients[i].sock > max_sd)
                max_sd = clients[i].sock;
            pthread_mutex_unlock(&clients[i].mutex);
        }

        if (max_sd == -1) {
            usleep(800000);
            continue;
        }

        struct timeval tv = {0, 800000};
        int activity = select(max_sd + 1, &readfds, NULL, NULL, &tv);
        if (activity == 0) continue;
        if (activity < 0) terror("select()");

        for (int i = op->lower_range; i < op->higher_range; ++i) {
            if (FD_ISSET(clients[i].sock, &readfds)) {
                usleep(200000);
                uint8_t buffer[BUFSIZE];
                memset(buffer, 0, BUFSIZE);

                if (read(clients[i].sock, buffer, BUFSIZE) <= 0) {
                    disconnected_client(i, op->active_clients_mutex);
                    continue;
                }

                int status, ind = 0;

                while (!should_terminate()) {
                    pdu *p = retrieve_PDU(clients[i].sock, buffer, &status,
                                          &ind);
                    if (status == -1) {
                        bad_PDU(i, op->active_clients_mutex);
                        break;
                    } else if (clients[i].state == 0 && p->op != JOIN) {
                        fprintf(stderr, "Closing because state = 0 and op is not JOIN\n");
                        close_connection(i, op->active_clients_mutex);
                        destroy_PDU(p);
                        break;
                    } else if (p->op == MESS) {
                        MESS_msg *mess = p->msg;
                        if (mess->msg_len != 0) {
                            add_to_send(create_MESS(p, i));
                        }
                        //fprintf(stderr, "Recieved MESS\n");
                    } else if (p->op == QUIT) {
                        fprintf(stderr, "Recieved QUIT from client %d\n", i);
                        recieved_QUIT(i, op->active_clients_mutex);
                        break;
                    } else if (p->op == JOIN) {
                        recieved_JOIN((JOIN_msg *) p->msg, i,
                                      op->active_clients_mutex);
                    } else {
                        fprintf(stderr, "Closing just ???\n");
                        close_connection(i, op->active_clients_mutex);
                        break;
                    }
                    destroy_PDU(p);
                    if (status != -2)
                        break;

                    int count;
                    ioctl(clients[i].sock, FIONREAD, &count);
                    if (count > 66000) {
                        close_connection(i, op->active_clients_mutex);
                        pdu *p1 = create_QUIT();
                        add_to_send(p1);
                        break;
                    }
                    //fprintf(stderr, "There is more in the buffer m8\n");
                }
                if (!should_terminate()) {
                    break;
                }
            }
        }
    }

    free(op);
    return NULL;
}

/**
 * Starts the client master thread and handles new connection and closing
 * connection. When anything has been recieved the master_thread handles it
 * and forwards it or whatever it should do. More info in the helper functions.
 *******************************************************************************
 * @param arg the active clients mutex
 * @return NULL
 */
void *start_master_thread(void *arg) {
    pthread_mutex_t *active_clients_mutex = arg;
    for (int i = 0; i < MAXCLIENTS; ++i) {
        clients[i].sock = 0;
        clients[i].client_name = NULL;
        clients[i].state = 0;
    }

    int addrlen;
    struct sockaddr_in address;

    int master_socket = init_connection(&address, &addrlen);

    const int MAXTHREADS = MAXCLIENTS / 10 + 1;
    pthread_t client_threads[MAXTHREADS];

    for (int i = 0; i < MAXTHREADS; ++i) {
        thread_op *op = malloc(sizeof(thread_op));
        op->lower_range = i * 10;
        op->higher_range = (i == MAXTHREADS - 1) ? MAXCLIENTS : (i + 1) * 10;
        op->active_clients_mutex = active_clients_mutex;
        pthread_create(&client_threads[i], NULL, client_connection, op);
        fprintf(stderr, "lower %d higher %d\n", op->lower_range, op->higher_range);
    }

    fd_set readfds;
    while (!should_terminate()) {
        FD_ZERO(&readfds);
        FD_SET(master_socket, &readfds);

        struct timeval tv = {0, 800000};
        int activity = select(master_socket + 1, &readfds, NULL, NULL, &tv);
        if (activity == 0) continue;
        if (activity < 0) terror("select()");

        pthread_mutex_lock(active_clients_mutex);
        if (FD_ISSET(master_socket, &readfds) && active_clients != 255) {
            pthread_mutex_unlock(active_clients_mutex);
            new_connection(master_socket, addrlen, &address,
                           active_clients_mutex);
        } else {
            pthread_mutex_unlock(active_clients_mutex);
        }

    }

    pdu *p = create_QUIT();
    for (int i = 0; i < MAXCLIENTS; ++i) {
        if (clients[i].sock > 0) {
            serialize_PDU(clients[i].sock, p);
            fprintf(stderr, "Closing as part of shutdown\n");
            close_connection(i, active_clients_mutex);
        }
    }
    destroy_PDU(p);

    for (int i = 0; i < MAXTHREADS; ++i) {
        pthread_join(client_threads[i], NULL);
    }

    return NULL;
}

/**
 * Send PJOIN to everyone except the client who joined.
 *******************************************************************************
 * @param p the pdu containing PJOIN which should be sent
 */
void send_PJOIN(pdu *p) {
    PJOIN_msg *pjoin = p->msg;
    for (int i = 0; i < MAXCLIENTS; ++i) {
        pthread_mutex_lock(&clients[i].mutex);
        if (clients[i].sock > 0 && clients[i].sock != pjoin->socket) {
            pthread_mutex_unlock(&clients[i].mutex);
            serialize_PDU(clients[i].sock, p);
        } else {
            pthread_mutex_unlock(&clients[i].mutex);
        }

    }
}

/**
 * Send PDU to all clients.
 *******************************************************************************
 * @param p the pdu which should be sent
 */
void send_PDU_to_all(pdu *p) {
    for (int i = 0; i < MAXCLIENTS; ++i) {
        pthread_mutex_lock(&clients[i].mutex);
        if (clients[i].sock > 0) {
            int sock = clients[i].sock;
            pthread_mutex_unlock(&clients[i].mutex);
            serialize_PDU(sock, p);

        } else {
            pthread_mutex_unlock(&clients[i].mutex);
        }
    }
}

/**
 * Checks which op code the pdu has and sends it to the clients which should
 * recieve the pdu. It prints to STDERR which op it sent.
 *******************************************************************************
 * @param p the pdu which should be sent
 */
void handle_sending(pdu *p) {
    switch (p->op) {
        case MESS:
            send_PDU_to_all(p);
            break;
        case QUIT:
            send_PDU_to_all(p);
            break;
        case PJOIN:
            send_PJOIN(p);
            fprintf(stderr, "Sent PJOIN\n");
            break;
        case PLEAVE:
            send_PDU_to_all(p);
            break;
        default:
            fprintf(stderr,
                    "Trying to send an invalid message, this shouldn't be able to happen\n");
            break;
    }
    //fprintf(stderr, "Sending pdu with op: %d\n", p->op);
}

/**
 * Locks the sending queue and returns boolean if the sending_queue is empty.
 *******************************************************************************
 * @return boolean if the sending_queue is empty
 */
int sending_is_empty(void) {
    pthread_mutex_lock(&send_mutex);
    return queue_is_empty(sending_queue);
}

/**
 * Sends a pdu when one has been recieved in the sending queue. Only
 * terminates when sending queue is empty and the thread should terminate.
 *******************************************************************************
 * @param arg NONE
 * @return NULL
 */
void *send_PDU(void *arg) {
    (void) arg;
	
    while (!should_terminate()) {

		pthread_mutex_lock(&send_mutex);
        if (queue_is_empty(sending_queue))
            pthread_cond_wait(&send_cond, &send_mutex);

        if (should_terminate()) {
            pthread_mutex_unlock(&send_mutex);
            break;
        }

        pdu *p = queue_pop(sending_queue);
        pthread_mutex_unlock(&send_mutex);

        handle_sending(p);
        destroy_PDU(p);
    }

	pthread_mutex_lock(&send_mutex);
	while (!queue_is_empty(sending_queue)) {
		destroy_PDU(queue_pop(sending_queue));
	}
	pthread_mutex_unlock(&send_mutex);

    return NULL;
}

/**
 * Sets the port, servername, nameserverhost and nameserverport. If invalid
 * arguments it prints an error message and quits.
 *******************************************************************************
 * @param argc number of arguments
 * @param argv the given arguments
 */
void set_info(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <port> <servername> <nameserverhost> "
                        "<nameserverport>\n", argv[0]);
        exit(1);
    }

    strcpy(port, argv[1]);
    strcpy(servername, argv[2]);
    strcpy(nameserverhost, argv[3]);
    strcpy(nameserverport, argv[4]);
}

/**
 * Runs the program by creating the sending_queue and initiating mutexes and
 * condition variables and then creates 3 threads which run the program. At
 * program exit it joins the threads and destroys the mutexes and the condition
 * variable and the frees the queue.
 */
void run_program(void) {
    sending_queue = queue_create();

    pthread_mutex_t active_clients_mutex;

    pthread_mutex_init(&active_clients_mutex, NULL);
    pthread_mutex_init(&send_mutex, NULL);
    pthread_mutex_init(&terminate_mutex, NULL);
    pthread_cond_init(&send_cond, NULL);
    for (int i = 0; i < MAXCLIENTS; ++i) {
        pthread_mutex_init(&clients[i].mutex, NULL);
    }

    pthread_t nameserver_thread, sending_thread[10], master_thread;

    pthread_create(&nameserver_thread, NULL, run_nameserver,
                   &active_clients_mutex);
    pthread_create(&master_thread, NULL, start_master_thread,
                   &active_clients_mutex);
	for (int i = 0; i < 1; i++) {
		pthread_create(&sending_thread[i], NULL, send_PDU, NULL);

	}

    pthread_join(nameserver_thread, NULL);
    pthread_join(master_thread, NULL);
	for (int i = 0; i < 1; i++)  {
		pthread_join(sending_thread[i], NULL);
	}

    pthread_mutex_destroy(&active_clients_mutex);
    pthread_mutex_destroy(&send_mutex);
    pthread_mutex_destroy(&terminate_mutex);
    pthread_cond_destroy(&send_cond);
    for (int i = 0; i < MAXCLIENTS; ++i) {
        pthread_mutex_destroy(&clients[i].mutex);
    }

    queue_free(sending_queue);
}

/**
 * Main function which runs the complete program
 * @param argc nr of arguments
 * @param argv the given arguments
 * @return 0 on correct finish
 */
int main(int argc, char *argv[]) {

    set_info(argc, argv);

    if (signal(SIGINT, handle_signal) == SIG_ERR) terror("signal()");
    if (signal(SIGPIPE, handle_signal) == SIG_ERR) terror("signal()");

    run_program();

    return 0;
}
