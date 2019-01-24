#include "modules.h"


/**
 * Helper-function that passes the responsibility depending on the OP-code of
 * the PDU.
 *******************************************************************************
 * @param p - PDU
 */
void determine_action(pdu *p) {
    switch (p->op) {
        case MESS:
            print_MESS(p);
            break;
        case QUIT:
            print_QUIT();
            break;
        case PJOIN:
            print_PJOIN(p);
            break;
        case PLEAVE:
            print_PLEAVE(p);
            break;
        case PARTICIPANTS:
            print_PARTICIPANTS(p);
            break;
        default:
            print_invalid_PDU();
            break;
    }
}


/**
 * Helper-function that sends a GETLIST to currently active socket.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 */
void get_server_list(struct thread_options *options) {
    GETLIST_msg *getlist = malloc(sizeof(GETLIST_msg));
    if (getlist == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
    }
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);

    }
    p->op = GETLIST;
    p->msg = getlist;

    serialize_PDU(options->socket, p);
    destroy_PDU(p);
}


/**
 * Helper-function that creates a MESS PDU of the given string.
 *******************************************************************************
 * @param buf - message string.
 * @return - Constructed PDU.
 */
pdu *create_MESS(char *buf) {
    MESS_msg *mess = calloc(1, sizeof(MESS_msg));
    if (mess == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    mess->timestamp = 0;
    mess->id_len = 0;
    mess->msg_len = (uint8_t) strlen(buf);
    mess->msg = calloc((unsigned int) mess->msg_len + 1, 1);
    if (mess->msg == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    mess->client_id = NULL;
    memcpy(mess->msg, buf, mess->msg_len);

    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    p->op = MESS;
    p->msg = mess;

    return p;
}


/**
 * Helper-function that creates a QUIT PDU.
 *******************************************************************************
 * @return - Constructed PDU.
 */
pdu *create_QUIT(void) {
    QUIT_msg *quit = malloc(sizeof(QUIT_msg));
    if (quit == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    p->op = QUIT;
    p->msg = quit;
    return p;
}


/**
 * Helper-function that creates a JOIN PDU.
 *******************************************************************************
 * @param options - struct that holds necessary options.
 * @return - Constructed PDU.
 */
pdu *create_JOIN(struct thread_options *options) {
    JOIN_msg *join = malloc(sizeof(JOIN_msg));
    if (join == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    join->len = (uint8_t) strlen(options->client_id);
    join->id = malloc((unsigned int) join->len + 1);
    if (join->id == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    strcpy(join->id, options->client_id);

    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) {
        fprintf(stderr,"Memory allocation failed, error: %d\n",errno);
        catch_signal(SIGINT);
        return NULL;
    }
    p->op = JOIN;
    p->msg = join;
    return p;
}


/**
 * Helper-function that prints a timestamp in [HH:MM:SS].
 *******************************************************************************
 * @param timestamp - UNIX timestamp.
 */
void print_time(time_t timestamp) {
    struct tm *time_info;
    char timeString[9];

    time(&timestamp);
    time_info = localtime(&timestamp);

    strftime(timeString, sizeof(timeString), "%H:%M:%S", time_info);
    fprintf(stderr, "[%s] ", timeString);
}


/**
 * Helper-function that prints a PJOIN PDU.
 *******************************************************************************
 * @param p - PJOIN PDU.
 */
void print_PJOIN(pdu *p) {
    PJOIN_msg *pjoin_msg = p->msg;
    print_time((time_t) pjoin_msg->timestamp);
    printf("SERVER NOTICE: %s has joined the chat.\n", pjoin_msg->id);

}


/**
 * Helper-function that prints a MESS PDU.
 *******************************************************************************
 * @param p - MESS PDU.
 */
void print_MESS(pdu *p) {
    MESS_msg *mess_msg = p->msg;
    print_time((time_t) mess_msg->timestamp);
    printf("[%s]: %s\n", mess_msg->client_id, mess_msg->msg);
}


/**
 * Helper-function that prints a message if a QUIT PDU is recieved.
 *******************************************************************************
 */
void print_QUIT(void) {
    fprintf(stderr, "[SYSTEM] Connection to server terminated (QUIT).\n");
    catch_signal(SIGINT);
}


/**
 * Helper-function that prints a PLEAVE PDU.
 *******************************************************************************
 * @param p - PLEAVE PDU.
 */
void print_PLEAVE(pdu *p) {
    PLEAVE_msg *pleave_msg = p->msg;
    print_time((time_t) pleave_msg->timestamp);
    printf("SERVER NOTICE: %s has left the chat.\n", pleave_msg->id);
}


/**
 * Helper-function that prints a PARTICIPANTS PDU.
 *******************************************************************************
 * @param p - PARTICIPANTS PDU.
 */
void print_PARTICIPANTS(pdu *p) {
    PART_msg *part_msg = p->msg;
    printf("Participants in the chat: ");
    for (int i = 0; i < part_msg->nr_of_ids; ++i) {
        if (i == part_msg->nr_of_ids - 1) {
            printf("%s\n", part_msg->ids[i]);
        } else {
            printf("%s, ", part_msg->ids[i]);
        }
    }
}


/**
 * Helper-function that prints if a corrupted or unknown PDU is recieved.
 *******************************************************************************
 */
void print_invalid_PDU(void) {
    fprintf(stderr,"PDU could not be recognized, disconnecting.\n");
    catch_signal(SIGINT);
}


/**
 * Helper-function that catches a SIGINT-signal and orders all threads to
 * terminate.
 *******************************************************************************
 * @param signo - signal.
 */
void catch_signal(int signo) {

    if (signo == SIGINT) {

        pthread_mutex_lock(&terminate_mutex);
        terminate = 1;
        pthread_mutex_unlock(&terminate_mutex);

        pthread_mutex_lock(&incoming_lock);
        pthread_cond_broadcast(&p_cond_incoming);
        pthread_mutex_unlock(&incoming_lock);

        pthread_mutex_lock(&connected_lock);
        pthread_cond_broadcast(&p_cond_connected);
        pthread_mutex_unlock(&connected_lock);
    }
}

