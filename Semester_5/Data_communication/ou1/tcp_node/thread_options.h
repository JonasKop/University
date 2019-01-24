//
// Created by Jonas Sjödin on 2018-09-11.
//

#ifndef OU1_THREAD_OPTIONS_H
#define OU1_THREAD_OPTIONS_H

#include "queue.h"
#include <pthread.h>

#define BUFSIZE 100

struct thread_options {
    pthread_cond_t *p_cond;
    pthread_mutex_t *m;
    char *localport, *nextport, *nexthost, *domain;
    queue *q;
};

void terror(char *err);

#endif //OU1_THREAD_OPTIONS_H
