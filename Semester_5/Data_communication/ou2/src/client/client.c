/*
 * A client that connects to either a nameserver or directly to a chatserver.
 * If user connects to nameserver, a list of registered server will be listed
 * and the the user can connect via a number input.
 * This is part of Assignment 2 of the course Computer communication and
 * computer networks given at Umeå University.
 *
 * Usage: client <ID> <ns/cs> <serverhost> <serverport>
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#include "client.h"


int main(int argc, const char *argv[]) {
    terminate = 0;
    active_socket = 0;

    if (signal(SIGINT, catch_signal) == SIG_ERR) terror("signal()");
    struct thread_options *options = malloc(sizeof(*options));
    if (options == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        return -1;
    }

    initialize(options);
    if (argc == 5 && load_args(options,argv)) {
        fprintf(stderr,"[SYSTEM] Arguments loaded correctly.\n");
    } else {
        fprintf(stderr,"Usage: client <ID> <ns/cs> <host> <port>\n");
        cleanup(options);
        return 0;
    }

    pthread_t threads[4];

    while (terminate < 2) {

        if (options->nameserver) {
            pthread_create(&threads[0], NULL, &init_nameserver, (void *) options);
            pthread_join(threads[0], NULL);
        }


        if (!terminate) {
            pthread_create(&threads[1], NULL, &connect_and_listen, (void *) options);
            pthread_create(&threads[2], NULL, &handle_pdu, (void *) options);
            pthread_create(&threads[3], NULL, &writing_prompt, (void *) options);


            pthread_join(threads[1], NULL);
            pthread_join(threads[2], NULL);
            pthread_join(threads[3], NULL);
        }

        if (active_socket) {
            close(options->socket);
            active_socket = 0;
        }
        if (options->nameserver == 0) {
            terminate = 2;
        }

        int valid_answer = 0;
        while (!valid_answer && options->nameserver == 1) {
            fprintf(stderr,"Do you want to reconnect to the nameserver? [Y/N]\n");
            char *buf = calloc(1, 65536);
            if (buf == NULL) {
                fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
                catch_signal(SIGINT);
                break;
            }
            fgets(buf, 65536, stdin);

            buf[strlen(buf) - 1] = '\0';
            if (strcmp(buf, "Y") == 0 || strcmp(buf, "y") == 0) {
                valid_answer = 1;
                free(options->server);
                free(options->server_port);
                char *server_adress = malloc(strlen(argv[3]) + 1);
                if (server_adress == NULL) {
                    fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
                    catch_signal(SIGINT);
                    break;
                }
                char *server_port = malloc(strlen(argv[4]) + 1);
                if (server_port == NULL) {
                    fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
                    catch_signal(SIGINT);
                    break;
                }
                strcpy(server_adress,argv[3]);
                strcpy(server_port,argv[4]);
                options->server = server_adress;
                options->server_port = server_port;
                terminate = 0;

            } else if (strcmp(buf, "N") == 0 || strcmp(buf, "n") == 0) {
                valid_answer = 1;
                terminate = 2;
            } else {
                fprintf(stderr, "Invalid answer, try again.\n");
            }
            free(buf);
        }
    }
    cleanup(options);
    return 0;
}

/**
 * Function to parse the arguments given at startup and stores them in the
 * "options"-struct.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 * @param argv - input arguments.
 * @return 1 if everything went well, 0 otherwise.
 */
int load_args(struct thread_options *options, const char *argv[]) {

    options->client_id = malloc(strlen(argv[1]) + 1);
    if (options->client_id == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return 0;
    }
    options->server = malloc(strlen(argv[3]) + 1);
    if (options->server == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return 0;
    }
    options->server_port = malloc(strlen(argv[4]) + 1);
    if (options->server_port == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return 0;
    }
    strcpy(options->client_id,argv[1]);
    if (strcmp(argv[2],"cs") == 0) {
        options->nameserver = 0;
    } else if (strcmp(argv[2],"ns") == 0) {
        options->nameserver = 1;
    } else {
        return 0;
    }
    strcpy(options->server,argv[3]);
    strcpy(options->server_port,argv[4]);
    return 1;
}


