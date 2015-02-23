/*
*	@filename: main.c
*	@authors: Tyler Steiner and Michael Esry
*	@date: 2015.02.23
*	Purpose: Program Main for Quite a Shell (quash) Project.
*/
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 80

int main(){
    char input[MAX_LENGTH];
    char exit[]="exit";
    char quit[]="quit";
    char help[]="help";
    char set[]="set";
    char cd[]="cd";
    char jobs[]="jobs";
    while(strcmp(input, exit)!=0 && strcmp(input, quit)!=0){
	printf("quash# > ");
	scanf("%s", input);

	if(strcmp(input, help)==0){
	    printf("commands:\n");
	    printf("\thelp\n");
	    printf("\texit\n");
	    printf("\tquit\n");
	    printf("\tset\n");
	    printf("\tcd\n");
	    printf("\tjobs\n");
	}else if(strcmp(input, set)==0){
	    
	}else if(strcmp(input, cd)==0){

	}else if(strcmp(input, jobs)==0){

	}else{
	    printf("%s: command not found.\n", input);
	}
    }
    return 0;
}
