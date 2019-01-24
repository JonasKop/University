/*
 * File containing PDU help functions for deserialize and serialize functions.
 * It also contains serializing, deserializing and destroy functions which
 * helps with handling PDUs.
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "protocols.h"
#include "deserialize.h"
#include "serialize.h"

/**
 * Calculate and return how much padding should be applied for a given length.
 *******************************************************************************
 * @param len length of string or nr of bytes
 * @return the amount of padding for len
 */
int calc_pad(int len) {
    return (4 - len % 4) % 4;
}

/**
 * Checks if there are bytes left in a given buffer after a specific index.
 *******************************************************************************
 * @param buffer the buffer possibly containing data
 * @param ind the startindex in the buffer
 * @return 0 if no bytes left, 1 if bytes left
 */
int is_bytes_left(const uint8_t *buffer, int *ind) {

    if (*ind < BUFSIZE && buffer[*ind] != 0) {
        return 1;
    }

    (*ind)++;
    return 0;
}

/**
 * Calculates checksum wraparound of an int and returns a sum between (0, 255).
 *******************************************************************************
 * @param sum that sum that should be wrapped around
 * @return the new sum
 */
int calc_wraparound(int sum) {
    while (sum > 255) {
        sum -= 255;
    }
    return sum;
}

/**
 * Calculates a strings byte sum and returns it
 *******************************************************************************
 * @param str the string which should be calculated on
 * @param len length of str
 * @return byte sum och string
 */
int calc_str_sum(const uint8_t *str, int len) {
    int sum = 0;
    for (int i = 0; i < len; ++i) {
        sum += (int) str[i];
    }
    return sum;
}

/**
 * Calculates an ints byte sum by a given size
 *******************************************************************************
 * @param val the integers value
 * @param size the byte size of the integer
 * @return byte sum of given int
 */
int calc_int_sum(unsigned int val, int size) {
    int sum = 0;
    if (size > 0) {
        uint8_t arr[size];
        memcpy(arr, &val, (unsigned int) size);

        for (int i = 0; i < size; i++) {
            sum += (int) arr[i];
        }
    }

    return sum;
}

/**
 * Calculates the checksum for a MESS and returns it. It is calculated by the
 * using the byte sum of the MESS and removing 255 until it is inbetween
 * the range (0, 255).
 *******************************************************************************
 * @param mess the mess which should be calculated on
 * @return checksum of message
 */
int calc_MESS_checksum(MESS_msg *mess) {
    int sum = MESS;
    sum += mess->id_len;
    sum += mess->checksum;
    sum += calc_int_sum(mess->timestamp, 4);
    sum += calc_int_sum(mess->msg_len, 2);
    if (mess->client_id != NULL) {
        sum += calc_str_sum((uint8_t *) mess->client_id, mess->id_len);
    }
    sum += calc_str_sum((uint8_t *) mess->msg, mess->msg_len);
    sum = calc_wraparound(sum);

    return sum;
}

/**
 * If a buffer is read until completion run this and it reads from the socket
 * again and fills the buffer and sets ind to 0.
 *******************************************************************************
 * @param sock the socket which should be read from
 * @param buffer the buffer which is full
 * @param ind the index pointer
 * @return 0 if correct read, else -1
 */
int read_data(int sock, uint8_t buffer[], int *ind) {

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    struct timeval tv = {0, 0};
    int ret = select(sock + 1, &readfds, NULL, NULL, &tv);

    if (ret == 0)
        return -1;
    (*ind) = 0;
    memset(buffer, 0, BUFSIZE);
    if (read(sock, buffer, BUFSIZE) < 0)
        return -1;
    return 0;
}

/**
 * Retrieves a PDU by deserializing it, if more PDUs are in the buffer it
 * sets status to -2 which signals to the reader that another PDU is in the
 * buffer. If all is correct status is 0 and a pdu is returned. If an error
 * has occurred status is set to -1 and NULL is returned.
 *******************************************************************************
 * @param sock the socket which should be read from
 * @param buffer the buffer containing a pdu
 * @param status pointer to status variable
 * @param ind pointer to index position to start read from in buffer
 * @return p if correct PDU else NULL
 */