/**
 * Function to initialize queues and locks.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 */
void initialize(struct thread_options *options) {
    // Setting up queues
    options->incoming_q = queue_create();

    // Initializing locks and conds.
    pthread_mutex_init(&terminate_mutex, NULL);
    pthread_mutex_init(&incoming_lock, NULL);
    pthread_cond_init(&p_cond_incoming, NULL);
    pthread_mutex_init(&connected_lock, NULL);
    pthread_cond_init(&p_cond_connected, NULL);
}


/**
 * Function to initiate the nameserver and everything that comes with it. Sends
 * a GETLIST and listen for an SLIST.
 *******************************************************************************
 * @param arg - void pointer to the options-struct.
 */
void *init_nameserver(void *arg) {
    struct thread_options *options = arg;

    uint8_t msg[BUFSIZE];
    memset(msg, 0, BUFSIZE);
    connect_to_server(options);
    int ret;
    pdu *p = NULL;

    fd_set readfds;
    int acceptable_pdu = 0;
    while (acceptable_pdu == 0 && !should_terminate()) {
        get_server_list(options);
        int status, index = 0;
        FD_ZERO(&readfds);
        FD_SET(options->socket, &readfds);
        struct timeval tv = {0, 800000};


        int activity = select(options->socket + 1, &readfds, NULL, NULL, &tv);
        if (activity == 0) continue;
        if (activity < 0) {
            terror("select()");
        }

        if ((ret = (int)read(options->socket, msg, BUFSIZE)) < 0) terror("read()");

        // Hinner inte få hela meddelandet.
        usleep(200000);


        if (ret == 0) break;
        p = retrieve_PDU(options->socket, msg, &status, &index);
        if (status == -2) {
            fprintf(stderr, "[SYSTEM] There is more to read from the nameserver "
                            "and this should not happen\n");
        }
        if (p == NULL) {
            fprintf(stderr, "[SYSTEM] Incorrect PDU\n");
            close(options->socket);
            active_socket = 0;
            catch_signal(SIGINT);
            break;
        }
        if (p->op == 4) {
            fprintf(stderr, "[SYSTEM] Recieved SLIST.\n");
            acceptable_pdu = 1;
            close(options->socket);
            active_socket = 0;
        } else {
            fprintf(stderr, "[SYSTEM] Did not recieve SLIST, sending GETLIST again.\n");
            fprintf(stderr, "%d\n", msg[0]);
            sleep(1);
        }

    }
    if (acceptable_pdu) {
        SLIST_msg *slist_msg = p->msg;

        choose_server(options, slist_msg);
    } else {
        catch_signal(SIGINT);
    }


    if (p != NULL) {
        destroy_PDU(p);
    }

    return NULL;
}


/**
 * Function that attempts to connect to the server that is stored in "options".
 * Tries to reconnect five times if a connection cannot be established.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 */
