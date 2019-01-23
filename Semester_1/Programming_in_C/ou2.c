/* Score calculator (ou2.c)
 * Author: Jonas Sjödin
 * Date: 2016-10-11
 * Cs: id16jsn
 * CAS: josj0105 */

/* This program is a score calculator. 
 * First it takes input from the user of how many judges there is.
 * Then it takes input from the user of each judges score.
 * Then it calculates the highest, lowest and average score.*/
 
#include <stdio.h>

/* Declaration of functions
 * Functioninfo is located just above the function*/
void info(void);
int getNumberOfJudges(void);
void getScore(double arr[], int arrSize);
void printScore(double arr[], int arrSize);
void calcs(double *min, double *max, double *avg, int arrSize, double arr[]);
void printCalcs(double highestScore, double lowestScore, double averageScore);


int main(void){
	int numberOfJudges;
	double highestScore = 0;
	double lowestScore = 0;
	double averageScore = 0;
	
	/* Prints program info*/
	info();
	
	/* Fills numberOfJudges with user input*/
	numberOfJudges = getNumberOfJudges();

	/* Fills array with score from user input*/
	double judge[numberOfJudges];
	getScore(judge, numberOfJudges);
	
	/* Prints score from the judge array*/
	printScore(judge, numberOfJudges);
	
	/* Calculates highest, lowest and average score*/
	calcs(&lowestScore, &highestScore, &averageScore, numberOfJudges, judge);

	/* Prints highest, lowest and average score*/
	printCalcs(highestScore, lowestScore, averageScore);

	return 0;
}

/* Input: NONE
 * Output: Prints program information*/
void info(void){
	printf("Program information\n");
	printf("The program reads in the number of judges and the score from each judge.\n");
	printf("Then it calculates the average score without regard to the lowest and\n");
	printf("highest judge score. Finally it prints the results (the highest, the\n");
	printf("lowest and the final average score).\n\n");
}

/* Input: User fills in the number of judges which is the arraysize
 * Output: The size of an array*/
int getNumberOfJudges(void){
	int arrSize = 0;
	
	/*Runs until an appropriate size of the array is inputted by the user*/
	while (arrSize < 3 || arrSize > 10){
		printf("Number of judges (min 3 and max 10 judges)? ");
		scanf("%d", &arrSize);
	}
	return arrSize;
}

/* Input: An array and its size. The arrays value is filled by the user
 * Output: An array that has been filled with the judges scores*/
void getScore(double arr[], int arrSize){
	printf("\n");
	
	/*Fills array with each judge score from the user*/
	for (int i = 0; i < (arrSize); i++){
		printf("Score from judge %d? ", (i + 1));
		scanf("%lf", &arr[(i)]);
	}
}

/* Input: An Array and it's size
 * Output: Prints the judges scores*/
void printScore(double arr[], int arrSize){
	printf("\nLoaded scores:\n");
	
	/*Prints Score by looping through the array*/
	for (int i = 0; i < (arrSize); i++){
		printf("Judge %d: %.1lf\n", (i + 1), arr[i]);
	}
}

/* Input: Array, it's size, a pointer to store min value, max value
 * and avg value of the array
 * Output: Sets min, max and avg number of an array*/
void calcs(double *min, double *max, double *avg, int arrSize, double arr[]){
	double smallestNr = arr[0];
	double biggestNr = arr[0];
	double averageNr = 0;
	
	for (int i = 0; i < arrSize; i++){
		/*Checks for smallest array nr*/
		if (arr[i] < smallestNr){
			smallestNr = arr[i];
		}
		/*Checks for largest array nr*/
		else if (arr[i] > biggestNr){
			biggestNr = arr[i];
		}
	}
	
	/* Adds all nrs of the array and calculates average nr 
	 * except the extreme nrs*/
	for (int i = 0; i < arrSize; i++){
		averageNr = (averageNr + arr[i]);
	}
	averageNr = (averageNr - biggestNr - smallestNr) / (arrSize - 2);
	
	*avg = averageNr;
	*min = smallestNr;
	*max = biggestNr;
}

/* Input: Highest score, lowest score and the average score
 * Output: Prints out the highest, the lowest and the average score*/
void printCalcs(double highestScore, double lowestScore, double averageScore){
	printf("\nFinal result:\n");
	
	printf("Highest judge score: %.1lf\n", highestScore);
	printf("Lowest judge score: %.1lf\n", lowestScore);
	printf("Final average score: %.1lf\n", averageScore);
}





























