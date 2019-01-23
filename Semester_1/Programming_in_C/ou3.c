/*
 * Programmeringsteknik med C och Matlab
 * Fall 15
 * Assignment 3

 * File:         ou3.c
 * Description:  A simple implementation of Conway's Game of Life.
 * Author:		 Jonas Sjödin
 * CS username:  id16jsn
 * Date:		 2016-10-19
 * Input:        Choice of initial configuration and then instruction to step
 *               or exit.
 * Output:       Prints the game field in each step.
 * Limitations:  No validation of input.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Constants, representation of states */
#define ALIVE 'X'
#define DEAD '.'

/* Declaration of data structure */
typedef struct{
  char current;
  char next;
} cell;

/* Declaration of functions */
void initField(const int rows, const int cols, cell field[rows][cols]);
void loadGlider(const int rows, const int cols, cell field[rows][cols]);
void loadSemaphore(const int rows, const int cols, cell field[rows][cols]);
void loadRandom(const int rows, const int cols, cell field[rows][cols]);
void loadCustom(const int rows, const int cols, cell field[rows][cols]);
void printField(const int rows, const int cols, cell field[rows][cols]);
void updateField(const int rows, const int cols, cell field[rows][cols]);
void checkNeighbours(const int rows, const int cols, cell field[rows][cols]);


/* Function:    main
 * Description: Start and run games, interact with the user.
 * Input:       About what initial structure and whether to step or exit.
 * Output:      Information to the user, and the game field in each step.
 */

int main(void) {
	/* Declaration of variables*/
	const int ROWS = 20;
	const int COLS = 20;
	char input[2];
	int active = 1;
	cell field[ROWS][COLS];
	
	/*Gets user input of which spec to load and initializes it*/
	initField(ROWS, COLS, field);	
	
	/*Runs program if active != 0 else it quits*/
	while(active != 0) {
		printField(ROWS, COLS, field);
		printf("Select one of the following options:\n");
		printf("(enter) Step\n");
		printf("(any) Exit\n");
				
		fgets(input,sizeof(input),stdin);

		if (input[0] != '\n') {
			active = 0;
		}
		if (active != 0) {
			updateField(ROWS, COLS, field);
		}
	}


    return 0;
}


/* Function:    initField
 * Description: Initialize all the cells to dead, then asks the user about
 *              which structure to load, and finally load the structure.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void initField(const int rows, const int cols, cell field[rows][cols]) {

    for (int r = 0 ; r < rows ; r++) {
        for (int c = 0 ; c < cols ; c++) {
            field[r][c].current = DEAD;
		}
	}

    printf("Select field spec to load ([G]lider, [S]emaphore, [R]andom ");
    printf("or [C]ustom): ");

    int ch = getchar();
    switch (ch) {
        case 'g':
        case 'G':
            loadGlider(rows, cols, field);
            break;
        case 's':
        case 'S':
            loadSemaphore(rows, cols, field);
            break;
        case 'r':
        case 'R':
            loadRandom(rows, cols, field);
            break;
        case 'c':
        case 'C':
        default:
            loadCustom(rows, cols, field);
            break;
    }

    /* Ignore following newline */
    if (ch != '\n') {
        getchar();
    }
}


/* Function:    loadGlider
 * Description: Inserts a glider into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadGlider(const int rows, const int cols, cell field[rows][cols]) {

    field[0][1].current = ALIVE;
    field[1][2].current = ALIVE;
    field[2][0].current = ALIVE;
    field[2][1].current = ALIVE;
    field[2][2].current = ALIVE;
}


/* Function:    loadSemaphore
 * Description: Inserts a semaphore into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadSemaphore(const int rows, const int cols, cell field[rows][cols]) {

    field[8][1].current = ALIVE;
    field[8][2].current = ALIVE;
    field[8][3].current = ALIVE;
}


/* Function:    loadRandom
 * Description: Inserts a random structure into the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated. There is a 50 % chance that a cell
 *              is alive.
 */

