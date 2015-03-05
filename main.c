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
    char fromFile[MAX_LENGTH];
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
    int jobids=1;
    pid_t childPIDs[MAX_LENGTH];
    char jobCmds[MAX_LENGTH][MAX_LENGTH];

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
    

    while(1){
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
	    char* setptr;
	    strtok(input, "\n");
	    setptr=strtok(input," ");
	    setptr=strtok(NULL,"=");
	    if(strcmp(setptr, "HOME")==0){
		setptr=strtok(NULL,"\n");
		strcpy(home, setptr);
	    }else if(strcmp(setptr, "PATH")==0){
		numPaths=0;
		setptr=strtok(NULL,":");
		while(setptr!=NULL){
		    strcpy(path[numPaths],setptr);
		    numPaths++;
		    setptr=strtok(NULL,":");
		}
	    }
	}else if(strncmp(input, cd, 2)==0){
	    char* cdtmp;
	    strtok(input, "\n");
	    cdtmp=strtok(input," ");
	    cdtmp=strtok(NULL, " ");
	    if(cdtmp==NULL){
		strcpy(wDirectory, home);
	    }else if(strcmp(cdtmp,"..")==0){
		char* cdtmp2;
		char wcopy[MAX_LENGTH];
		strcpy(wcopy, wDirectory);
		cdtmp2=strrchr(wcopy,'/');
		strncpy(cdtmp2, "\0",1);
		cdtmp2=strrchr(wcopy,'/');
		strncpy(cdtmp2, "\0",1);
		strcat(wcopy,"/");
		strcpy(wDirectory,wcopy);
	    }else{
		if(strncmp(cdtmp,"/", 1)==0){
		    strcpy(wDirectory, cdtmp);
		}else{
		    char cdfull[MAX_LENGTH];
		    strcpy(cdfull, wDirectory);
		    strcat(cdfull,cdtmp);
		    strcat(cdfull,"/");
		    strcpy(wDirectory, cdfull);
		}
	    }
	    
	}else if(strncmp(input,jobs,4)==0){
            printf("[JOBID]\tPID\tCOMMAND\n");
	    printf("-------\t---\t-------\n");
            for(i=0; i<jobids-1; i++){
                printf("%d\t%ld\t%s\n",i+1,childPIDs[i],jobCmds[i]);
            }	
	}else if(strncmp(input,quit,4)==0){
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input,ex,4)==0){	
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input, "./", 1)==0){
		char temp[MAX_LENGTH];
		char prog[MAX_LENGTH];
		strtok(input, "\n");
		char file[MAX_LENGTH];
		strcpy(file,input);
		strcpy(temp,wDirectory);
		strcat(temp,input);
		strcpy(prog,wDirectory);
		char* t = strtok(input," ");
		strcat(prog,t);
		if(access(prog, X_OK) != -1){
			system(temp);
		}else{
			int j=0;
			int k = 0;
			while(j < 5){
				char c[MAX_LENGTH];
				strcpy(c,path[j]);
				strcat(c,file);
				strcpy(prog,path[j]);
				strcat(prog,t);
				if(access(prog,X_OK) != -1){
					k=1;
					printf("c: %s\n", c);
					system(c);
					break;
				}
				else{
					j++;
				}
			}
			if(k==0){
				printf("Executable not in path.\n");
			}
		}
	}else if(strcmp(input, "ls\n")==0){
		strtok(input, "\n");
		strcat(input, " ");
		strcat(input,wDirectory);
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
			strcat(temp,wDirectory);
			strcat(temp,rm);
			rm=strtok(NULL, " ");
		}
		system(temp);
	}else if(strncmp(input,"make",4)==0){
		char temp[MAX_LENGTH];
		strtok(input, "\n");
		strcat(input, " -C ");
		strcat(input, wDirectory);
		system(input);
	}else if(strstr(input,"&")!=NULL){
	    
	}else{
		char temp[MAX_LENGTH];
		strtok(input,"\n");
		strcpy(temp,input);
		char* temper;
		strtok(temp," ");
		temper=strtok(NULL," ");
		if(strcmp(temper,"<")==0){
			printf("From File\n");
		}else if(strcmp(temper,">")==0){
			char to[MAX_LENGTH];
			char* next;
			next=strtok(input," ");
			strcpy(to,next);
			strcat(to," ");
			strcat(to,wDirectory);
			strcat(to," ");
			next=strtok(NULL," ");
			strcat(to,next);
			next=strtok(NULL," ");
			while(next != NULL){
				strcat(to," ");
				strcat(to,wDirectory);
				strcat(to,next);
				next=strtok(NULL," ");
			}
			system(to);
		}
		else{
	    	printf("%s: command not found.\n", input);
		}
	}
    }
    return 0;
}
