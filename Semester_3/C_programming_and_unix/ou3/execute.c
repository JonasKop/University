/*
 * execute.c
 * -----------------------------------------------------------------------------
 * This file contains a function which duplicates a pipe to a file descriptor
 * and closes both pipe ends. It also contains a function which redirect a file
 * descriptor to a file.
 * -----------------------------------------------------------------------------
 * Assignment 3
 * Course: Systemnära programmering HT17 5DV088, Umeå University
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */


#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "execute.h"


/* 
 * dupPipe
 * -----------------------------------------------------------------------------
 * Description: Duplicate a pipe to a standard I/O file descriptor, and close
 *              both pipe ends
 * -----------------------------------------------------------------------------
 * Input:	    pip: the pipe.
 *			    end: tells which end of the pipe which should be duplicated.
 *			         It can be one of READ_END or WRITE_END.
 *		    	destfd:	the standard I/O file descriptor to be replaced.
 * -----------------------------------------------------------------------------
 * Output:	    Returns -1 on error, else destfd.
 */
int dupPipe(int pip[2], int end, int destfd) {
    if (close(destfd) < 0) {
        perror("close_destfd");
        return -1;
    }

    if (end == READ_END || end == WRITE_END) {
        if ((destfd = dup2(pip[end], destfd)) < 0) {
            perror("dup");
            return -1;
        }
    } else {
        fprintf(stderr, "Invalid parameter 'end': %d\n", end);
        return -1;
    }

	if (close(pip[0]) < 0) {
		perror("close_pipeRead");
		return -1;
	}
	if (close(pip[1]) < 0) {
		perror("close_pipeWrite");
		return -1;
	}
	return destfd;
}

/* 
 * redirect
 * -----------------------------------------------------------------------------
 * Description: Redirect a standard I/O file descriptor to a file
 * -----------------------------------------------------------------------------
 * Input:   	filename: the file to/from which the standard I/O file
 *			    		  descriptor should be redirected
 *		    	flags: indicates whether the file should be opened for reading
 *			    	   or writing
 *	    		destfd: the standard I/O file descriptor which shall be
 *			    		redirected
 * -----------------------------------------------------------------------------
 * Output:	    Returns -1 on error, else destfd
 */
int redirect (char *filename, int flags, int destfd) {

    if (flags != STDIN_FILENO && flags != STDOUT_FILENO){
        fprintf(stderr,
                "redirect: flags must indicate 0 for read or 1 for write\n");
        return -1;
    }

    struct stat path_stat;
	if (stat(filename, &path_stat) == 0 && flags == STDOUT_FILENO) {
		fprintf(stderr, "%s: File already exists\n", filename);
		return -1;
	}

	int fd;

    if (flags == STDIN_FILENO) {
        if ((fd = open(filename, flags)) < 0) {
            perror("open_file_read");
            return -1;
        }
    } else {
        if ((fd = open(filename, flags | O_CREAT, 0666)) < 0) {
            perror("create_file_write");
            return -1;
        }
    }

	if (close(destfd) < 0) {
		perror("close");
		return -1;
	}
	if ((destfd = dup(fd)) < 0) {
		perror("dup");
		return -1;
	}
	if (close(fd) < 0) {
		perror("close");
		return -1;
	}

	return destfd;
}