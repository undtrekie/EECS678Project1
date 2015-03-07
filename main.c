/*
*	@filename: main.c
*	@authors: Tyler Steiner and Michael Esry
*	@date: 2015.02.23
*	Purpose: Program Main for Quite a Shell (quash) Project.
*/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_LENGTH 80

int main(int argc, char** argv, char** envp){
	char cwd[MAX_LENGTH];
	getcwd(cwd, sizeof(cwd));
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
    int fileBreak=0;;
    int ch;
    char* split;
    int jobids=0;
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
		if(strcmp(input,"exit")==0){
			printf("\n%s\n",input);
		}

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
	    for(i=1; i<=jobids; i++){
	    	printf("[%d]\t%d\t%s\n",jobids,(childPIDs[(i-1)]+1),jobCmds[(i-1)]);
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
		char alt[MAX_LENGTH];
		strtok(input, "\n");
		strcpy(alt,input);
		strcpy(temp,wDirectory);
		strcpy(prog,wDirectory);
		char* t=strtok(alt," ");
		int bg = 0;
		int bgd = 0;
		int exe=0;
		if((strchr(t,'&')) != NULL){
			strcpy(alt,input);
			t = strtok(input,"& ");
			bg = 1;
		}else{
			strcpy(alt,input);
			t = strtok(input," ");
		}
		char* args = strtok(NULL,">");
		char* redir = strtok(NULL," ");
		char* bckg = strtok(NULL,"\n");
		args=strtok(args," ");
		char* fFile=strtok(NULL," ");
		fFile=strtok(NULL,"\n");
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
			bgd=1;
		}
		if(fFile != NULL){
			strcat(temp," < ");
			if(strchr(fFile,'/')!=NULL){
				strcat(temp,fFile);
			}
			else{
				strcat(temp,wDirectory);
				strcat(temp,fFile);
			}
		}
		if(redir != NULL){
			strcat(temp," > ");
			if(strchr(redir,'/')!=NULL){
				strcat(temp,redir);
			}
			else{
				strcat(temp,wDirectory);
				strcat(temp,redir);
			}
		}
		if(bckg != NULL){
			strcat(temp," ");
			strcat(temp,bckg);
			bgd=1;
		}
		if(exe==1){
			if(bgd==1){
				jobids++;
				int status;
				strcpy(jobCmds[(jobids-1)],alt);
				childPIDs[(jobids-1)] = fork();
				if(childPIDs[(jobids-1)] > 0){
					printf("[%d] %d Started\n",jobids,(childPIDs[(jobids-1)]+1));
				}
				if(childPIDs[(jobids-1)] == 0){
					if((execl("/bin/bash", "/bin/bash", "-c", temp, (char *) 0)) < 0){
						fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
						return EXIT_FAILURE;
					}
					exit(0);
					printf("[%d] %d Done\n",jobids,(childPIDs[(jobids-1)]+1));
				}
				
				if ((waitpid(childPIDs[(jobids-1)], &status, 0)) == -1) {
					fprintf(stderr, "Process %d encountered an error. ERROR%d\n", jobids, errno);
					return EXIT_FAILURE;
				}
				else{	
					printf("[%d] %d finished %s\n",jobids,(childPIDs[(jobids-1)]+1),jobCmds[(jobids-1)]);
				}
			}
			else{
				system(temp);
			}
		}else{
			bgd = 0;
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
					bgd=1;
				}
				if(fFile != NULL){
					strcat(c," < ");
					if(strchr(c,'/')!=NULL){
						strcat(c,fFile);
					}
					else{
						strcat(c,wDirectory);
						strcat(c,fFile);
					}
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
				if(bckg != NULL){
					strcat(c," ");
					strcat(c,bckg);
					bgd=1;
				}
				strcpy(prog,path[j]);
				strcat(prog,t);
				if(access(prog,X_OK) != -1){
					k=1;
					if(bgd==1){
						jobids++;
						int status;
						strcpy(jobCmds[(jobids-1)],alt);
						childPIDs[(jobids-1)] = fork();
						if(childPIDs[(jobids-1)] > 0){
							printf("[%d] %d Started\n",jobids,(childPIDs[(jobids-1)]+1));
						}
						if(childPIDs[(jobids-1)] == 0){
							if((execl("/bin/bash", "/bin/bash", c, (char *) 0)) < 0){
								fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
								return EXIT_FAILURE;
							}
							exit(0);
						}
				
						if ((waitpid(childPIDs[(jobids-1)], &status, 0)) == -1) {
							fprintf(stderr, "Process %d encountered an error. ERROR%d\n", jobids, errno);
							return EXIT_FAILURE;
						}
						else{
							printf("[%d] %d finished %s\n",jobids,(childPIDs[(jobids-1)]+1),jobCmds[(jobids-1)]);
						}
					}
					else{
						system(c);
					}
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
	}else if(strchr(input,'|')!=NULL){
		chdir(wDirectory);
		strtok(input,"\n");
		char* temp1 = strtok(input,"|");
		char* temp2 = strtok(NULL,"\n");
		int pipe1[2];
		pipe(pipe1);
		int status;
		pid_t pid_1;
		pid_t pid_2;
		
		pid_1 = fork();
		if(pid_1 == 0){
			char pbuf[256];
			bzero(pbuf,256);
			sprintf(pbuf,"%s",temp1);
			
			close(pipe1[0]);
			dup2(pipe1[1],1);
			
			if((execl("/bin/bash", "/bin/bash", "-c", pbuf, (char *) 0)) < 0){
				fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
				return EXIT_FAILURE;
			}
			
			close(pipe1[1]);
			exit(0);
		}
		
		pid_2 = fork();
		if(pid_2 == 0){
			char pbuf[256];
			bzero(pbuf,256);
			sprintf(pbuf,"%s",temp2);
			
			close(pipe1[1]);
			dup2(pipe1[0],0);
			
			if((execl("/bin/bash", "/bin/bash", "-c", pbuf, (char *) 0)) < 0){
				fprintf(stderr, "\nError execing find. ERROR#%d\n", errno);
				return EXIT_FAILURE;
			}
			
			close(pipe1[0]);
			exit(0);
		}
		
		close(pipe1[0]);
		close(pipe1[1]);
		
		if ((waitpid(pid_1, &status, 0)) == -1) {
			fprintf(stderr, "Process 1 encountered an error. ERROR%d", errno);
			return EXIT_FAILURE;
		}
		if ((waitpid(pid_2, &status, 0)) == -1) {
			fprintf(stderr, "Process 2 encountered an error. ERROR%d", errno);
			return EXIT_FAILURE;
		}
		chdir(cwd);
	}else if(strncmp(input,"kill",4)==0){
		strtok(input,"\n");
		char temp[MAX_LENGTH];
		char* t = strtok(input," ");
		strcpy(temp,t);
		strcat(temp," ");
		t=strtok(NULL," ");
		strcat(temp,t);
		system(temp);
	}else{
		strtok(input,"\n");
	    printf("%s: command not found.\n", input);
	}
    }
    return 0;
}
