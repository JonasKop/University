/*
 * File containing deserializing functions which takes a socket and a buffer
 * and reads more data to the buffer if necessary it then returns the PDU if
 * everything went ok, else it returns NULL
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "protocols.h"
#include "deserialize.h"


/**
 * Checks if there is padding in the buffer starting from an index
 *******************************************************************************
 * @param sock the socket which should be read from if need more data
 * @param buffer the buffer containing a pdu
 * @param ind current index in the buffer
 * @param nr_of_bytes nr of bytes to be read
 * @return 0 on completion, else -1
 */
int get_padd(int sock, uint8_t *buffer, int *ind, int nr_of_bytes) {

    for (int i = 0; i < nr_of_bytes; ++i) {
        if (*ind >= BUFSIZE && read_data(sock, buffer, ind) == -1)
            return -1;

        if (buffer[*ind] != 0)
            return -1;

        (*ind)++;
    }
    return 0;
}

/**
 * Gets 1 byte int from the buffer
 *******************************************************************************
 * @param sock the socket which should be read from if need more data
 * @param buffer the buffer containing a pdu
 * @param ind current index in the buffer
 * @param dest destination int pointer
 * @return 0 on completion, else -1
 */
int get8(int sock, uint8_t buffer[], int *ind, uint8_t *dest) {
    if (*ind >= BUFSIZE && read_data(sock, buffer, ind) == -1)
        return -1;
    *dest = buffer[*ind];
    (*ind)++;
    return 0;
}

/**
 * Gets 2 byte int from the buffer
 *******************************************************************************
 * @param sock the socket which should be read from if need more data
 * @param buffer the buffer containing a pdu
 * @param ind current index in the buffer
 * @param dest destination int pointer
 * @return 0 on completion, else -1
 */
int get16(int sock, uint8_t buffer[], int *ind, uint16_t *dest) {
    uint16_t arr[2];
    for (int i = 0; i < 2; ++i) {
        if (*ind >= BUFSIZE && read_data(sock, buffer, ind) == -1)
            return -1;

        arr[i] = buffer[*ind];
        (*ind)++;
    }
    *dest = (uint16_t) (arr[1] | arr[0] << 8);
    return 0;
}

/**
 * Gets 4 byte int from the buffer
 *******************************************************************************
 * @param sock the socket which should be read from if need more data
 * @param buffer the buffer containing a pdu
 * @param ind current index in the buffer
 * @param dest destination int pointer
 * @return 0 on completion, else -1
 */
int get32(int sock, uint8_t buffer[], int *ind, uint32_t *dest) {
    uint8_t arr[4];
    for (int i = 0; i < 4; ++i) {
        if (*ind >= BUFSIZE && read_data(sock, buffer, ind) == -1)
            return -1;
        arr[i] = buffer[*ind];
        (*ind)++;
    }
    *dest = (uint32_t) (arr[3] | arr[2] << 8 | arr[1] << 16 | arr[0] << 24);
    return 0;
}

/**
 * Gets a string from the buffer and set it to the str pointer
 *******************************************************************************
 * @param sock the socket which should be read from if need more data
 * @param buffer the buffer containing a pdu
 * @param ind current index in the buffer
 * @param size length of string
 * @param str the pointer where the string should be stored
 * @return 0 on completion, else -1
 */
int get_str(int sock, uint8_t buffer[], int *ind, int size, char *str) {

    for (int i = 0; i < size; ++i) {
        if (*ind >= BUFSIZE && read_data(sock, buffer, ind) == -1)
            return -1;
        str[i] = (char) buffer[*ind];
        (*ind)++;
    }
    return 0;
}

/**
 * Destroys a pdu and returns NULL
 *******************************************************************************
 * @param p the pdu which should be destroyed
 * @return NULL
 */
void *invalid_PDU(pdu *p) {
    if (p != NULL)
        destroy_PDU(p);

    return NULL;
}

/**
 * Prints the given string and returns NULL,
 *******************************************************************************
 * @param p The pdu which shuold be destroyed
 * @param str error string
 * @return NULL
 */
void *sys_error(pdu *p, const char *str) {
    if (p != NULL)
        destroy_PDU(p);
    perror(str);
    return NULL;
}

