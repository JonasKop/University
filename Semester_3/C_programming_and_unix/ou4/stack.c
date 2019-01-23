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

#include "stack.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"

/*
 * stack_create
 * ----------------------------------------------------------------------------
 * Description: Creates a stack annd returns it.
 * ----------------------------------------------------------------------------
 * Input: NONE
 * ----------------------------------------------------------------------------
 * Output: Returns the stack.
 */
stack *stack_create(void) {

    stack *s = malloc(sizeof(stack));
    if (s == NULL) {
        perror("malloc");
        exit(1);
    }
    s->top = NULL;
    return s;
}

/*
 * stack_isEmpty
 * ----------------------------------------------------------------------------
 * Description: Checks if the stack is empty and returns the boolean result
 * ----------------------------------------------------------------------------
 * Input:   s:  the stack
 * ----------------------------------------------------------------------------
 * Output: The boolean result of if the stack is empty.
 */
bool stack_isEmpty(stack *s) {
    return s->top == NULL;
}

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
void stack_push(stack *s, void *data) {

    elem *ins = malloc(sizeof(elem));
    if (ins == NULL) {
        perror("malloc");
        exit(1);
    }
    ins->data = data;
    elem *top = s->top;
    s->top = ins;
    ins->next = top;
}

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
void stack_pop(stack *s, void *val) {

    elem *top = s->top;
    s->top = top->next;

    strcpy(val, top->data);
    free(top->data);
    free(top);
}

/*
 * stack_top
 * ----------------------------------------------------------------------------
 * Description: Returns the top of the stack.
 * ----------------------------------------------------------------------------
 * Input: The stack
 * ----------------------------------------------------------------------------
 * Output: Returns the element at the top of the stack
 */
elem *stack_top(stack *s) {
    elem *top = s->top;

    return top;
}

/*
 * stack_free
 * ----------------------------------------------------------------------------
 * Description: Frees the complete stack and all of it's elements.
 * ----------------------------------------------------------------------------
 * Input:   s: The stack that should be freed.
 * ----------------------------------------------------------------------------
 * Output:  The stack has been freed.
 */
void stack_free(stack *s) {

    elem *e = s->top;

    while (e != NULL) {
        elem *next = e->next;
        free(e->data);
        free(e);
        e = next;
    }
    free(s);
}

/*
 * stack_print
 * ----------------------------------------------------------------------------
 * Description: prints the stack to STDOUT
 * ----------------------------------------------------------------------------
 * Input:   s: the stack
 * ----------------------------------------------------------------------------
 * Output: The stacks content is printed to STDOUT.
 */
void stack_print(stack *s) {
    printf("------STACK-----\n");
    elem *e = s->top;
    while(e != NULL) {
        printf("%s\n", (char *) e->data);
        e = e->next;
    }
    printf("----------------\n");
}