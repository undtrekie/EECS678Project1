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
	    system("jobs -l");
	}else if(strncmp(input,quit,4)==0){
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input,ex,4)==0){	
		printf("Good-Bye.\n");
		return 0;
	}else if(strncmp(input, "./", 1)==0){
		char temp[MAX_LENGTH];
		char prog[MAX_LENGTH];
		char alt[MAX_LENGTH];
		strtok(input, "\n");
		strcpy(alt,input);
		strcpy(temp,wDirectory);
		strcpy(prog,wDirectory);
		char* t=strtok(alt," ");
		int bg = 0;
		int exe=0;
		if((strchr(t,'&')) != NULL){
			t = strtok(input,"& ");
			bg = 1;
		}else{
			t = strtok(input," ");
		}
		char* args = strtok(NULL,">");
		char* redir = strtok(NULL," ");
		char* back = strtok(NULL,"\n");
		strcat(prog,t);
		if(access(prog, X_OK) != -1){
			exe=1;
		}
		strcat(temp,t);
		strcat(temp," ");
		if(args != NULL)
		{
			strcat(temp,args);
		}
		if(bg == 1){
			strcat(temp,"> /dev/null & ");
		}
		if(redir != NULL){
			strcat(temp,"> ");
			if(strchr(redir,'/')!=NULL){
				strcat(temp,redir);
			}
			else{
				strcat(temp,wDirectory);
				strcat(temp,redir);
			}
		}
		if(back != NULL){
			strcat(temp," ");
			strcat(temp,back);
		}
		if(exe==1){
			system(temp);
		}else{
			int j=0;
			int k = 0;
			while(j < 5){
				char c[MAX_LENGTH];
				strcpy(c,path[j]);
				strcat(c,t);
				strcat(c," ");
				if(args != NULL)
				{
					strcat(c,args);
				}
				if(bg == 1){
					strcat(c, "> /dev/null & ");
				}
				if(redir != NULL){
					strcat(c,"> ");
					if(strchr(redir,'/')!=NULL){
						strcat(c,redir);
					}
					else{
						strcat(c,wDirectory);
						strcat(c,redir);
					}
				}
				if(back != NULL){
					strcat(c," ");
					strcat(c,back);
				}
				strcpy(prog,path[j]);
				strcat(prog,t);
				if(access(prog,X_OK) != -1){
					k=1;
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
		strcpy(input," ");
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
	}else if(strchr(input,'>')!=NULL){
	    strtok(input,"\n");
	    char temp[MAX_LENGTH];
	    char* t = strtok(input," ");
	    strcpy(temp,t);
	    strcat(temp," ");
	    t=strtok(NULL," ");
	    if(strchr(t,'/') != NULL){
	    	strcat(temp,t);
	    	strcat(temp," ");
	    	strcat(temp,strtok(NULL," "));
	    }
	    else{
	    	strcat(temp,wDirectory);
	    	strcat(temp," ");
	    	strcat(temp,t);
	    }
	    strcat(temp," ");
	    t=strtok(NULL,"\n");
	    if(strchr(t,'/')!=NULL){
	    	strcat(temp,t);
	    }
	    else{
	    	strcat(temp,wDirectory);
	    	strcat(temp,t);
	    }
	    printf("command: %s\n",temp);
	    system(temp);
	}else if(strchr(input,'<')!=NULL){
	
	}else if(strchr(input,'|')!=NULL){
	
	}else{
	    printf("%s: command not found.\n", input);
	}
    }
    return 0;
}