pdu *deserialize_REG(int sock, uint8_t buffer[], int *ind) {
    fprintf(stderr, "%d %d %d %d %d %d\n", buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    REG_msg *reg = malloc(sizeof(REG_msg));
    if (reg == NULL) sys_error(p, "malloc()");
    p->msg = reg;
    p->op = REG;
    reg->server_name = NULL;


    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &reg->len) == -1 ||
        get16(sock, buffer, ind, &reg->port) == -1)
        return invalid_PDU(p);

    if (reg->port == 0) {
        return invalid_PDU(p);
    }

    reg->server_name = calloc((unsigned int) (reg->len + 1), 1);
    if (reg->server_name == NULL) sys_error(p, "calloc()");

    if (get_str(sock, buffer, ind, reg->len, reg->server_name) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(reg->len)) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_ACK(int sock, uint8_t *buffer, int *ind) {

    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    ACK_msg *ack = malloc(sizeof(ACK_msg));
    if (ack == NULL) sys_error(p, "malloc()");
    p->msg = ack;
    p->op = ACK;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, 1) == -1 ||
        get16(sock, buffer, ind, &ack->id) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_ALIVE(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    ALIVE_msg *alive = malloc(sizeof(ALIVE_msg));
    if (alive == NULL) sys_error(p, "malloc()");
    p->msg = alive;
    p->op = ALIVE;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &alive->nr_of_clients) == -1 ||
        get16(sock, buffer, ind, &alive->id) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_NOTREG(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    NOTREG_msg *notreg = malloc(sizeof(NOTREG_msg));
    if (notreg == NULL) sys_error(p, "malloc()");
    p->msg = notreg;
    p->op = NOTREG;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, 1) == -1 ||
        get16(sock, buffer, ind, &notreg->id) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_GETLIST(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    GETLIST_msg *getlist = malloc(sizeof(GETLIST_msg));
    if (getlist == NULL) sys_error(p, "malloc()");
    p->msg = getlist;
    p->op = GETLIST;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(3)) == -1)
        return invalid_PDU(p);

    return p;
}

int get_SLIST_item(int sock, SLIST_ITEM *slist_item, uint8_t *buffer, int *ind) {

    for (int i = 0; i < 4; i++) {
        if (get8(sock, buffer, ind, &slist_item->addr[i]) == -1)
            return -1;
    }
    if (get16(sock, buffer, ind, &slist_item->port) == -1 ||
        get8(sock, buffer, ind, &slist_item->nr_of_clients) == -1 ||
        get8(sock, buffer, ind, &slist_item->len) == -1)
        return -1;

    slist_item->server_name = calloc((unsigned int) (slist_item->len + 1), 1);
    if (slist_item->server_name == NULL) {
        perror("malloc()");
        return -1;
    }

    if (get_str(sock, buffer, ind, slist_item->len, slist_item->server_name) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(slist_item->len)) == -1) {
        free(slist_item->server_name);
        free(slist_item);
        return -1;
    }
    return 0;
}


pdu *deserialize_SLIST(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    SLIST_msg *slist = malloc(sizeof(SLIST_msg));
    if (slist == NULL) sys_error(p, "malloc()");
    p->msg = slist;
    p->op = SLIST;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, 1) == -1 ||
        get16(sock, buffer, ind, &slist->nr_of_servers) == -1)
        return invalid_PDU(p);

    fprintf(stderr, "Index is %d\n", *ind);

    slist->server_info = calloc(sizeof(SLIST_ITEM *) * slist->nr_of_servers + 1, 1);
    for (int i = 0; i < slist->nr_of_servers; ++i) {
        slist->server_info[i] = NULL;
    }
    if (slist->server_info == NULL)
        sys_error(p, "calloc()");


    for (int i = 0; i < slist->nr_of_servers; i++) {

        slist->server_info[i] = calloc(sizeof(SLIST_ITEM), 1);
        if (slist->server_info[i] == NULL)
            sys_error(p, "calloc()");

        if (get_SLIST_item(sock, slist->server_info[i], buffer, ind) == -1) {
            slist->nr_of_servers = (uint16_t) i;
            return invalid_PDU(p);
        }
    }


    return p;
}

pdu *deserialize_MESS(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    MESS_msg *mess = malloc(sizeof(MESS_msg));
    if (mess == NULL) sys_error(p, "malloc()");
    p->msg = mess;
    mess->client_id = NULL;
    mess->msg = NULL;
    p->op = MESS;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, 1) == -1 ||
        get8(sock, buffer, ind, &mess->id_len) == -1 ||
        get8(sock, buffer, ind, &mess->checksum) == -1 ||
        get16(sock, buffer, ind, &mess->msg_len) == -1 ||
        get_padd(sock, buffer, ind, 2) == -1 ||
        get32(sock, buffer, ind, &mess->timestamp) == -1)
        return invalid_PDU(p);

    mess->msg = calloc((unsigned int) (mess->msg_len + 1), 1);
    if (mess->msg == NULL)
        return sys_error(p, "calloc()");
    mess->client_id = calloc((unsigned int) (mess->id_len + 1), 1);
    if (mess->client_id == NULL)
        return sys_error(p, "calloc()");

    if (get_str(sock, buffer, ind, mess->msg_len, mess->msg) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(mess->msg_len)) == -1 ||
        get_str(sock, buffer, ind, mess->id_len, mess->client_id) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(mess->id_len)) == -1)
        return invalid_PDU(p);

    if (calc_MESS_checksum(mess) != 255)
        return invalid_PDU(p);
    return p;
}

