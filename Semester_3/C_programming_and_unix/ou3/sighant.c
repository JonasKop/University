/*
 * sighant.c
 * -----------------------------------------------------------------------------
 * The file that handles signals for the mish project. It kills all active
 * child-processes located in the pid[] array.
 * -----------------------------------------------------------------------------
 * Assignment 3
 * Course: Systemnära programmering HT17 5DV088, Umeå University
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */


#include <signal.h>
#include "sighant.h"
#include "mish.h"


/*
 * sig_handle
 * -----------------------------------------------------------------------------
 * Description: Handles the SIGINT signal for child processes. If it is received
 *              it kills all child processes.
 * -----------------------------------------------------------------------------
 * Input:       sig: The signal
 * -----------------------------------------------------------------------------
 * Output:      Kills all active child processes.
 */
void sig_handle(int sig) {

    if (sig == SIGINT) {
        for (int i = nrOfPids - 1; i >= 0; i--) {
            kill(pid[i], SIGINT);
        }
    }
}