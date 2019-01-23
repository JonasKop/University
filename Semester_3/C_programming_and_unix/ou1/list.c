/*
 * File: list.c
 * ------------
 * A double-linked list and it's fully implemented interface.
 *
 * Assignment 1
 * Course: Systemnära programmering HT17, Umeå Universitet
 * Author: Jonas Sjödin
 * Cs-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: 2017-09-01
 */

#include <stdio.h>
#include "list.h"

/*
 * Function: list_Create
 * ---------------------
 * Creates a list and returns it.
 *
 * INPUT:   NONE.
 *
 * OUTPUT:  Returns an empty allocated list.
 */
list *list_Create(void){
    list *l = malloc(sizeof(list));

    l->first = malloc(sizeof(element));
    l->last = malloc(sizeof(element));
    l->first->next = l->last;
    l->last->prev = l->first;
    l->size = 0;
    return l;
}

/*
 * Function: list_Insert
 * ---------------------
 * Inserts data to the end of the list.
 *
 * INPUT:   l: a list which the data should be added to.
 *          c: the data that should be added to the list.
 *
 * OUTPUT:  A list with an added element.
 */
void list_Insert(list *l, data d){
    element *e = malloc(sizeof(element));

    e->value = d;
    l->last->prev->next = e;
    e->prev = l->last->prev;
    l->last->prev = e;
    e->next = l->last;
    l->size++;
}

/*
 * Function: list_IsEmpty
 * ----------------------
 * Checks if the list is empty and returns a boolean result.
 *
 * INPUT:   l: the list that should be checked if it is empty.
 *
 * OUTPUT:  True if list is empty, else false.
 */
bool list_IsEmpty(list *l) {
    return (l->first->next == l->last);
}

/*
 * Function: list_GetValue
 * -----------------------
 * Gets a elements value.
 *
 * INPUT:   e: The element from which the value should be gotten from.
 *
 * OUTPUT:  Returns the elements value.
 */
data list_GetValue(element *e) {
    return e->value;
}
/*
 * Function: list_Next
 * -------------------
 * Returns the next element in the list.
 *
 * INPUT:   e: The previous element.
 *
 * OUTPUT:  Returns the next element.
 */
element *list_Next(element *e) {
    return e->next;
}

/*
 * Function: list_Prev
 * -------------------
 * Returns the previous element in the list.
 *
 * INPUT:   e: The next element.
 *
 * OUTPUT:  Returns the previous element.
 */
element *list_Prev(element *e) {
    return e->prev;
}

/*
 * Function: list_First
 * --------------------
 * Returns the first element in the list.
 *
 * INPUT:   l: The list.
 *
 * OUTPUT:  Returns the first element in the list.
 */
element *list_First(list *l) {
    return l->first;
}

/*
 * Function: list_Last
 * ----------------------
 * Returns the last element in the list.
 *
 * INPUT:   l: The list.
 *
 * OUTPUT:  Returns the last element in the list.
 */
element *list_Last(list *l) {
    return l->last;
}

/*
 * Function: list_Remove
 * ----------------------
 * Removes a desired element from the list and frees it.
 *
 * INPUT:   e: The element that should be removed.
 *
 * OUTPUT:  A list with one less element.
 */
void list_Remove(element *e){
    e->prev->next = e->next;
    e->next->prev = e->prev;
    free(e->value);
    free(e);
}

/*
 * Function: list_Free
 * ----------------------
 * Frees the complete list.
 *
 * INPUT:   l: a list
 *
 * OUTPUT:  A deallocated and destroyed list.
 */
void list_Free(list *l) {

    element *next;
    for (element *e = l->first->next; e != l->last; e = next){
        next = e->next;

        for (size_t i = 0; i < 7; ++i)
            if (e->value[i] != NULL)
                free(e->value[i]);
        free(e->value);
        free(e);
    }

    free(l->first);
    free(l->last);
    free(l);
}