#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 80

int main(){
    char input[MAX_LENGTH];
    char exit[]="exit";
    char help[]="help";
    while(strcmp(input, exit)!=0){
	printf("quash# > ");
	scanf("%s", input);

	if(strcmp(input, help)==0){
	    printf("commands:\n");
	    printf("\thelp\n");
	    printf("\texit\n");
	}
    }
    return 0;
}
