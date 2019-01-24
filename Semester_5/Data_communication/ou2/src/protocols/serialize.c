/*
 * File containing serializing functions which takes a thet and serializes a
 * PDU and sends it on the given thet to a client. It returns 0 on success
 * and -1 on error.
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>
#include "protocols.h"
#include "serialize.h"

/**
 * Prints the error string and returns -1
 *******************************************************************************
 * @param str the error string
 * @return -1
 */
int ser_sys_error(const char *str) {
    perror(str);
    return -1;
}

/**
 * Sends a buffer containing pdu data to the given thet and zeros the
 * buffer and sets ind to 0.
 *******************************************************************************
 * @param th the thet which the data should be sent on
 * @param buffer the buffer containing pdu data
 * @param ind index which will be reset to 0
 * @return 0 on success -1 on error
 */
int send_buf(int sock, uint8_t buffer[], int *ind) {

    if (sock == 0) {
        return -1;
    }
    if (send(sock, buffer, (unsigned int)*ind, 0) < 0) {
        return -1;
    }
    *ind = 0;
    memset(buffer, 0, BUFSIZE);
    return 0;
}

/**
 * Sets an int of a given size to the buffer, if larger than empty space in
 * buffer it sends the buffer and starts on a new buffer to send.
 *******************************************************************************
 * @param th the thet which should be sent on
 * @param buffer buffer where the int should be inserted
 * @param size size of int
 * @param ind index to insert value on
 * @param val Value which should be inserted into the buffer
 * @return 0 on sucess and -1 on error
 */
int set_int(int sock, uint8_t buffer[], int size, int *ind, uint32_t val) {
    if (size > 0) {
        uint8_t arr[size];
        memcpy(arr, &val, (unsigned int) size);
        for (int i = size - 1; i >= 0; i--) {
            if (*ind >= BUFSIZE && send_buf(sock, buffer, ind) == -1)
                return -1;

            buffer[*ind] = arr[i];
            (*ind)++;
        }
    }
    return 0;
}

/**
 * Sets an string of a given size to the buffer, if larger than empty space in
 * buffer it sends the buffer and starts on a new buffer to send.
 *******************************************************************************
 * @param th the thet which should be sent on
 * @param buffer buffer where the string should be inserted
 * @param size length of string
 * @param ind index to insert string on
 * @param str String which should be inserted into the buffer
 * @return 0 on sucess and -1 on error
 */
int set_str(int sock, uint8_t buffer[], int size, int *ind, char *str) {
    for (int i = 0; i < size; ++i) {
        if (*ind >= BUFSIZE && send_buf(sock, buffer, ind) == -1)
            return -1;

        buffer[*ind] = (uint8_t) str[i];
        (*ind)++;
    }

    return 0;
}

/**
 * Sets padding onto a buffer of given size. If larger than empty space in
 * buffer it sends the buffer and starts on a new buffer to send.
 *******************************************************************************
 * @param th the thet which should be sent on
 * @param buffer buffer where the string should be inserted
 * @param size length of string
 * @param ind index to insert string on
 * @return 0 on success and -1 on error
 */
int set_pad(int sock, uint8_t buffer[], int size, int *ind) {
    for (int i = 0; i < size; ++i) {
        if (*ind >= BUFSIZE && send_buf(sock, buffer, ind) == -1)
            return -1;
        buffer[*ind] = 0;
        (*ind)++;
    }

    return 0;
}

