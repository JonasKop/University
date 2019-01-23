/*
 * File: mfind.h
 * ----------------------------------------------------------------------------
 * Description: A program that is a simple implementation of find. It searches
 *              through the file system for the requested file. It has support
 *              for multiple thread use. The program takes to optional flags
 *              -t and -p where -t says which type it should search for d for
 *              directory, l for link and f for file. And -p tells the program
 *              how many threads that should be used. Then one or more start
 *              directories are required to be entered and lastly the name of
 *              the required file/dir/link.
 * ----------------------------------------------------------------------------
 * Example: ./mfind -t f -p 6 /home/userdir/ file1
 * ----------------------------------------------------------------------------
 * Assignment 4
 * Course: Systemnära programmering HT17, Umeå Universitet
 * Author: Jonas Sjödin
 * Cs-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */

#include "stack.h"
#include "list.h"
#include "stdbool.h"

#ifndef MFIND_MFIND_H_H
#define MFIND_MFIND_H_H

/*
 * create_threads
 * ----------------------------------------------------------------------------
 * Description: Creates nrthr nr of threads and makes them run the search
 *              function. Then it joins the threads.
 * ----------------------------------------------------------------------------
 * Input:   v: The args_t struct
 * ----------------------------------------------------------------------------
 * Output:  NONE (Created threads which run search and then are joined).
 */
void create_threads(void *v);

/*
 * search
 * ----------------------------------------------------------------------------
 * Description: Searches through the given args_t lokking for the searched
 *              file/folder/link and if it finds it it adds it to the list. All
 *              found folders are added to the stack which it then iterates
 *              through.
 * ----------------------------------------------------------------------------
 * Input: v: the void pointer. It is a pointer to a args_t struct.
 * ----------------------------------------------------------------------------
 * Output: None, (Returns NULL but it is of no use). If the correct file/folder
 *         /link is found its searchpath has been added to the list.
 */
void *search(void *v);

/*
 * do_while_empty_stack
 * ----------------------------------------------------------------------------
 * Description: Operations which the search function should execute while the
 *              stack is empty. When the search is finished it prints the
 *              threads ID and the nr of reads it has done.
 * ----------------------------------------------------------------------------
 * Input:   read_dirs: the number of read directories by the thread.
 * ----------------------------------------------------------------------------
 * Output:  0 if the thread should continue, -1 if it should quit.
 */
int do_while_empty_stack(int read_dirs);

/*
 * read_directory
 * ----------------------------------------------------------------------------
 * Description: Reads a directory and checks if a file/folder/link is the one
 *              that we are searching for and if so, it adds it to the list. If
 *              it is a folder it adds it (also) to the stack.
 * ----------------------------------------------------------------------------
 * Input:   current_name: The current name of a file/folder/link which were
 *                        looking for.
 *          current_dir: The current directory which is being searched.
 *          void *v: the search data from the struct args_t.
 * ----------------------------------------------------------------------------
 * Output:  Nothing except it might have been added to stack if it is a folder
 *          and added to the list if it is what were searching for.
 */
void read_directory(char *current_name, char *current_dir, void *v);

/*
 * add_dir
 * ----------------------------------------------------------------------------
 * Description: Adds a directory to the given stack.
 * ----------------------------------------------------------------------------
 * Input:   str_size: the size of the given directory
 *          str: the directory
 *          s: the stack
 * ----------------------------------------------------------------------------
 * Output:  A directory has been added to the stack
 */
void add_dir(size_t str_size, char *str, stack *s);

/*
 * check_if_correct_file
 * ----------------------------------------------------------------------------
 * Description: Checks if a file/folder/link should is the one we're searching
 *              for.
 * ----------------------------------------------------------------------------
 * Input:   type: The type that should be checked for.
 *          name: The name that should be searched for
 *          current_name: The current name of the file/folder/link
 *          mt: st_mode after a lstat has been run. Used to check what kind of
 *              file/folder/link it is
 * ----------------------------------------------------------------------------
 * Output:  Returns true if it is the file/folder/link were searching for, else
 *          false.
 */
bool check_if_correct_file(char type, char *name, char *current_name, __mode_t mt);


/*
 * get_options
 * ----------------------------------------------------------------------------
 * Description: Parses through the arguments of argv and sets the type param
 *              and nrthr param if entered.
 * ----------------------------------------------------------------------------
 * Input:   type: the type var that takes the -t flag
 *          nrthr: the nr of threads var that takes the -p flag
 *          argc: argc from main function
 *          argv: argv from main function
 * ----------------------------------------------------------------------------
 * Output:  Sets the type and nrthr pointers
 */
void get_options(char *type, int argc, char *argv[]);

/*
 * set_folders
 * ----------------------------------------------------------------------------
 * Description: sets the folder paths where the search should start. If the
 *              given file path doesn't exist it prints to stderr and exits.
 *              Duplicates and sub dirs are removed.
 * ----------------------------------------------------------------------------
 * Input:   argc: args from main function
 *          argv: argv from main function
 *          dirs: list that will contain the fold paths
 * ----------------------------------------------------------------------------
 * Output:  folder paths are set to the given list
 */
void set_folders(int argc, char *argv[], list *dirs);

/*
 * print_list
 * ----------------------------------------------------------------------------
 * Description: Prints the given list
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be printed
 * ----------------------------------------------------------------------------
 * Output:  The list is printed to stdout
 */
void print_list(list *l);

/*
 * remove_sub_dirs
 * ----------------------------------------------------------------------------
 * Description: removes all duplicates and sub-directories in a given list
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be checked
 * ----------------------------------------------------------------------------
 * Output: all duplicates and sub-dirs has been removed.
 */
void remove_sub_dirs(list *l);

#endif