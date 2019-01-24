/*
 * Queue.c
 * Authors: Jonas Sjödin & Linus Ström
 * Description: A simple and effective queue. Use only for small queues since insert is O(n).
 * Course: 5DV197 @ Umeå University
 * Date: September 2019
 * Copyright: Use freely.
 */

#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include "queue.h"

/*
 * queue_create()
 * ----------------------------------------------------------------------------
 * Creates a queue, sets the first value to NULL and initiates the mutex and returns it.
 * ----------------------------------------------------------------------------
 * INPUT:   NONE
 * ----------------------------------------------------------------------------
 * OUTPUT:  an empty queue
 */
queue *queue_create(void) {
    queue *q = malloc(sizeof(queue));
    q->first = NULL;
    q->mutex = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(q->mutex, NULL);
    return q;
}

/*
 * queue_is_empty()
 * ----------------------------------------------------------------------------
 * Checks if the given queue is empty and returns boolean result.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that should be checked
 * ----------------------------------------------------------------------------
 * OUTPUT:  an empty queue
 */
int queue_is_empty(queue *q) {
    return q->first == NULL;
}

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
void queue_push(queue *q, data_t *d) {
    pthread_mutex_lock(q->mutex);
    queue_node *n = malloc(sizeof(queue_node));
    n->next = NULL;
    n->data = d;

    if (queue_is_empty(q)) {
        q->first = n;
    } else {
        queue_node *n1;
        for (n1 = q->first; n1->next != NULL; n1 = n1->next);
        n1->next = n;
    }
    pthread_mutex_unlock(q->mutex);
}

/*
 * queue_pop()
 * ----------------------------------------------------------------------------
 * Removes the first item of the queue and returns its data.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that the item should be removed from
 * ----------------------------------------------------------------------------
 * OUTPUT:  The first item in the queues data
 */
data_t *queue_pop(queue *q) {
    pthread_mutex_lock(q->mutex);
    if (queue_is_empty(q)) return NULL;

    queue_node *first = q->first;
    q->first = q->first->next;
    data_t *d = first->data;
    free(first);
    pthread_mutex_unlock(q->mutex);
    return d;
}

/*
 * queue_top()
 * ----------------------------------------------------------------------------
 * Returns the first item in the queues data.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that the item should be taken from
 * ----------------------------------------------------------------------------
 * OUTPUT:  The first item in the queues data
 */
data_t *queue_top(queue *q) {
    return queue_is_empty(q) ? NULL : q->first->data;
}

/*
 * queue_free()
 * ----------------------------------------------------------------------------
 * Removes all items in the queue and frees everything.
 * ----------------------------------------------------------------------------
 * INPUT:   queue *q: The queue that should be destroyed and freed
 * ----------------------------------------------------------------------------
 * OUTPUT:  NONE
 */
void queue_free(queue *q) {
    pthread_mutex_lock(q->mutex);

    while (!queue_is_empty(q)) {
        queue_node *first = q->first;
        q->first = q->first->next;
        free(first->data);
        free(first);
    }

    pthread_mutex_unlock(q->mutex);
    pthread_mutex_destroy(q->mutex);
    free(q->mutex);
    free(q);
}