pdu *retrieve_PDU(int sock, uint8_t *buffer, int *status, int *ind) {
    *status = 0;

    pdu *p = deserialize_PDU(sock, buffer, ind);

    if (p == NULL) {
        *status = -1;
        return NULL;
    }

    if (is_bytes_left(buffer, ind)) {
        *status = -2;
    }

    return p;
}

/**
 * Serializes a pdu and sends it on a given socket.
 *******************************************************************************
 * @param sock the socket where the pdu should be sent
 * @param p the pdu which should be sent
 * @return 0 if ok, else -1
 */
int serialize_PDU(int sock, pdu *p) {
    switch (p->op) {
        case REG:
            return serialize_REG(sock, p);
        case ACK:
            return serialize_ACK(sock, p);
        case ALIVE:
            return serialize_ALIVE(sock, p);
        case NOTREG:
            return serialize_NOTREG(sock, p);
        case GETLIST:
            return serialize_GETLIST(sock, p);
        case SLIST:
            return serialize_SLIST(sock, p);
        case MESS:
            return serialize_MESS(sock, p);
        case QUIT:
            return serialize_QUIT(sock, p);
        case JOIN:
            return serialize_JOIN(sock, p);
        case PJOIN:
            return serialize_PJOIN(sock, p);
        case PLEAVE:
            return serialize_PLEAVE(sock, p);
        case PARTICIPANTS:
            return serialize_PART(sock, p);
        default:
            return -1;
    }
}

/**
 * Deserializes a PDU in a buffer and reads more from a sock if needed.
 * Returns the PDU
 * @param sock the socket to potentially read more from
 * @param buffer the buffer containing a PDU
 * @param ind the index in the buffer to start deserializing from.
 * @return a correct PDU if no problems has occurred, otherwise NULL.
 */
pdu *deserialize_PDU(int sock, uint8_t *buffer, int *ind) {

    switch (buffer[0]) {
        case ACK:
            return deserialize_ACK(sock, buffer, ind);
        case NOTREG:
            return deserialize_NOTREG(sock, buffer, ind);
        case GETLIST:
            return deserialize_GETLIST(sock, buffer, ind);
        case SLIST:
            return deserialize_SLIST(sock, buffer, ind);
        case MESS:
            return deserialize_MESS(sock, buffer, ind);
        case QUIT:
            return deserialize_QUIT(sock, buffer, ind);
        case JOIN:
            return deserialize_JOIN(sock, buffer, ind);
        case PJOIN:
            return deserialize_PJOIN(sock, buffer, ind);
        case PLEAVE:
            return deserialize_PLEAVE(sock, buffer, ind);
        case PARTICIPANTS:
            return deserialize_PART(sock, buffer, ind);
        default:
            return NULL;
    }
}

/**
 * Destroys a PDU by freeing all of its allocated memory
 *******************************************************************************
 * @param p the PDU which should be destroyed and freed.
 */
void destroy_PDU(pdu *p) {
    if (p->op == REG) {
        REG_msg *reg = p->msg;
        if (reg->server_name != NULL)
            free(reg->server_name);
    } else if (p->op == SLIST) {
        SLIST_msg *slist = p->msg;
        for (int i = 0; i < slist->nr_of_servers; ++i) {
            if (slist->server_info[i]->server_name != NULL)
                free(slist->server_info[i]->server_name);
            if (slist->server_info[i] != NULL)
                free(slist->server_info[i]);
        }
        if (slist->server_info != NULL)
            free(slist->server_info);
    } else if (p->op == JOIN) {
        JOIN_msg *join = p->msg;
        if (join->id != NULL)
            free(join->id);
    } else if (p->op == PARTICIPANTS) {
        PART_msg *part = p->msg;
        for (int i = 0; i < part->nr_of_ids; ++i) {
            if (part->ids[i] != NULL)
                free(part->ids[i]);
        }
        if (part->ids != NULL)
            free(part->ids);
    } else if (p->op == PJOIN) {
        PJOIN_msg *pjoin = p->msg;
        if (pjoin->id != NULL)
            free(pjoin->id);
    } else if (p->op == PLEAVE) {
        PLEAVE_msg *pleave = p->msg;
        if (pleave->id != NULL)
            free(pleave->id);
    } else if (p->op == MESS) {
        MESS_msg *mess = p->msg;
        if (mess->client_id != NULL)
            free(mess->client_id);
        if (mess->msg != NULL)
            free(mess->msg);
    }

    free(p->msg);
    free(p);
}
