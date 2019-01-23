/*
 * File: mpasswdsort.c
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

#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "mpasswdsort.h"

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
FILE *openFile(char *path, bool *error) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "%s: No such file or directory\n", path);
        *error = true;
    }
    return file;
}

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
void twoColons(char **arr, int index, bool *error, int *success, int line) {

    if (index == 1 || index == 4) {
        arr[index] = NULL;
    } else {
        if (index == 2) {
            fprintf(stderr, "ERROR Line: %d: UID cannot be empty.\n", line);
        } else if (index == 3) {
            fprintf(stderr, "ERROR Line: %d: GID cannot be empty.\n", line);
        } else if (index == 5) {
            fprintf(stderr, "ERROR Line: %d: Directory cannot be empty.\n",
                    line);
        }
        *error = true;
        *success = index;
    }
}

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
void isUIDGIDaNr(char **arr, int index, bool *error, int *success, int line) {
    char *p;
    long val = strtol(arr[index], &p, 10);

    bool b = true;
    for (size_t j = 0; j < strlen(arr[index]); ++j) {
        if (isdigit(arr[index][j]) == 0) {
            b = false;
            break;
        }
    }
    if ((b && val < 0) || !b) {

        if (index == 2) {
            fprintf(stderr, "ERROR Line: %d: UID can only be a positive "
                    "number.\n", line);
        } else {
            fprintf(stderr, "ERROR Line: %d: GID can only be a positive "
                    "number.\n", line);
        }
        *error = true;
        *success = index + 1;
    }
}

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
char **splitString(char *s, int *success, int line, bool *error) {
    size_t len = strlen(s) - 1;
    char **arr = calloc(1, sizeof(char *) * 8);
    size_t start = 0;
    int index = 0;
    int colons = 0;
    *success = 10;

    for (size_t i = 0; i < len; ++i)
        if (s[i] == ':')
            colons++;

    if (colons != 6) {
        fprintf(stderr, "ERROR Line: %d: Invalid format: %s", line, s);
        *error = true;
        *success = 0;
    } else {
        for (size_t i = 0; i < len; ++i) {
            if (s[i] == ':') {
                if (i > 0) {
                    if (s[i - 1] == ':' || i == (len - 1)) {
                        twoColons(arr, index, error, success, line);
                        if (index != 1 && index != 4) {
                            break;
                        }
                    } else {
                        arr[index] = calloc(1, sizeof(char *) * (i - start + 4));
                        strncpy(arr[index], s + start, i - start);
                        if (index == 0 && strlen(arr[index]) > 32) {
                            fprintf(stderr, "ERROR Line: %d: Username must be"
                                    " between 1-32 characters.\n", line);
                            *error = true;
                            *success = index;
                            break;
                        } else if (index == 2 || index == 3) {
                            isUIDGIDaNr(arr, index,error, success, line);
                            if (*success == index + 1){
                                break;
                            }
                        }
                    }

                    start = i + 1;
                    index++;

                    if (index == 6) {
                        if (i == (len - 1)) {
                            fprintf(stderr, "ERROR Line: %d: Shell cannot be "
                                    "empty.\n", line);
                            *error = true;
                            *success = index;
                            break;
                        }
                        arr[index] = calloc(1, sizeof(char *) * (i - start + 4));
                        strncpy(arr[index], s + start, strlen(s) - (start + 1));
                        break;
                    }
                } else {
                    fprintf(stderr, "ERROR Line: %d: Username must be between "
                            "1-32 characters.\n", line);
                    *error = true;
                    *success = index;
                    break;
                }
            }
        }
    }
    return arr;
}

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
void readInput(list *l, FILE *stream, bool *error) {
    char *buffer = calloc(1, 1024);
    int line = 1;
    int *index = calloc(1, sizeof(int) + 1);
    while (fgets(buffer, 1024, stream)) {
        if (strlen(buffer) >= 12) {

            char **arr = splitString(buffer, index, line, error);

            //If the input line is not valid then free the memory.
            if (*index != 10) {
                for (int j = 0; j < *index; ++j)
                    free(arr[j]);
                free(arr);
            } else {
                list_Insert(l, (data) arr);
            }
        } else if (strlen(buffer) - 1 == 0) {
            fprintf(stderr, "ERROR Line: %d: Blank line.\n", line);
        } else {
            fprintf(stderr, "ERROR Line: %d: Invalid format: %s", line, buffer);
        }
        line++;
    }
    free(index);
    free(buffer);
}

/*
 * Function: printList
 * -------------------
 * Prints the lists UID and username.
 *
 * INPUT: *l: pointer to a list.
 *
 * OUTPUT: Prints UID and username to stdout.
 */
void printList(list *l){
    for (element *e = l->first->next; e != l->last; e = e->next) {
        char *p2 = e->value[2];
        char *p0 = e->value[0];
        printf("%s:%s\n", p2, p0);
    }
}

/*
 * Function: sortList
 * ------------------
 * Sorts a given list of items from /etc/passwd in ascending order by UID.
 *
 * INPUT: *l: pointer to a list.
 *
 * OUTPUT: A sorted list.
 */
void sortList(list *l) {

    for (int i = 1; i < l->size; ++i) {
        element *next = l->first->next->next;
        for (int j = 1; j < l->size; ++j) {
            element *e = next;
            char *p;

            if (strtol(e->prev->value[2], &p, 10) > strtol(e->value[2], &p, 10)) {
                next = e;
                swapElements(e->prev, e);
            } else {
                next = e->next;
            }
        }
    }
}

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
void swapElements(element *e1, element *e2) {
    e2->prev = e1->prev;
    e2->prev->next = e2;
    e1->next = e2->next;
    e1->next->prev = e1;
    e2->next = e1;
    e1->prev = e2;
}

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
int main(int argc, char *argv[]) {
    UNUSED(argc);
    bool *error = malloc(sizeof(bool) + 1);
    *error = false;
    list *l;

    if (argv[1] == NULL){
        l = list_Create();
        readInput(l, stdin, error);
    } else {
        FILE *fp = openFile(argv[1], error);
        if (*error){
            free(error);

            return EXIT_FAILURE;
        }
        l = list_Create();
        readInput(l, fp, error);
        fclose(fp);
    }

    sortList(l);
    printList(l);
    list_Free(l);
    if (*error) {
        free(error);
        return EXIT_FAILURE;
    }
    free(error);
    return 0;
}