/*
 * mish.h
 * -----------------------------------------------------------------------------
 * The main file that runs the mish shell. This is the third assignment of the
 * cource Systemnära programmering att Umeå University. It gets input commands from
 * stdin and executes them. It can handle pipes '|', redirecting output '>' and
 * redirecting input '<'.
 * -----------------------------------------------------------------------------
 * Assignment 3
 * Course: Systemnära programmering HT17 5DV088, Umeå University
 * Author: Jonas Sjödin
 * CS-user: id16jsn
 * Email: jonas.k.sjodin@gmail.com
 * Date: October 2017
 */


#ifndef _MISH_
#define _MISH_


#include "parser.h"


pid_t pid[MAXCOMMANDS];
int nrOfPids;


/*
 * prompt_getline
 * ---------------------------------------------------------------------------
 * Description: This function prompts 'mish% ' and gets a line of userinput.
 * -----------------------------------------------------------------------------
 * Input:       line: pointer to the line which should be set.
 * -----------------------------------------------------------------------------
 * Output:      a changed line, exits on failure.
 */
void prompt_getline(char *line);

/*
 * isInternalCommand
 * -----------------
 * Description: Checks if a given command is an internal command i.e. echo or cd
 * -----------------------------------------------------------------------------
 * Input:       cmd: The given command
 * -----------------------------------------------------------------------------
 * Output:      Return -1 if it isnt an internal command, else 0
 */
int isInternalCommand(command *cmd);

/*
 * cd
 * -----------------------------------------------------------------------------
 * Description: Changes working directory.
 * -----------------------------------------------------------------------------
 * Input:   dir_command: The string with the directory which cd should change to
 *                       or ".." to go back one directory.
 * -----------------------------------------------------------------------------
 * Output:  Returns 0 if succeded, else -1;
 */
int cd(char *dir_command);

/*
 * echo
 * -----------------------------------------------------------------------------
 * Description: Prints the given char** to stdout.
 * -----------------------------------------------------------------------------
 * Input:   c: the chararray that should be printed.
 * -----------------------------------------------------------------------------
 * Output:  Returns 0 if succeeded, else -1.
 */
int echo(char **c, int size);

/*
 * child_action
 * ---------------------------------------------------------------------------
 * Description: Contains the actions which the child should execute when it
 *              executes a program. It checks if it should redirect input or
 *              output, duplicate output and executes a command.
 * -----------------------------------------------------------------------------
 * Input:       i: the current loop round
 *              cmd: a command which has been parsed by parser.c
 *              nrOfCommands: the nr of commands thats been parced by parser.c
 *              fd: The file descriptor which maybe should be redirected.
 * -----------------------------------------------------------------------------
 * Output:      executes a program, on error it terminates.
 */
void child_action(int i, command cmd, int nrOfCommands, int fd[]);

/*
 * exec
 * -----------------------------------------------------------------------------
 * Description: Executes a program
 * -----------------------------------------------------------------------------
 * Input:       nrOfWords: The nr of words in the command
 *              words: The char array with the command-words.
 * -----------------------------------------------------------------------------
 * Output:  Return -1 on error and 0 on success.
 */
int exec(int nrOfWords, char **words);

/*
 * restore_stdin
 * ---------------------------------------------------------------------------
 * Description: Restore stdin from a given file descriptor
 * -----------------------------------------------------------------------------
 * Input:       stdin_cp: The stdin copy.
 * -----------------------------------------------------------------------------
 * Output:      stdin is back at 0, if not shell exits.
 */
void restore_stdin(int stdin_cp);


#endif