pdu *deserialize_QUIT(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    QUIT_msg *quit = malloc(sizeof(QUIT_msg));
    if (quit == NULL) sys_error(p, "malloc()");
    p->msg = quit;
    p->op = QUIT;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get_padd(sock, buffer, ind, 3) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_JOIN(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    JOIN_msg *join = malloc(sizeof(JOIN_msg));
    if (join == NULL) sys_error(p, "malloc()");
    p->msg = join;
    join->id = NULL;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &join->len) == -1 ||
        get_padd(sock, buffer, ind, 2) == -1)
        return invalid_PDU(p);

    join->id = calloc((unsigned int) (join->len + 1), 1);
    if (join->id == NULL) sys_error(p, "calloc()");

    if (get_str(sock, buffer, ind, join->len, join->id) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(join->len)) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_PJOIN(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    PJOIN_msg *pjoin = malloc(sizeof(PJOIN_msg));
    if (pjoin == NULL) sys_error(p, "malloc()");
    p->op = PJOIN;
    p->msg = pjoin;
    pjoin->id = NULL;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &pjoin->len) == -1 ||
        get_padd(sock, buffer, ind, 2) == -1 ||
        get32(sock, buffer, ind, &pjoin->timestamp) == -1)
        return invalid_PDU(p);

    pjoin->id = calloc((unsigned int) (pjoin->len + 1), 1);
    if (pjoin->id == NULL) sys_error(p, "calloc()");

    if (get_str(sock, buffer, ind, pjoin->len, pjoin->id) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(pjoin->len)) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_PLEAVE(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    PLEAVE_msg *pleave = malloc(sizeof(PLEAVE_msg));
    if (pleave == NULL) sys_error(p, "malloc()");
    p->op = PLEAVE;
    p->msg = pleave;
    pleave->id = NULL;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &pleave->len) == -1 ||
        get_padd(sock, buffer, ind, 2) == -1 ||
        get32(sock, buffer, ind, &pleave->timestamp) == -1)
        return invalid_PDU(p);

    pleave->id = calloc((unsigned int) (pleave->len + 1), 1);
    if (pleave->id == NULL) sys_error(p, "calloc()");

    if (get_str(sock, buffer, ind, pleave->len, pleave->id) == -1 ||
        get_padd(sock, buffer, ind, calc_pad(pleave->len)) == -1)
        return invalid_PDU(p);

    return p;
}

pdu *deserialize_PART(int sock, uint8_t *buffer, int *ind) {
    pdu *p = malloc(sizeof(pdu));
    if (p == NULL) sys_error(p, "malloc()");
    PART_msg *part = malloc(sizeof(PART_msg));
    if (part == NULL) sys_error(p, "malloc()");
    p->op = PARTICIPANTS;
    p->msg = part;
    part->ids = NULL;

    if (get8(sock, buffer, ind, &p->op) == -1 ||
        get8(sock, buffer, ind, &part->nr_of_ids) == -1 ||
        get16(sock, buffer, ind, &part->ids_len) == -1)
        return invalid_PDU(p);

    if (part->ids_len == 0)
        return p;


    int found_ids = 0, buf_ind = 0;
    part->ids = calloc(sizeof(char *) * part->ids_len, 1);
    for (int i = 0; i < part->nr_of_ids; ++i) {
        part->ids[i] = NULL;
    }
    if (part->ids == NULL)
        return sys_error(p, "calloc()");

    char buf[256];
    memset(buf, 0, 256);

    for (int i = 0; i < part->ids_len; ++i) {
        if (buf_ind == 256 || get_str(sock, buffer, ind, 1, &buf[buf_ind]) == -1) {
            part->nr_of_ids = (uint8_t) found_ids;
            return invalid_PDU(p);
        }

        if (buf[buf_ind] == 0) {
            char *str = calloc((unsigned int) (buf_ind + 1), 1);
            if (str == NULL)
                sys_error(p, "calloc()");

            memcpy(str, &buf[0], (unsigned int) (buf_ind));
            part->ids[found_ids] = str;
            found_ids++;

            if (found_ids == part->nr_of_ids)
                break;

            buf_ind = -1;
        }
        buf_ind++;
    }


    if (get_padd(sock, buffer, ind, calc_pad(part->ids_len)) == -1)
        return invalid_PDU(p);

    return p;
}