void connect_to_server(struct thread_options *options) {

    // Setting up tcp.

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = 0;
    hints.ai_flags = AI_ADDRCONFIG;
    if ((options->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) terror("socket()");
    errno = 0;
    if (getaddrinfo(options->server, options->server_port, &hints,
                    &res) < 0) {
        fprintf(stderr,"[SYSTEM] getaddrinfo error: %d\n",errno);
    }
    fprintf(stderr, "[SYSTEM] Attempting to connect to: "
                    "%s:%s\n", options->server, options->server_port);

    int connectAttempt = 0;
    while (!should_terminate()) {

        int status = connect(options->socket, res->ai_addr, res->ai_addrlen);
        if (status == 0) {
            printf("[SYSTEM] Connection successful!\n");
            active_socket = 1;
            break;
        }
        printf("[SYSTEM] Connection failed, retrying...\n");
        if (connectAttempt > 5) {
            fprintf(stderr,"[SYSTEM] Cannot connect to server.\n");
            catch_signal(SIGINT);
            break;
        }
        sleep(1);
        connectAttempt++;
    }

    freeaddrinfo(res);
}


/**
 * Function that reads an SLIST-PDU to get information about every server
 * registered to the nameserver. Prompts the user to choose a server from the
 * list.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 * @param slist_msg - SLIST PDU containing all active server registered..
 */
void choose_server(struct thread_options *options, SLIST_msg *slist_msg) {


    SLIST_ITEM **slist_items = slist_msg->server_info;


    fprintf(stderr, "SLIST recieved, number of servers: %d\n",
            slist_msg->nr_of_servers);

    for (int i = 0; i < slist_msg->nr_of_servers; ++i) {
        fprintf(stderr, "[%d]: '%s' has %d users.\n",
                i, slist_items[i]->server_name, slist_items[i]->nr_of_clients);
    }
    int acceptable_answer = 0;
    char answer[100];
    while (acceptable_answer == 0) {
        fprintf(stderr, "Choose server by entering its number: \n");
        fgets(answer, 100, stdin);
        int server_choice;
        char *endptr;
        errno = 0;
        long result = strtol(answer, &endptr, 10);
        if (endptr == answer) {
            server_choice = -1;
        } else if (errno == EINVAL && errno == ERANGE) {
            server_choice = -1;
        } else {
            server_choice = (int)result;
        }

        if (server_choice >= 0 && server_choice < slist_msg->nr_of_servers) {
            fprintf(stderr, "Excellent choice, going for: %d\n", server_choice);
            free(options->server);
            free(options->server_port);
            char *server_adress = malloc(sizeof(char) * 100);
            if (server_adress == NULL) {
                fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
                catch_signal(SIGINT);
                break;
            }

            char *server_port = malloc(sizeof(char) * 10);
            if (server_port == NULL) {
                fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
                catch_signal(SIGINT);
                break;
            }
            sprintf(server_adress, "%d.%d.%d.%d",
                    slist_items[server_choice]->addr[0],
                    slist_items[server_choice]->addr[1],
                    slist_items[server_choice]->addr[2],
                    slist_items[server_choice]->addr[3]);
            sprintf(server_port, "%d", slist_items[server_choice]->port);

            options->server = server_adress;
            options->server_port = server_port;
            acceptable_answer = 1;


        } else {
            fprintf(stderr, "Sorry, that number does not exist. Try again.\n");
        }
    }
}


/**
 * Threaded function that connects to a server and then listens to the port.
 *******************************************************************************
 * @param arg - void pointer to the options-struct.
 */
void *connect_and_listen(void *arg) {
    struct thread_options *options = arg;
    int status, index = 0;

    connect_to_server(options);
    usleep(500000);
    pdu *p = create_JOIN(options);
    uint8_t msg[MAXMSGSIZE];
    memset(msg, 0, MAXMSGSIZE);
    serialize_PDU(options->socket, p);
    destroy_PDU(p);

    fd_set readfds;


    while (!should_terminate()) {


        FD_ZERO(&readfds);
        FD_SET(options->socket, &readfds);
        struct timeval tv = {0, 800000};

        int activity = select(options->socket + 1, &readfds, NULL, NULL, &tv);
        if (activity == 0) {
            continue;
        }
        if (activity < 0) terror("select()");

        index = 0;
        status = 0;
        memset(msg, 0, MAXMSGSIZE);
        int ret;
        if ((ret = (int)read(options->socket, msg, MAXMSGSIZE)) < 0) terror("read()");
        if (ret == 0) {
            fprintf(stderr, "[SYSTEM] Connection to server lost.\n");
            catch_signal(SIGINT);
            break;
        }
        if (should_terminate()) {
            break;
        }

        while (!should_terminate()) {
            p = retrieve_PDU(options->socket, msg, &status, &index);

            if (p == NULL) {
                fprintf(stderr, "[SYSTEM] Incorrect PDU\n");
                catch_signal(SIGINT);
                break;
            }
            pthread_mutex_lock(&incoming_lock);
            queue_push(options->incoming_q, p);
            pthread_cond_signal(&p_cond_incoming);
            pthread_mutex_unlock(&incoming_lock);

            if (status != -2)
                break;
        }



    }
    return NULL;
}


/**
 * Threaded function that reads input from the user and sends it to the server
 * via PDUs.
 *******************************************************************************
 * @param arg - void pointer to the options-struct.
 */
void *writing_prompt(void *arg) {
    struct thread_options *options = arg;

    fd_set readfds;
    pthread_mutex_lock(&connected_lock);

    pthread_cond_wait(&p_cond_connected,&connected_lock);
    pthread_mutex_unlock(&connected_lock);

    while (!should_terminate()) {

        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timeval tv = {0, 800000};

        int activity = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &tv);
        if (activity == 0) continue;
        if (activity < 0) terror("select()");

        if (should_terminate()) {
            break;
        }

        char *buf = calloc(1, 65536);
        if (buf == NULL) {
            fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
            catch_signal(SIGINT);
            break;
        }
        fgets(buf, 65536, stdin);

        buf[strlen(buf) - 1] = '\0';
        if (strcmp(buf, "QUIT") == 0) {
            pdu *p = create_QUIT();
            serialize_PDU(options->socket, p);
            destroy_PDU(p);
        } else {
            pdu *p = create_MESS(buf);
            serialize_PDU(options->socket, p);
            destroy_PDU(p);
        }
        free(buf);
    }
    return NULL;
}


