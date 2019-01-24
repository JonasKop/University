/*
 * Queue.h
 * Authors: Jonas Sjödin & Linus Ström
 * Description: A simple and effective queue. Use only for small queues since insert is O(n).
 * Course: 5DV197 @ Umeå University
 * Date: September 2019
 * Copyright: Use freely.
 */

#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

#include <sys/types.h>

typedef void data_t;

typedef struct node_t {
    struct node_t *next;
    data_t *data;
} node;

typedef struct queue_t {
    struct node_t *first;
    pthread_mutex_t *mutex;
} queue;

/*
 * queue_create()
 * ----------------------------------------------------------------------------
 * Creates a queue, sets the first value to NULL and initiates the mutex and returns it.
 * ----------------------------------------------------------------------------
 * INPUT:   NONE
 * ----------------------------------------------------------------------------
 * OUTPUT:  an empty queue
 */
queue *queue_create(void);

/*
 * queue_is_empty()
 * ----------------------------------------------------------------------------
 * Checks if the given queue is empty and returns boolean result.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that should be checked
 * ----------------------------------------------------------------------------
 * OUTPUT:  an empty queue
 */
int queue_is_empty(queue *q);

/*
 * queue_push()
 * ----------------------------------------------------------------------------
 * Inserts data at the end of the queue.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that the data should be inserted to
 *          data_t *d: The data that should be inserted
 * ----------------------------------------------------------------------------
 * OUTPUT:  NONE
 */
void queue_push(queue *q, data_t *d);

/*
 * queue_pop()
 * ----------------------------------------------------------------------------
 * Removes the first item of the queue and returns its data.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that the item should be removed from
 * ----------------------------------------------------------------------------
 * OUTPUT:  The first item in the queues data
 */
data_t *queue_pop(queue *q);

/*
 * queue_top()
 * ----------------------------------------------------------------------------
 * Returns the first item in the queues data.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that the item should be taken from
 * ----------------------------------------------------------------------------
 * OUTPUT:  The first item in the queues data
 */
data_t *queue_top(queue *q);

/*
 * queue_free()
 * ----------------------------------------------------------------------------
 * Removes all items in the queue and frees everything.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that should be destroyed and freed
 * ----------------------------------------------------------------------------
 * OUTPUT:  NONE
 */
void queue_free(queue *q);

#endif //QUEUE_QUEUE_H
