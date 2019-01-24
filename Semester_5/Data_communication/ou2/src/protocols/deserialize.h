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

#ifndef _deserialize
#define _deserialize

#include "protocols.h"


pdu *deserialize_REG(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_ACK(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_ALIVE(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_NOTREG(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_GETLIST(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_SLIST(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_MESS(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_QUIT(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_JOIN(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_PJOIN(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_PLEAVE(int sock, uint8_t *buffer, int *ind);

pdu *deserialize_PART(int sock, uint8_t *buffer, int *ind);


#endif
