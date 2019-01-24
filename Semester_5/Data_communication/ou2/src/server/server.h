#ifndef _SERVER
#define _SERVER

#include <ctype.h>
#include <zconf.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <pthread.h>
#include <netdb.h>

#include "queue.h"
#include "../protocols/protocols.h"


#endif