int serialize_REG(int sock, pdu *p) {
    REG_msg *reg = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, reg->len) == -1 ||
        set_int(sock, buffer, 2, &ind, reg->port) == -1 ||
        set_str(sock, buffer, reg->len, &ind, reg->server_name) == -1 ||
        set_pad(sock, buffer, calc_pad(reg->len), &ind) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_ACK(int sock, pdu *p) {
    ACK_msg *ack = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 1, &ind) == -1 ||
        set_int(sock, buffer, 2, &ind, ack->id) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_ALIVE(int sock, pdu *p) {
    ALIVE_msg *alive = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, alive->nr_of_clients) == -1 ||
        set_int(sock, buffer, 2, &ind, alive->id) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_NOTREG(int sock, pdu *p) {
    NOTREG_msg *notreg = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 1, &ind) == -1 ||
        set_int(sock, buffer, 2, &ind, notreg->id) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_GETLIST(int sock, pdu *p) {
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 3, &ind) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_SLIST(int sock, pdu *p) {
    SLIST_msg *slist = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 1, &ind) == -1 ||
        set_int(sock, buffer, 2, &ind, slist->nr_of_servers) == -1)
        return -1;


    for (int i = 0; i < slist->nr_of_servers; ++i) {

        for (int j = 0; j < 4; ++j) {
            if (set_int(sock, buffer, 1, &ind, slist->server_info[i]->addr[j]) == -1)
                return -1;
        }

        if (set_int(sock, buffer, 2, &ind, slist->server_info[i]->port) == -1 ||
            set_int(sock, buffer, 1, &ind, slist->server_info[i]->nr_of_clients) == -1 ||
            set_int(sock, buffer, 1, &ind, slist->server_info[i]->len) == -1 ||
            set_str(sock, buffer, slist->server_info[i]->len, &ind, slist->server_info[i]->server_name) == -1 ||
            set_pad(sock, buffer, calc_pad(slist->server_info[i]->len), &ind) == -1)
            return -1;
    }

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_MESS(int sock, pdu *p) {
    MESS_msg *mess = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    uint32_t checksum = 255 - (uint32_t) calc_MESS_checksum(mess);

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 1, &ind) == -1 ||
        set_int(sock, buffer, 1, &ind, mess->id_len) ||
        set_int(sock, buffer, 1, &ind, checksum) ||
        set_int(sock, buffer, 2, &ind, mess->msg_len) == -1 ||
        set_pad(sock, buffer, 2, &ind) == -1 ||
        set_int(sock, buffer, 4, &ind, mess->timestamp) == -1 ||
        set_str(sock, buffer, mess->msg_len, &ind, mess->msg) == -1 ||
        set_pad(sock, buffer, calc_pad(mess->msg_len), &ind) == -1)
        return -1;

    if (mess->client_id != NULL) {
        if (set_str(sock, buffer, mess->id_len, &ind, mess->client_id) == -1 ||
            set_pad(sock, buffer, calc_pad(mess->id_len), &ind) == -1)
            return -1;
    }


    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_QUIT(int sock, pdu *p) {
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_pad(sock, buffer, 3, &ind) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);
    return 0;
}

int serialize_JOIN(int sock, pdu *p) {
    JOIN_msg *join = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, join->len) == -1 ||
        set_pad(sock, buffer, 2, &ind) ||
        set_str(sock, buffer, join->len, &ind, join->id) ||
        set_pad(sock, buffer, calc_pad(join->len), &ind))
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_PJOIN(int sock, pdu *p) {
    PJOIN_msg *pjoin = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, pjoin->len) == -1 ||
        set_pad(sock, buffer, 2, &ind) ||
        set_int(sock, buffer, 4, &ind, pjoin->timestamp) ||
        set_str(sock, buffer, pjoin->len, &ind, pjoin->id) ||
        set_pad(sock, buffer, calc_pad(pjoin->len), &ind))
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_PLEAVE(int sock, pdu *p) {
    PLEAVE_msg *pleave = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, pleave->len) == -1 ||
        set_pad(sock, buffer, 2, &ind) ||
        set_int(sock, buffer, 4, &ind, pleave->timestamp) ||
        set_str(sock, buffer, pleave->len, &ind, pleave->id) ||
        set_pad(sock, buffer, calc_pad(pleave->len), &ind))
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}

int serialize_PART(int sock, pdu *p) {
    PART_msg *part = p->msg;
    uint8_t buffer[BUFSIZE];
    memset(buffer, 0, BUFSIZE);
    int ind = 0;

    if (set_int(sock, buffer, 1, &ind, p->op) == -1 ||
        set_int(sock, buffer, 1, &ind, part->nr_of_ids) == -1 ||
        set_int(sock, buffer, 2, &ind, part->ids_len) == -1)
        return -1;

    for (int i = 0; i < part->nr_of_ids; i++) {
        int len = (int) strlen(part->ids[i]);

        if (set_str(sock, buffer, len, &ind, part->ids[i]) == -1)
            return -1;
        if (set_pad(sock, buffer, 1, &ind) == -1)
            return -1;
    }
    if (set_pad(sock, buffer, part->ids_len, &ind) == -1)
        return -1;

    return send_buf(sock, buffer, &ind);

    return 0;
}