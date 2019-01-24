/******************************************************************************
 * FILE: modules.h
 ******************************************************************************
 * DESCRIPTION: This file contains helper functions for client.
 ******************************************************************************
 * AUTHORS: Jonas Sjödin & Linus Ström Hortell
 * DATE: October 2018
 * CS-users: id16jsn@cs.umu.se & oi16lsm@cs.umu.se
 ******************************************************************************
 */

#ifndef SRC_MODULES_H
#define SRC_MODULES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include "../server/queue.h"
#include "../protocols/protocols.h"
#include "time.h"

#define MAXMSGSIZE 4096

int terminate, active_socket;
pthread_mutex_t terminate_mutex;
pthread_cond_t p_cond_incoming;
pthread_mutex_t incoming_lock;
pthread_cond_t p_cond_connected;
pthread_mutex_t connected_lock;

struct thread_options {
    char *server, *server_port, *client_id;
    queue *incoming_q;
    int socket;
    int nameserver;
};

void determine_action(pdu *p);

void get_server_list(struct thread_options *options);

pdu *create_MESS(char *buf);

pdu *create_QUIT(void);

pdu *create_JOIN(struct thread_options *options);

void print_time(time_t timestamp);

void print_PJOIN(pdu *p);

void print_MESS(pdu *p);

void print_QUIT(void);

void print_PLEAVE(pdu *p);

void print_PARTICIPANTS(pdu *p);

void print_invalid_PDU(void);

void catch_signal(int signo);

#endif //SRC_MODULES_H
