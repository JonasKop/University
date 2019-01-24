//
// Created by jonaskop on 2018-10-04.
//

#ifndef SRC_CLIENT_H
#define SRC_CLIENT_H

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
#include <time.h>
#include <signal.h>
#include "../protocols/protocols.h"
#include "../server/queue.h"
#include "modules.h"


int load_args(struct thread_options *options, const char *argv[]);

void initialize(struct thread_options *options);

void *init_nameserver(void *arg);

void connect_to_server(struct thread_options *options);

void choose_server(struct thread_options *options, SLIST_msg *slist_msg);

void *connect_and_listen(void *arg);

void *handle_pdu(void *arg);

void *writing_prompt(void *arg);

void cleanup(struct thread_options *options);

int should_terminate(void);

#endif //SRC_CLIENT_H