void loadRandom(const int rows, const int cols, cell field[rows][cols]) {
	srand(time(NULL));
	int randomNr = rand()%2 + 0;
	int xCounter = 0;
	int pointCounter = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++){
			randomNr = rand()%2 + 0;
			if (randomNr == 1) {
				field[r][c].current = ALIVE;
				xCounter = xCounter + 1;
			}
			if (randomNr == 0) {
				field[r][c].current = DEAD;
				pointCounter = pointCounter + 1;
			}
		}

	}
	printf("\nX: %d\n", xCounter);
	printf("\n.: %d\n", pointCounter);
}


/* Function:    loadCustom
 * Description: Lets the user specify a structure that then is inserted into
 *              the field.
 * Input:       The field array and its size.
 * Output:      The field array is updated.
 */

void loadCustom(const int rows, const int cols, cell field[rows][cols]) {

    printf("Give custom format string: ");
    do {
        int r, c;
        scanf("%d,%d", &r, &c);
        field[r][c].current = ALIVE;
    } while (getchar() != '\n');
}


/* Function: 	printField
 * Description:	Prints the field of the game
 * Input:		The field array and its size
 * Output:		The field array is printed
 */

void printField(const int rows, const int cols, cell field[rows][cols]) {

	for (int r = 0; r < rows; r++){
		for (int c = 0; c < cols; c++){
			printf("%2c", field[r][c].current);
		}
		printf("\n");
	}
}


/* Function: 	updateField
 * Description:	Updates the field according to the games rules
 * Input:		The field array and its size
 * Output:		The field array is updated with new [X]:s and [.]:s if 
 * 				they should be.
 */

void updateField(const int rows, const int cols, cell field[rows][cols]){
	for (int r = 0 ; r < rows ; r++) {
        for (int c = 0 ; c < cols ; c++) {
            field[r][c].next = DEAD;
		}
	}
	
	checkNeighbours(rows, cols, field);
	
	
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			field[r][c].current = field[r][c].next;
		}
	}
}

 
/* Function: 	checkNeighbours
 * Description:	Checks the neighbours of each tile in the field array and 
 * 				sets it to alive or dead next round
 * Input:		The field array and its size
 * Output:		The field array is updated with new values 
 */

void checkNeighbours(const int rows, const int cols, cell field[rows][cols]){
	int nrOfNeighbours = 0;
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {

			/* Check top fields*/
			if (r > 0) {
				/*Top left field*/
				if (c > 0) {
					if (field[r-1][c-1].current == ALIVE) {
						nrOfNeighbours++;
					}
				}
				/*Top field*/
				if (field[r-1][c].current == ALIVE) {
					nrOfNeighbours++;
				}
				/*Top right field*/
				if (c < (cols - 1)) {
					if (field[r-1][c+1].current == ALIVE) {
						nrOfNeighbours++;
					}
				}
			}

			/* Check left field*/
			if (c > 0) {
				if (field[r][c-1].current == ALIVE) {
					nrOfNeighbours++;
				}
			}
			/*Check right field*/
			if (c < (cols - 1)) {
				if (field[r][c+1].current == ALIVE) {
					nrOfNeighbours++;
				}
			}

			/* Check bottom fields*/
			if (r - (rows - 1)) {
				/*Bottom left field*/
				if (c > 0) {
					if (field[r+1][c-1].current == ALIVE) {
						nrOfNeighbours++;
					}
				}
				/*Bottom field*/
				if (field[r+1][c].current == ALIVE) {
					nrOfNeighbours++;
				}
				/*Bottom right field*/
				if (c < cols - 1) {
					if (field[r+1][c+1].current == ALIVE) {
						nrOfNeighbours++;
					}	
				}
			}
			
			/* Check if current field should be dead or alive next turn.*/
			if (field[r][c].current == ALIVE) {
				if (nrOfNeighbours == 0 || nrOfNeighbours == 1) {
					field[r][c].next = DEAD;
				}
				if (nrOfNeighbours == 2 || nrOfNeighbours == 3) {
					field[r][c].next = ALIVE;
				}
				if (nrOfNeighbours >= 4) {
					field[r][c].next = DEAD;
				}
			}
			
			if (field[r][c].current == DEAD && nrOfNeighbours == 3) {
				field[r][c].next = ALIVE;
			}

			nrOfNeighbours=0;
		}
	}
}