#include <stdio.h>
#include <time.h>
#include <stdlib.h>

/*Programmet ska testa om man får samma slumptal om man anger samma seed till
  slumpgeneratorn och slumpar ett antal tal*/
int main(void){

    int i, j, seed, failed, tested; 
    int **mybuffer;
    
    printf("Skriv in en randseed (vilken som helst) : "); 
    scanf("%d",&seed); 
    srand(seed); 

    /* Allokera en massa pekare */
    mybuffer = calloc(100,sizeof(int *)); 
    if (mybuffer == NULL){
        fprintf(stderr, "Kunde inte allokera minne!"); 
        exit (-1); 
    }
    /* Allokera minne för pekarna */
    for (i=0 ; i<100 ; i++){
        mybuffer[i] = calloc(10,sizeof(int));    
        if (mybuffer[i] == NULL){
            fprintf(stderr, "Kunde inte allokera minne!"); 
            exit (-1); 
        }   
    }
    /* Fyll mitt allokerade minne med slumptal */
    for (i=0 ; i<100 ; i++){
        for (j=0 ; j<10 ; j++){
            mybuffer[i][j] = rand();    
        }
    }

    /* Nu kan vi prova att se om vi får samma slumptal */
    /* Genom att se samma randseed en gång till */
    srand(seed);         
    failed = 0; 
    tested = 0; 
    for (i=0 ; i<100 ; i++)
        for(j=0 ; j<10 ; j++)
            if (mybuffer[i][j] != rand())
                printf("Olika slumptal!!! (fel?)\n"); 
                tested++; 
                failed = 1; 
            
            
    if (failed)
        printf("Det här blev nog fel kanske.. \n"); 
    else
        printf("Kollade %d tal och de var iaf korrekta\n",tested);  

    /* Frigör minnet */
    for (i=0 ; i<100; i++){
        free(mybuffer[i]); 
    }
    free(mybuffer); 
    return 1; 
}