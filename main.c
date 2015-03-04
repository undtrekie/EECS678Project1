/*
*	@filename: main.c
*	@authors: Tyler Steiner and Michael Esry
*	@date: 2015.02.23
*	Purpose: Program Main for Quite a Shell (quash) Project.
*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LENGTH 80

int main(int argc, char** argv){
    char input[MAX_LENGTH];
    char argus[MAX_LENGTH];
    char* home=argv[1];
    char wDirectory[MAX_LENGTH];
    char* path=argv[2];
    char ex[]="exit";
    char quit[]="quit";
    char help[]="help";
    char set[]="set";
    char cd[]="cd";
    char jobs[]="jobs";
    int num=0;
    //Set Working Directory

    if(argc!=3){
		printf("\n%i: Incorrect # of parameters\n\n",argc);
		printf("usage: ./project <home directory> <path>\n\n");
		return 1;
    }
    strcpy(wDirectory, home);
    

    while(strcmp(input, ex)!=0 && strcmp(input, quit)!=0){
		printf("quash:%s > ", wDirectory);
		//fgets(input, MAX_LENGTH, stdin);
		//strtok(input, "\n");
      	scanf("%s",input);

	if(strcmp(input, help)==0){
	    printf("commands:\n");
	    printf("\thelp\n");
	    printf("\texit\n");
	    printf("\tquit\n");
	    printf("\tset\n");
	    printf("\tcd\n");
	    printf("\tjobs\n");
	    printf("\tls\n");
	    printf("\trm\n");
	    printf("\t./ - for executing programs can be run with or without arguments\n");
	}else if(strncmp(input, set, 3)==0){
	    
	}else if(strncmp(input, cd, 2)==0){
	    char buff[MAX_LENGTH];
	    fgets(buff, 80, stdin);
	    if(strlen(buff)>1){
		strtok(buff, "\n");
		int i;
		for(i=0; i<strlen(buff)-1; i++){
		    wDirectory[i]=buff[i+1];
		}
			wDirectory[i]='\0';
	    }else{
			strcpy(wDirectory, home);
	    }
	    system(input);
	}else if(strcmp(input, jobs)==0){
	
	}else if(strcmp(input, quit)==0){
		printf("Good-Bye.\n");
	}else if(strcmp(input, ex)==0){	
		printf("Good-Bye.\n");
	}else if(strncmp(input, "./", 1)==0){
		char arg[MAX_LENGTH];
		fgets(arg,80,stdin);
		printf("arg: %s",arg);
		char temp[MAX_LENGTH];
		strcpy(temp,home);
		strcat(temp,input);
		printf("command: %s \n",temp);
		system(temp);
	}else if(strcmp(input, "ls")==0){
		strcat(input," ");
		strcat(input,home);
		system(input);	
	}else if(strncmp(input, "rm", 2)==0){
		strcat(input, " ");
		strcat(input,home);
		char file[MAX_LENGTH];
		printf("Enter Filename: ");
		scanf("%s",file);
		strcat(input,file);
		system(input);
	}else{
	    printf("%s: command not found.\n", input);
	}
	}
    return 0;
}
