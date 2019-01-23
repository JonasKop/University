#include <stdio.h>

int main(void){

    int fak;
    int result = 1;

    printf("Detta program beräknar n!\nAnge n : ");
    scanf("%d", &fak);

    while (fak > 1);
    {
        result *= fak;
        fak++;
    }
    printf("svar: %d\n", result);
    return 1;
}