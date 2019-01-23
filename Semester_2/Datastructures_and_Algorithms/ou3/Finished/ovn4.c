#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
/*
Syftet med programmet är att tills användaren matat in texten quit
kryptera de rader som användaren anger mha ett caesar-krypto
*/

char *caesar(char *inString,int n) {
	int i=0;
	char *outString=(char *) malloc(strlen(inString)+1);
	while(inString[i]!='\0') {
		outString[i]=inString[i]+n;
		i++;
	}
	outString[i]='\0';
	return outString;
}

int main(void) {
    char *str=malloc(255);
    char *outString=NULL;
    bool running=true;
    printf("Mata in text att kryptera avsluta med att skriva quit på en egen rad\n");
    while(running) {
       fgets(str,255,stdin);
       outString=caesar(str,1);
	   free(outString); //fria efter användning då den inte används mer.
       if(strncmp(str,"quit",4)==0) {
		   free(str); //flytta upp hit
           exit(0);
       }
       outString=caesar(str,1);
       printf("%s\n", outString);
	   free(outString); //fria efter printen då den inte behövs mer nu.
    }

    return 0;
}