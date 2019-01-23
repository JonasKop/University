/*
 * File: mfind.c
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "mfind.h"

size_t nrthr = 1;
int waiting = 0;
bool all_threads_finished = false;

pthread_mutex_t *lock;
pthread_cond_t *cond;

typedef struct args {
    stack *s;
    list *l;
    char *name;
    char type;
} args_t;

int main(int argc, char *argv[]) {

    char type = '*';
    char *name = argv[argc - 1];
    if (name[0] == '/') {
        fprintf(stderr, "Not a valid name: %s\n", name);
        exit(1);
    }

    list *dirs = list_create();

    get_options(&type, argc, argv);

    set_folders(argc, argv, dirs);

    stack *s = stack_create();
    list *l = list_create();

    while (list_IsEmpty(dirs) == -1) {
        element *e = list_First(dirs)->next;

        char *stack_string = malloc(sizeof(char) * (strlen(e->data) + 1));

        if (stack_string == NULL) {
            perror("malloc");
            exit(1);
        }

        strcpy(stack_string, e->data);
        stack_push(s, stack_string);

        list_Remove(l->m, e);
    }
    list_Free(dirs);

    args_t *search_data = malloc(sizeof(args_t));
    if (search_data == NULL) {
        perror("malloc");
        exit(1);
    }

    search_data->name = name;
    search_data->type = type;
    search_data->s = s;
    search_data->l = l;

    create_threads(search_data);

    free(cond);
    free(lock);
    free(search_data);
    stack_free(s);

    print_list(l);

    list_Free(l);

    return 0;
}

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
void create_threads(void *v) {
    args_t *search_data = (args_t *) v;
    lock = malloc(sizeof(pthread_mutex_t));
    if (lock == NULL) {
        perror("malloc");
        exit(1);
    }
    cond = malloc(sizeof(pthread_cond_t));
    if (cond == NULL) {
        perror("malloc");
        exit(1);
    }


    if (pthread_mutex_init(lock, NULL) != 0){
        fprintf(stderr, "pthread_mutex_init: An error has occurred.\n");
        exit(1);
    }
    if (pthread_cond_init(cond, NULL) != 0){
        fprintf(stderr, "pthread_cond_init: An error has occurred.\n");
        exit(1);
    }

    pthread_t pt[nrthr];
    for (size_t i = 0; i < nrthr; i++) {
        if (pthread_create(&pt[i], NULL, search, search_data) != 0) {
            fprintf(stderr, "pthread_create: An error has occurred.\n");
            exit(1);
        }
    }

    search(search_data);

    for (size_t i = 0; i < nrthr; i++) {
        if (pthread_join(pt[i], NULL) != 0) {
            fprintf(stderr, "pthread_join: An error has occurred.\n");
            exit(1);
        }
    }

    if (pthread_cond_destroy(cond) != 0) {
        fprintf(stderr, "pthread_cond_destroy: An error has occurred.\n");
        exit(1);
    }
    if (pthread_mutex_destroy(lock) != 0) {
        fprintf(stderr, "pthread_mutex_destroy: An error has occurred.\n");
        exit(1);
    }
}
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
void *search(void *v) {

    args_t *search_data = (args_t *) v;
    stack *s = search_data->s;

    int read_dirs = 0;
    DIR *directory;

    while (true) {

        if (pthread_mutex_lock(lock) != 0){
            fprintf(stderr, "pthread_mutex_lock: An error has occurred.\n");
            exit(1);
        }

        while(stack_isEmpty(s)) {
            if (do_while_empty_stack(read_dirs) == -1) {
                return NULL;
            }
        }

        char dir[4096];
        stack_pop(s, dir);
        if (pthread_mutex_unlock(lock) != 0){
            fprintf(stderr, "pthread_mutex_unlock: An error has occurred.\n");
            exit(1);
        }

        if ((directory = opendir(dir)) == NULL) {
            perror(dir);
        } else {

            struct dirent *d;
            while ((d = readdir(directory)) != NULL) {
                read_directory(d->d_name, dir, search_data);
            }
            closedir(directory);
            read_dirs++;
        }
    }
}

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
int do_while_empty_stack(int read_dirs) {
    if (all_threads_finished) {
        if (pthread_mutex_unlock(lock) != 0) {
            fprintf(stderr, "pthread_mutex_unlock: An error has occurred.\n");
            exit(1);
        }
    } else if (waiting == (int)nrthr) {
        all_threads_finished = true;

        if (pthread_cond_broadcast(cond) != 0) {
            fprintf(stderr, "pthread_cond_broadcast: An error has occurred.\n");
            exit(1);
        }
        if (pthread_mutex_unlock(lock) != 0) {
            fprintf(stderr, "pthread_mutex_unlock: An error has occurred.\n");
            exit(1);
        }
    } else {
        waiting++;
        if (pthread_cond_wait(cond, lock) != 0) {
            fprintf(stderr, "pthread_cond_wait: An error has occurred.\n");
            exit(1);
        }
        waiting--;
        return 0;
    }
    pthread_mutex_lock(lock);
    printf("Thread: %zu Reads: %d\n", pthread_self(), read_dirs);
    pthread_mutex_unlock(lock);
    return -1;
}

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
void read_directory(char *current_name, char *current_dir, void *v) {
    args_t *search_data = (args_t *) v;
    stack *s = search_data->s;
    list *l = search_data->l;
    char type = search_data->type;
    char *name = search_data->name;

    bool is_valid_name = strcmp(current_name, ".") != 0 && strcmp(current_name, "..") != 0;

    if (is_valid_name) {

        size_t str_size = sizeof(char) * (strlen(current_dir) + strlen(current_name) + 2);
        char *str = malloc(str_size);
        if (str == NULL) {
            perror("malloc");
            exit(1);
        }

        strcpy(str, current_dir);
        if (strlen(str) != 1){
            strcat(str, "/");
        }
        strcat(str, current_name);

        struct stat st;
        if (lstat(str, &st) == -1) {
            perror(current_name);
            free(str);
            return;
        }
        if (S_ISDIR(st.st_mode)) {
            add_dir(str_size, str, s);
        }

        if (check_if_correct_file(type, name, current_name, st.st_mode)) {
            list_Insert(l, str);
        } else {
            free(str);
        }
    }
}

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
void add_dir(size_t str_size, char *str, stack *s) {

    char *stack_str = malloc(str_size);
    if (stack_str == NULL) {
        perror("malloc");
        exit(1);
    }

    strcpy(stack_str, str);
    pthread_mutex_lock(lock);
    stack_push(s, stack_str);


    if (pthread_cond_signal(cond) != 0) {
        fprintf(stderr, "pthread_cond_signal: An error has occurred.\n");
        exit(1);
    }
    pthread_mutex_unlock(lock);
}

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
bool check_if_correct_file(char type, char *name, char *current_name, __mode_t mt) {

    if (strcmp(name, current_name) == 0) {

        switch (type) {
            case '*':
                return true;
            case 'd':
                return S_ISDIR(mt);
            case 'f':
                return S_ISREG(mt);
            case 'l':
                return S_ISLNK(mt);
            default:
                fprintf(stderr, "check_if_correct_file: An unexpected type has been found.\n");
                exit(1);
        }
    }
    return false;
}


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
void get_options(char *type, int argc, char *argv[]) {

    int val;
    while ((val = getopt(argc, argv, "t:p:")) != -1) {


        switch (val) {
            case 't':
                if (strcmp("d", optarg) == 0) {
                    *type = 'd';
                } else if (strcmp("f", optarg) == 0) {
                    *type = 'f';
                } else if (strcmp("l", optarg) == 0) {
                    *type = 'l';
                } else {
                    fprintf(stderr, "Invalid type (-t) parameter\n");
                    exit(1);
                }
                break;
            case 'p':
                nrthr = (int) strtoul(optarg, 0, 10);

                if (errno != 0) {
                    fprintf(stderr, "Invalid type (-p) parameters\n");
                    exit(1);
                }

                if (nrthr < 1) {
                    fprintf(stderr, "Invalid amount of threads. Must be positive\n");
                    exit(1);
                }
                nrthr--;


                break;
            default:
                fprintf(stderr, "Not a valid argument (%s)\n", optarg);
                exit(1);
        }
    }
}

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
void set_folders(int argc, char *argv[], list *dirs) {

    bool empty = true;

    struct stat st;
    for (int i = optind; i < argc - 1; i++) {


        if (lstat(argv[i], &st) != 0) {
            fprintf(stderr, "%s: No such file or directory\n", argv[i]);
        } else {
            char *folders = malloc(sizeof(char) * (strlen(argv[i]) + 1));
            if (folders == NULL) {
                perror("malloc");
                exit(1);
            }
            strcpy(folders, argv[i]);

            size_t len = strlen(folders) - 1;
            if (folders[len] == '/' && len != 0) {
                memmove(&folders[len], &folders[len + 1], strlen(folders) - len);
            }
            list_Insert(dirs, folders);
            empty = false;
        }
    }
    if (empty) {
        fprintf(stderr, "mfind: No startfolder has been entered\n");
        exit(1);
    }
    remove_sub_dirs(dirs);
}

/*
 * print_list
 * ----------------------------------------------------------------------------
 * Description: Prints the given list
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be printed
 * ----------------------------------------------------------------------------
 * Output:  The list is printed to stdout
 */
void print_list(list *l) {
    element *e = l->first->next;
    while (e != l->last) {
        printf("%s\n", (char *) e->data);
        e = e->next;
    }
}

/*
 * remove_sub_dirs
 * ----------------------------------------------------------------------------
 * Description: removes all duplicates and sub-directories in a given list
 * ----------------------------------------------------------------------------
 * Input:   l: the list that should be checked
 * ----------------------------------------------------------------------------
 * Output: all duplicates and sub-dirs has been removed.
 */
void remove_sub_dirs(list *l) {
    for (element *e = l->first->next; e != l->last; e = e->next) {
        for (element *e1 = l->first->next; e1 != l->last; e1 = e1->next) {
            if (e != e1 && strstr(e->data, e1->data) != NULL) {
                element *prev = e1->prev;
                list_Remove(l->m, e1);
                e1 = prev;
            }
        }
    }
}