/*
 * File: list.h
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

#ifndef _OU1_LIST_H
#define _OU1_LIST_H

#include <stdlib.h>
#include <stdbool.h>

#ifndef _DATA
#define _DATA
typedef void **data;
#endif

typedef struct node {
    struct node *next;
    struct node *prev;
    data value;
} element;

typedef struct linked_list {
    element *first;
    element *last;
    int size;
} list;


/*
 * Function: list_Create
 * ---------------------
 * Creates a list and returns it.
 *
 * INPUT:   NONE.
 *
 * OUTPUT:  Returns an empty allocated list.
 */
list *list_Create(void);

/*
 * Function: list_Insert
 * ---------------------
 * Inserts data to the end of the list.
 *
 * INPUT:   *l: a list which the data should be added to.
 *          d: the data that should be added to the list.
 *
 * OUTPUT:  A list with an added element.
 */
void list_Insert(list *l, data d);

/*
 * Function: list_IsEmpty
 * ----------------------
 * Checks if the list is empty and returns a boolean result.
 *
 * INPUT:   *l: the list that should be checked if it is empty.
 *
 * OUTPUT:  True if list is empty, else false.
 */
bool list_IsEmpty(list *l);

/*
 * Function: list_GetValue
 * -----------------------
 * Gets a elements value.
 *
 * INPUT:   *e: The element from which the value should be gotten from.
 *
 * OUTPUT:  Returns the elements value.
 */
data list_GetValue(element *e);

/*
 * Function: list_Next
 * -------------------
 * Returns the next element in the list.
 *
 * INPUT:   *e: The previous element.
 *
 * OUTPUT:  Returns the next element.
 */
element *list_Next(element *e);

/*
 * Function: list_Prev
 * -------------------
 * Returns the previous element in the list.
 *
 * INPUT:   *e: The next element.
 *
 * OUTPUT:  Returns the previous element.
 */
element *list_Prev(element *e);

/*
 * Function: list_First
 * --------------------
 * Returns the first element in the list.
 *
 * INPUT:   *l: The list.
 *
 * OUTPUT:  Returns the first element in the list.
 */
element *list_First(list *l);

/*
 * Function: list_Last
 * ----------------------
 * Returns the last element in the list.
 *
 * INPUT:   *l: The list.
 *
 * OUTPUT:  Returns the last element in the list.
 */
element *list_Last(list *l);

/*
 * Function: list_Remove
 * ----------------------
 * Removes a desired element from the list and frees it.
 *
 * INPUT:   *e: The element that should be removed.
 *
 * OUTPUT:  A list with one less element.
 */
void list_Remove(element *e);

/*
 * Function: list_Free
 * ----------------------
 * Frees the complete list.
 *
 * INPUT:   *l: a list
 *
 * OUTPUT:  A deallocated and destroyed list.
 */
void list_Free(list *l);

#endif //_OU1_LIST_H
