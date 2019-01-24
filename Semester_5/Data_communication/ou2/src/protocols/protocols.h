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

#ifndef _serv_name
#define _serv_name

#include "inttypes.h"

#define BUFSIZE 500000
#define MAXCLIENTS 256

#define REG 0
#define ACK 1
#define ALIVE 2
#define NOTREG 100
#define GETLIST 3
#define SLIST 4
#define MESS 10
#define QUIT 11
#define JOIN 12
#define PJOIN 16
#define PLEAVE 17
#define PARTICIPANTS 19

#define terror(err) {perror(err); exit(1);};


typedef struct PDU_t {
    uint8_t op;
    void *msg;
} pdu;

typedef struct REG_t {
    char *server_name;
    uint8_t len;
    uint16_t port;
} REG_msg;

typedef struct ACK_t {
    uint16_t id;
} ACK_msg;

typedef struct ALIVE_t {
    uint8_t nr_of_clients;
    uint16_t id;
} ALIVE_msg;

typedef struct NOTREG_t {
    uint16_t id;
} NOTREG_msg;

typedef void *GETLIST_msg;

typedef struct SLIST_ITEM_t {
    uint8_t addr[4], nr_of_clients, len;
    char *server_name;
    uint16_t port;
} SLIST_ITEM;

typedef struct SLIST_t {
    uint16_t nr_of_servers;
    SLIST_ITEM **server_info;
} SLIST_msg;

typedef struct JOIN_t {
    uint8_t len;
    char *id;
} JOIN_msg;

typedef struct PART_t {
    char **ids;
    uint8_t nr_of_ids;
    uint16_t ids_len;
} PART_msg;

typedef void *QUIT_msg;

typedef struct MESS_t {
    uint8_t id_len, checksum;
    char *msg, *client_id;
    uint16_t msg_len;
    uint32_t timestamp;
} MESS_msg;

typedef struct PJOIN_t {
    uint8_t len;
    char *id;
    uint32_t timestamp;
    int socket;
} PJOIN_msg;

typedef struct PLEAVE_t {
    uint8_t len;
    char *id;
    uint32_t timestamp;
} PLEAVE_msg;

/**
 * Calculate and return how much padding should be applied for a given length.
 *******************************************************************************
 * @param len length of string or nr of bytes
 * @return the amount of padding for len
 */
int calc_pad(int len);

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
pdu *retrieve_PDU(int sock, uint8_t *buffer, int *status, int *ind);

/**
 * Calculates the checksum for a MESS and returns it. It is calculated by the
 * using the byte sum of the MESS and removing 255 until it is inbetween
 * the range (0, 255).
 *******************************************************************************
 * @param mess the mess which should be calculated on
 * @return checksum of message
 */
int calc_MESS_checksum(MESS_msg *mess);

/**
 * If a buffer is read until completion run this and it reads from the socket
 * again and fills the buffer and sets ind to 0.
 *******************************************************************************
 * @param sock the socket which should be read from
 * @param buffer the buffer which is full
 * @param ind the index pointer
 * @return 0 if correct read, else -1
 */
int read_data(int sock, uint8_t buffer[], int *ind);

/**
 * Serializes a pdu and sends it on a given socket.
 *******************************************************************************
 * @param sock the socket where the pdu should be sent
 * @param p the pdu which should be sent
 * @return 0 if ok, else -1
 */
int serialize_PDU(int sock, pdu *p);

/**
 * Deserializes a PDU in a buffer and reads more from a sock if needed.
 * Returns the PDU
 * @param sock the socket to potentially read more from
 * @param buffer the buffer containing a PDU
 * @param ind the index in the buffer to start deserializing from.
 * @return a correct PDU if no problems has occurred, otherwise NULL.
 */
pdu *deserialize_PDU(int sock, uint8_t *buffer, int *ind);

/**
 * Destroys a PDU by freeing all of its allocated memory
 *******************************************************************************
 * @param p the PDU which should be destroyed and freed.
 */
void destroy_PDU(pdu *p);

#endif
