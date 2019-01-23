/*
 * File: mpasswdsort.h
 * ------------
 * The main file that runs mpasswdsort which is the first assignment in the
 * course Systemnära programmering at Umeå University. It reads given info
 * from stdin or a file in same format as /etc/passwd. The program then sorts
 * the input lines in ascending order by UID and prints the result.
 *
 * Assignment 1
 * Course: Systemnära programmering HT17, Umeå Universitet
 * Author: Jonas Sjödin
 * Cs-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: 2017-09-07
 */

#ifndef OU1_MPASSWDSORT_H_H
#define OU1_MPASSWDSORT_H_H

#define UNUSED(x) (void)(x)

#include <stdio.h>
#include "list.h"

/*
 * Function: openFile
 * -------------------
 * Opens a file and returns it.
 *
 * INPUT: *path: the path to the file that should be opened.
 *        *error: pointer to a bool that tells if an error has occurred or not.
 *
 *
 * OUTPUT: A split string that's split by each ':'
 */
FILE *openFile(char *path, bool *error);

/*
 * Function: twoColons
 * -------------------
 * If there are two colons next to each other then this function runs. It
 * checks if the given index to the array is ok to be null or not. If it isn't
 * it prints to stderr an error code.
 *
 * INPUT:
 *        *arr: the array with strings.
 *        index: the index that should be checked.
 *        *error: pointer to the error bool.
 *        *success: A pointer to an int. If it is 10 when the program ends it
 *                  has succeeded. Else it hasn't and the returned arr should
 *                  be freed.
 *        line: the current read line.
 *
 * OUTPUT: Error if it isn't index 1 or 4 else nothing.
 */
void twoColons(char **arr, int index, bool *error, int *success, int line);

/*
 * Function: isUIDorGIDaNr
 * -----------------------
 * This function checks if the requested string is only nrs.
 *
 * INPUT:
 *        *arr: the array with strings.
 *        index: the index that should be checked.
 *        *error: pointer to the error bool.
 *        *success: A pointer to an int. If it is 10 when the program ends it
 *                  has succeeded. Else it hasn't and the returned arr should
 *                  be freed.
 *        line: the current read line.
 *
 * OUTPUT: Error if it isn't only nrs else nothing.
 */
void isUIDGIDaNr(char **arr, int index, bool *error, int *success, int line);

/*
 * Function: splitString
 * -------------------
 * Splits a given string and prints error to stderr if errors should occur.
 *
 * INPUT: *s: pointer to the string that should be split.
 *        *success: A pointer to an int. If it is 10 when the program ends it
 *                  has succeeded. Else it hasn't and the returned arr should
 *                  be freed.
 *        line: the current read line.
 *        *error: pointer to the error bool.
 *
 * OUTPUT: A split string that's split by each ':'
 */
char **splitString(char *s, int *success, int line, bool *error);

/*
 * Function: readInput
 * -------------------
 * Reads input from file or stdin and saves it in a given list. If error occurs
 * it changes the pointer so the program later executes with an error code.
 *
 * INPUT: *l: pointer to a list.
 *        *stream: pointer to a file stream.
 *        *error: pointer to the error bool.
 *
 * OUTPUT: A list filled with info from the given stream.
 */
void readInput(list *l, FILE *stream, bool *error);

/*
 * Function: printList
 * -------------------
 * Prints the lists UID and username.
 *
 * INPUT: *l: pointer to a list.
 *
 * OUTPUT: Prints UID and username to stdout.
 */
void printList(list *l);

/*
 * Function: sortList
 * ------------------
 * Sorts a given list of items from /etc/passwd in ascending order by UID.
 *
 * INPUT: *l: pointer to a list.
 *
 * OUTPUT: A sorted list.
 */
void sortList(list *l);

/*
 * Function: swapElements
 * -------------------
 * Swaps the position of element e1 and e2 with each other.
 *
 * INPUT:   *e1: element preceding e2 in list.
 *          *e2: element after e1 in list.
 *
 * OUTPUT:  The position of the elements has been swapped.
 */
void swapElements(element *e1, element *e2);

/*
 * Function: main
 * --------------
 * Main function that runs the whole program.
 *
 * INPUT:   argc: Not currently in use.
 *          *argv[]: If input stream is a file then this is it.
 *
 * OUTPUT:  Returns 0 if success, otherwise something else.
 */
int main(int argc, char *argv[]);

#endif //OU1_MPASSWDSORT_H_H
