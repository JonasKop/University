/*
 * File: stack.c
 * ----------------------------------------------------------------------------
 * Description: A thread safe stack and it's fully implemented
 *              interface.
 * ----------------------------------------------------------------------------
 * Assignment 4
 * Course: Systemnära programmering HT17, Umeå Universitet
 * Author: Jonas Sjödin
 * Cs-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */

#ifndef MFIND_STACK_H
#define MFIND_STACK_H

#include <pthread.h>
#include <stdbool.h>

typedef void *data_t;

typedef struct elem {
    void *data;
    struct elem *next;
} elem;

typedef struct stack {
    elem *top;
} stack;

/*
 * stack_create
 * ----------------------------------------------------------------------------
 * Description: Creates a stack annd returns it.
 * ----------------------------------------------------------------------------
 * Input: NONE
 * ----------------------------------------------------------------------------
 * Output: Returns the stack.
 */
stack *stack_create(void);

/*
 * stack_isEmpty
 * ----------------------------------------------------------------------------
 * Description: Checks if the stack is empty and returns the boolean result
 * ----------------------------------------------------------------------------
 * Input:   s:  the stack
 * ----------------------------------------------------------------------------
 * Output: The boolean result of if the stack is empty.
 */
bool stack_isEmpty(stack *s);

/*
 * stack_push
 * ----------------------------------------------------------------------------
 * Description: Pushes data to the top of the stack
 * ----------------------------------------------------------------------------
 * Input:   s:  The stack
 *          data:   The data that should be added to the stack
 * ----------------------------------------------------------------------------
 * Output:  The stack has an added value.
 */
void stack_push(stack *s, void *data);

/*
 * stack_pop
 * ----------------------------------------------------------------------------
 * Description: Removes the first element of the stack and saves its data to
 *              the val parameter.
 * ----------------------------------------------------------------------------
 * Input:   s: The stack
 *          val: The variable which the value of the top of the stack should
 *               be saved to
 * ----------------------------------------------------------------------------
 * Output: val has gotten a new value.
 */
void stack_pop(stack *s, void *val);

/*
 * stack_top
 * ----------------------------------------------------------------------------
 * Description: Returns the top of the stack.
 * ----------------------------------------------------------------------------
 * Input: The stack
 * ----------------------------------------------------------------------------
 * Output: Returns the element at the top of the stack
 */
elem *stack_top(stack *s);

/*
 * stack_free
 * ----------------------------------------------------------------------------
 * Description: Frees the complete stack and all of it's elements.
 * ----------------------------------------------------------------------------
 * Input:   s: The stack that should be freed.
 * ----------------------------------------------------------------------------
 * Output:  The stack has been freed.
 */
void stack_free(stack *s);

/*
 * stack_print
 * ----------------------------------------------------------------------------
 * Description: prints the stack to STDOUT
 * ----------------------------------------------------------------------------
 * Input:   s: the stack
 * ----------------------------------------------------------------------------
 * Output: The stacks content is printed to STDOUT.
 */
void stack_print(stack *s);

#endif //MFIND_STACK_H
