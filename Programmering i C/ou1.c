/*
 * Shopping assistant (ou1.c)
 * Author: Jonas Sjödin
 * Date: 2016-10-11
 * Cs: id16jsn
 * CAS: josj0105
 */


/*
 * This program is a Currency exchanger.
 * It takes input of a foreign currency from the user.
 * The user can then sum prices and change the sum to the foreign currency.
*/


#include <stdio.h>

int main(){
	int start = 1; 				/* If start = 1 program runs, else it quits*/
	int choice = 0; 			/* Different choices*/
	double exchangeRate = 1;	/* User inputted exchange rate*/
	double price = 0;			/* User entered price of purchase*/
	double sum = 0;				/* Sum of all prices of purchases*/
	
	/* Header*/
	printf("Your shopping assistant\n");

	/* Checks which choice the user chooses*/
	/* Loops until user wants to quit*/
	do{
		printf("\n1. Set exchange rate in SEK (current rate: %.2lf)\n", exchangeRate);
		printf("2. Read prices in the foreign currency\n");
		printf("3. End\n\n");
		printf("Give your choice (1 - 3): ");
		scanf("%d", &choice);
		printf("\n");
		

		switch (choice){
			
			/* User sets exchange rate*/
			case 1 : 
				printf("Give exchange rate: ");
				scanf("%lf", &exchangeRate);
			break;
			
			/* User enters prices which are added together*/
			/* Prices are shown in both standard and foreign currency*/
			case 2 : 
				while (price >= 0){
					sum = sum + price;
					printf("Give price (finish with < 0): ");
					scanf("%lf", &price);
				}
				printf("\nSum in foreign currency: %.2lf", sum);
				printf("\nSum in SEK: %.2lf\n", (sum * exchangeRate));
				price = 0;
				sum = 0;
			break;
			
			case 3 :
			
			/* User quits program*/
				start = 0;
			break;
			
			/* If user enters wrong input nr then print*/
			default :
				printf("Not a valid choice!\n");
		}
	} while (start == 1);
	
	printf("End of program!\n\n");
	
	return 0;
}