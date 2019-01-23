/*
 * File: list.c
 * ----------------------------------------------------------------------------
 * Description: A thread safe double-linked list and it's fully implemented
 *              interface.
 * ----------------------------------------------------------------------------
 * Assignment 4
 * Course: Systemnära programmering HT17, Umeå Universitet
 * Author: Jonas Sjödin
 * Cs-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */

#include <stdio.h>
#include <pthread.h>
#include "list.h"

/*
 * Function: list_create
 * ----------------------------------------------------------------------------
 * Description: Creates a list and returns it.
 * ----------------------------------------------------------------------------
 * Input:   NONE.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns an empty allocated list.
 */
list *list_create(void){
    list *l = malloc(sizeof(list));
    if (l == NULL) {
        perror("malloc");
        exit(1);
    }
    l->m = malloc(sizeof(pthread_mutex_t));
    if (l->m == NULL) {
        perror("malloc");
        exit(1);
    }
    pthread_mutex_init(l->m, NULL);
    l->first = malloc(sizeof(element));
    if (l->first == NULL) {
        perror("malloc");
        exit(1);
    }
    l->last = malloc(sizeof(element));
    if (l->last == NULL) {
        perror("malloc");
        exit(1);
    }
    l->first->next = l->last;
    l->last->prev = l->first;
    l->size = 0;
    return l;
}

/*
 * Function: list_Insert
 * ----------------------------------------------------------------------------
 * Description: Inserts data to the end of the list.
 * ----------------------------------------------------------------------------
 * Input:   l: a list which the data should be added to.
 *          d: the data that should be added to the list.
 * ----------------------------------------------------------------------------
 * OUTPUT:  A list with an added element.
 */
void list_Insert(list *l, data_t d){
    element *e = malloc(sizeof(element));
    if (e == NULL) {
        perror("malloc");
        exit(1);
    }

    pthread_mutex_lock(l->m);
    e->data = d;
    l->last->prev->next = e;
    e->prev = l->last->prev;
    l->last->prev = e;
    e->next = l->last;
    l->size++;
    pthread_mutex_unlock(l->m);
}

/*
 * Function: list_IsEmpty
 * ----------------------------------------------------------------------------
 * Description: Checks if the list is empty and returns -1 if not, if true 0.
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be checked if it is empty.
 * ----------------------------------------------------------------------------
 * OUTPUT:  True if list is empty, else false.
 */
int list_IsEmpty(list *l) {
    pthread_mutex_lock(l->m);
    int ret = (l->first->next == l->last);
    pthread_mutex_unlock(l->m);
    return ret-1;
}

/*
 * Function: list_GetValue
 * ----------------------------------------------------------------------------
 * Description: Gets a elements value.
 * ----------------------------------------------------------------------------
 * Input:   e: The element from which the value should be gotten from.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the elements value.
 */
data_t list_GetValue(element *e) {
    return e->data;
}
/*
 * Function: list_Next
 * ----------------------------------------------------------------------------
 * Description: Returns the next element in the list.
 * ----------------------------------------------------------------------------
 * Input:   e: The previous element.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the next element.
 */
element *list_Next(element *e) {
    return e->next;
}

/*
 * Function: list_Prev
 * ----------------------------------------------------------------------------
 * Description: Returns the previous element in the list.
 * ----------------------------------------------------------------------------
 * Input:   e: The next element.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the previous element.
 */
element *list_Prev(element *e) {
    return e->prev;
}

/*
 * Function: list_First
 * ----------------------------------------------------------------------------
 * Description: Returns the first element in the list.
 * ----------------------------------------------------------------------------
 * Input:   l: The list.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the first element in the list.
 */
element *list_First(list *l) {
    return l->first;
}

/*
 * Function: list_Last
 * ----------------------------------------------------------------------------
 * Description: Returns the last element in the list.
 * ----------------------------------------------------------------------------
 * Input:   l: The list.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the last element in the list.
 */
element *list_Last(list *l) {
    return l->last;
}

/*
     * Function: list_Remove
 * ----------------------------------------------------------------------------
 * Description: Removes a desired element from the list and frees it.
 * ----------------------------------------------------------------------------
 * Input:   e: The element that should be removed.
 * ----------------------------------------------------------------------------
 * OUTPUT:  A list with one less element.
 */
void list_Remove(pthread_mutex_t *m, element *e){
    pthread_mutex_lock(m);
    e->prev->next = e->next;
    e->next->prev = e->prev;
    free(e->data);
    free(e);
    pthread_mutex_unlock(m);
}


/*
 * Function: list_Free
 * ----------------------------------------------------------------------------
 * Description: Frees the complete list.
 * ----------------------------------------------------------------------------
 * Input:   l: a list
 * ----------------------------------------------------------------------------
 * OUTPUT:  A deallocated and destroyed list.
 */
void list_Free(list *l) {

    element *next;

    pthread_mutex_lock(l->m);
    for (element *e = l->first->next; e != l->last; e = next) {
        next = e->next;
        free(e->data);
        free(e);
    }

    free(l->first);
    free(l->last);
    pthread_mutex_unlock(l->m);
    free(l->m);
    free(l);
}