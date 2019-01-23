#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void){

    int die, lastdie, count, unfinished, throws; 
    char *dienames[] = {"ettor", "tvåor", "treor", "fyror", "femmor", "sexor"}; 
     
    srand(time(NULL)); 
    
    printf("Detta program kastar tärning tills vi fått tre lika i rad\n"); 
    count = 0; 
    unfinished = 1; 
    lastdie = 0; 
    throws = 0; 

    while (unfinished){

        /* Simulera ett kast */
        throws++; 
        die = rand()%6 + 1; 
        printf("Kast %d gav %d prickar\n", throws, die);     
    
        /* Jämför med senaste */
        if (die == lastdie)
            count++; 
        else 
            count = 0;
		
        lastdie = die; 
		
		if (count == 2){		//byt ut unfinished = count == 2; till en if
			unfinished = 0;
		}
    }

    printf("Vi fick tre stycken %s efter %d kast\n", dienames[die-1], throws); 
    return 1; 
}