/**
 * Threaded function that waits for a recieved PDU to enter the queue. When
 * that happens, it acts accordingly.
 *******************************************************************************
 * @param arg - void pointer to the options-struct.
 */
void *handle_pdu(void *arg) {


    struct thread_options *options = arg;

    while (!should_terminate()) {

        pthread_mutex_lock(&incoming_lock);
        if (queue_is_empty(options->incoming_q)) {
            pthread_cond_wait(&p_cond_incoming, &incoming_lock);

        }
        if (should_terminate()) {
            pthread_mutex_unlock(&incoming_lock);
            break;
        }
        pdu *p = (queue_pop(options->incoming_q));
        pthread_mutex_unlock(&incoming_lock);

        determine_action(p);
        if (p->op == PARTICIPANTS) {
            pthread_mutex_lock(&connected_lock);

            pthread_cond_broadcast(&p_cond_connected);
            pthread_mutex_unlock(&connected_lock);
        }
        destroy_PDU(p);

    }
    while (!queue_is_empty(options->incoming_q)) {
        pthread_mutex_lock(&incoming_lock);
        destroy_PDU(queue_pop(options->incoming_q));
        pthread_mutex_unlock(&incoming_lock);
    }

    return NULL;
}


/**
 * Function to clean up after the program. Free's up allocated memory, empties
 * the queue and destroys the locks.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 */
void cleanup(struct thread_options *options) {

    while (!queue_is_empty(options->incoming_q)) {
        free(queue_pop(options->incoming_q));
    }

    queue_free(options->incoming_q);
    free(options->server);
    free(options->server_port);
    free(options->client_id);


    pthread_mutex_destroy(&terminate_mutex);
    pthread_mutex_destroy(&incoming_lock);
    pthread_cond_destroy(&p_cond_incoming);
    pthread_mutex_destroy(&connected_lock);
    pthread_cond_destroy(&p_cond_connected);


    free(options);

    fprintf(stderr, "[SYSTEM] Cleanup complete.\n");
}


/**
 * Function that determines if a terminate-call has been made.
 *******************************************************************************
 * @return terminate, true/false.
 */
int should_terminate(void) {
    pthread_mutex_lock(&terminate_mutex);
    int ret = terminate;
    pthread_mutex_unlock(&terminate_mutex);
    return ret;
}
