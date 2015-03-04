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
    char path[5][MAX_LENGTH];
    int numPaths=0;
    char ex[]="exit";
    char quit[]="quit";
    char help[]="help";
    char set[]="set";
    char cd[]="cd";
    char jobs[]="jobs";
    int num=0;
    int i;           //loop iterator;
    char* split;
    if(argc!=3){
		printf("\n%i: Incorrect # of parameters\n\n",argc);
		printf("usage: ./project <home directory> <path>\n\n");
		return 1;
    }

    //set Working Directory
    strcpy(wDirectory, home);
    //set Path
    if(strstr(argv[2],":")==NULL){
	strcpy(path[0], argv[2]);
	numPaths++;
    }else{
	split=strtok(argv[2], ":");
	while(split != NULL){
	    strcpy(path[numPaths],split);
	    numPaths++;
	    split=strtok(NULL, ":");
	}
    }
    numPaths++;
    

    while(strcmp(input, ex)!=0 && strcmp(input, quit)!=0){
		printf("quash:%s > ", wDirectory);
      	fgets(input,80,stdin);

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
	    char buff[MAX_LENGTH];
	    fgets(buff, 80, stdin);
	    if(strlen(buff)>1){
		char* setin;
		strtok(buff, "\n");
		setin=strtok(buff, " =");
		if(strcmp(setin,"HOME")==0){
		    setin=strtok(NULL,"\n");
		    strcpy(home, setin);
		}else if(strcmp(setin,"PATH")==0){
		    numPaths=0;
		    setin=strtok(NULL,":");
		    while(setin !=NULL){
			strcpy(path[numPaths],setin);
			numPaths++;
			setin=strtok(NULL, ":");
		    }
		}
	    }
	}else if(strncmp(input, cd, 2)==0){
	    char buff[MAX_LENGTH];
	    fgets(buff, 80, stdin);
	    if(strlen(buff)>1){
		strtok(buff, "\n");
		for(i=0; i<strlen(buff)-1; i++){
		    wDirectory[i]=buff[i+1];
		}
			wDirectory[i]='\0';
	    }else{
			strcpy(wDirectory, home);
	    }
	    system(input);
	}else if(strncmp(input,jobs,4)==0){
	
	}else if(strncmp(input,quit,4)==0){
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input,ex,4)==0){	
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input, "./", 1)==0){
		char temp[MAX_LENGTH];
		strtok(input, "\n");
		strcpy(temp,home);
		strcat(temp,input);
		printf("command: %s \n",temp);
		system(temp);
	}else if(strncmp(input, "ls", 2)==0){
		strtok(input, "\n");
		strcat(input, " ");
		strcat(input,home);
		system(input);	
	}else if(strncmp(input, "rm", 2)==0){
		char temp[MAX_LENGTH];
		char* rm;
		strtok(input,"\n");
		rm=strtok(input," ");
		strcpy(temp,rm);
		rm=strtok(NULL," ");
		while(rm != NULL){
			strcat(temp," ");
			strcat(temp,home);
			strcat(temp,rm);
			rm=strtok(NULL, " ");
		}
		system(temp);
	}else{
	    printf("%s: command not found.\n", input);
	}
    }
    return 0;
}
