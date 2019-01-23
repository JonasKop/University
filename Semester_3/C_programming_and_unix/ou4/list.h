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

#ifndef _OU1_LIST_H
#define _OU1_LIST_H

#include <stdlib.h>

#ifndef _DATA
#define _DATA
typedef void *data_t;
#endif

typedef struct node {
    struct node *next;
    struct node *prev;
    data_t data;
} element;

typedef struct linked_list {
    element *first;
    element *last;
    int size;
    pthread_mutex_t *m;
} list;


/*
 * Function: list_create
 * ----------------------------------------------------------------------------
 * Description: Creates a list and returns it.
 * ----------------------------------------------------------------------------
 * Input:   NONE.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns an empty allocated list.
 */
list *list_create(void);

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
void list_Insert(list *l, data_t d);

/*
 * Function: list_IsEmpty
 * ----------------------------------------------------------------------------
 * Description: Checks if the list is empty and returns -1 if not, if true 0.
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be checked if it is empty.
 * ----------------------------------------------------------------------------
 * OUTPUT:  True if list is empty, else false.
 */
int list_IsEmpty(list *l);

/*
 * Function: list_GetValue
 * ----------------------------------------------------------------------------
 * Description: Gets a elements value.
 * ----------------------------------------------------------------------------
 * Input:   e: The element from which the value should be gotten from.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the elements value.
 */
data_t list_GetValue(element *e);

/*
 * Function: list_Next
 * ----------------------------------------------------------------------------
 * Description: Returns the next element in the list.
 * ----------------------------------------------------------------------------
 * Input:   e: The previous element.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the next element.
 */
element *list_Next(element *e);

/*
 * Function: list_Prev
 * ----------------------------------------------------------------------------
 * Description: Returns the previous element in the list.
 * ----------------------------------------------------------------------------
 * Input:   e: The next element.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the previous element.
 */
element *list_Prev(element *e);

/*
 * Function: list_First
 * ----------------------------------------------------------------------------
 * Description: Returns the first element in the list.
 * ----------------------------------------------------------------------------
 * Input:   l: The list.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the first element in the list.
 */
element *list_First(list *l);

/*
 * Function: list_Last
 * ----------------------------------------------------------------------------
 * Description: Returns the last element in the list.
 * ----------------------------------------------------------------------------
 * Input:   l: The list.
 * ----------------------------------------------------------------------------
 * OUTPUT:  Returns the last element in the list.
 */
element *list_Last(list *l);

/*
 * Function: list_Remove
 * ----------------------------------------------------------------------------
 * Description: Removes a desired element from the list and frees it.
 * ----------------------------------------------------------------------------
 * Input:   e: The element that should be removed.
 * ----------------------------------------------------------------------------
 * OUTPUT:  A list with one less element.
 */
void list_Remove(pthread_mutex_t *m, element *e);


/*
 * Function: list_Free
 * ----------------------------------------------------------------------------
 * Description: Frees the complete list.
 * ----------------------------------------------------------------------------
 * Input:   l: a list
 * ----------------------------------------------------------------------------
 * OUTPUT:  A deallocated and destroyed list.
 */
void list_Free(list *l);
#endif //_OU1_LIST_H