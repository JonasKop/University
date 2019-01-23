/*
 * mish.c
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


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <wait.h>
#include <fcntl.h>

#include "mish.h"
#include "execute.h"
#include "sighant.h"


int main(void) {
    nrOfPids = 0;

    while (1) {
        signal(SIGINT, SIG_IGN);

        char line[MAXLINELEN];
        prompt_getline(line);
        if (strlen(line) > 1) {
            if (strncmp(line, "exit", 4)  == 0) {
                break;
            }

            command cmd[MAXCOMMANDS];
            int nrOfCommands = parse(line, cmd);

            if (isInternalCommand(cmd) == -1) {
                int nrOfPipes = nrOfCommands - 1;
                int fd[nrOfPipes + 1][2];
                int stdin_cp;

                if ((stdin_cp  = dup(STDIN_FILENO)) == -1) {
                    perror("dup_STDIN");
                    exit(EXIT_FAILURE);
                }

                for (int i = 0; i < nrOfPipes; i++) {
                    if (pipe(fd[i]) == -1) {
                        perror("pipe_fd");
                        exit(EXIT_FAILURE);
                    }
                }
                signal(SIGINT, SIG_DFL);
                for (int i = 0; i < nrOfCommands; i++) {
                    if ((pid[i] = fork()) == -1){
                        perror("fork");
                        exit(EXIT_FAILURE);
                    } else if (pid[i] == 0) {
                        close(stdin_cp);
                        for (int j = nrOfPipes - 1; j > i; j--) {
                            close(fd[j][0]);
                            close(fd[j][1]);
                        }
                        child_action(i,cmd[i],nrOfCommands,fd[i]);
                    }
                    nrOfPids++;

                    signal(SIGINT, sig_handle);

                    if (i != nrOfCommands - 1 && dupPipe(fd[i], 0, 0) == -1) {
                        exit(EXIT_FAILURE);
                    }
                }


                for (int i = 0; i < nrOfCommands; i++) {

                    int status;
                    if (wait(&status) != pid[i]) {
                        perror("wait");
                        exit(EXIT_FAILURE);
                    }
                    pid[i] = 0;
                }
                signal(SIGINT, SIG_IGN);
                restore_stdin(stdin_cp);
                nrOfPids = 0;
            }
        }
    }
	return 0;
}

/*
 * prompt_getline
 * ---------------------------------------------------------------------------
 * Description: This function prompts 'mish% ' and gets a line of userinput.
 * -----------------------------------------------------------------------------
 * Input:       line: pointer to the line which should be set.
 * -----------------------------------------------------------------------------
 * Output:      a changed line, exits on failure.
 */
void prompt_getline(char *line) {
    if (fprintf(stderr, "mish%% ") < 0) {
        fprintf(stderr, "ERROR fprintf\n");
        exit(EXIT_FAILURE);
    }
    if (fflush(stderr) == EOF) {
        fprintf(stderr, "ERROR fflush\n");
        exit(EXIT_FAILURE);
    }
    if (fgets(line, MAXLINELEN, stdin) == NULL) {
        fprintf(stderr, "ERROR fgets\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * isInternalCommand
 * -----------------
 * Description: Checks if a given command is an internal command i.e. echo or cd
 * -----------------------------------------------------------------------------
 * Input:       cmd: The given command
 * -----------------------------------------------------------------------------
 * Output:      Return -1 if it isnt an internal command, else 0
 */
int isInternalCommand(command *cmd) {

    if (strncmp(cmd[0].argv[0], "cd", 2) == 0) {
        if (cmd[0].argc != 2) {
            fprintf(stderr, "Invalid nr of arguments\n");
        } else {
            cd(cmd[0].argv[1]);
        }
        return 0;
    } else if (strncmp(cmd[0].argv[0], "echo", 4) == 0) {
        if (cmd[0].argc < 2) {
            fprintf(stderr, "Invalid nr of arguments\n");
        } else {
            echo(cmd[0].argv + 1, cmd[0].argc - 1);
        }
        return 0;
    }
    return -1;
}

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
int cd(char *dir_command) {
    struct stat path_stat;
    if (stat(dir_command, &path_stat) < 0) {
        fprintf(stderr, "cd: %s: No such file or directory\n", dir_command);
        return -1;
    }
    if (chdir(dir_command) < 0)  {
        perror("chdir");
        return -1;
    }

    return 0;
}

/*
 * echo
 * -----------------------------------------------------------------------------
 * Description: Prints the given char** to stdout.
 * -----------------------------------------------------------------------------
 * Input:   c: the chararray that should be printed.
 * -----------------------------------------------------------------------------
 * Output:  Returns 0 if succeeded, else -1.
 */
int echo(char **c, int size) {

    for (int i = 0; i < size; i++) {

        printf("%s", c[i]);
        if (i != size - 1) {
            printf(" ");
        }
    }
    printf("\n");
    return 0;
}

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
void child_action(int i, command cmd, int nrOfCommands, int fd[]) {

    if (i == 0 && cmd.infile != NULL) {
        if (redirect(cmd.infile, 0, 0) == -1) {
            kill(getppid(), SIGINT);
            exit(EXIT_FAILURE);
        }
    }

    if (i + 1 < nrOfCommands) {
        if (dupPipe(fd, 1, 1) == -1) {
            kill(getppid(), SIGINT);
            exit(EXIT_FAILURE);
        }
    } else if (cmd.outfile != NULL){
        if (redirect(cmd.outfile, 1, 1) == -1) {
            kill(getppid(), SIGINT );
            exit(EXIT_FAILURE);
        }
    }

    if (exec(cmd.argc, cmd.argv) == -1) {
        kill(getppid(), SIGINT);
        exit(EXIT_FAILURE);
    }
}

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
int exec(int nrOfWords, char **words) {

    char *cmd_argv[1024];

    for (int i = 0; i < nrOfWords; i++) {
        char *word = words[i];
        cmd_argv[i] = word;
    }

    cmd_argv[nrOfWords] = NULL;

    if (execvp(cmd_argv[0], (char**)cmd_argv) == -1) {
        perror("exec");
        return -1;
    }

    return 0;
}

/*
 * restore_stdin
 * ---------------------------------------------------------------------------
 * Description: Restore stdin from a given file descriptor
 * -----------------------------------------------------------------------------
 * Input:       stdin_cp: The stdin copy.
 * -----------------------------------------------------------------------------
 * Output:      stdin is back at 0, if not shell exits.
 */
void restore_stdin(int stdin_cp) {
    if (close(0) == -1) {
        perror("close_0");
        exit(EXIT_FAILURE);
    }
    if (dup2(stdin_cp, 0) == -1) {
        perror("dup_stdin");
        exit(EXIT_FAILURE);
    }
    if (close(stdin_cp) == -1) {
        perror("close_incopy");
        exit(EXIT_FAILURE);
    }
}
