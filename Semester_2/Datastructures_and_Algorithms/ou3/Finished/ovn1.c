#include <stdio.h>

int main(void){

    int fak;
    int result = 1;

    printf("Detta program beräknar n!\nAnge n : ");
    scanf("%d", &fak);

    while (fak > 0)			//Ändrade "fak > 1" till "fak > 0"
    {						//Tog bort ";"
        result *= fak;
        fak--;				//Ändrade "fak++;" till "fak--;"
    }
    printf("svar: %d\n", result);
    return 0;

}