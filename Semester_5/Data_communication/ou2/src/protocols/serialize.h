/*
 * File containing serializing functions which takes a socket and serializes a
 * PDU and sends it on the given socket to a client. It returns 0 on success
 * and -1 on error.
 *******************************************************************************
 * Authors: Jonas Sjödin (id16jsn) Linus Ström (oi16lsh)
 * Date: October 2018
 * Course: Computer communication and computer networks.
 * Assignment: 2
 */

#ifndef _serialize
#define _serialize

#include "protocols.h"

int serialize_REG(int sock, pdu *p);

int serialize_ACK(int sock, pdu *p);

int serialize_ALIVE(int sock, pdu *p);

int serialize_NOTREG(int sock, pdu *p);

int serialize_GETLIST(int sock, pdu *p);

int serialize_SLIST(int sock, pdu *p);

int serialize_MESS(int sock, pdu *p);

int serialize_QUIT(int sock, pdu *p);

int serialize_JOIN(int sock, pdu *p);

int serialize_PJOIN(int sock, pdu *p);

int serialize_PLEAVE(int sock, pdu *p);

int serialize_PART(int sock, pdu *p);

#endif //SRC_serialize1_H
