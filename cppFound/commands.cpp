#include "commands.h"

#include <unistd.h>
#include <string.h>
#include <cerrno>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <vector>
#include <sstream>
#include <csignal>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
//#include <process.h>
//#include <process.h>// int execvpe( const char * file, char * const argv[], char * const envp[] );

extern char **environ;

using namespace std;

commands::commands()
{
	//constuctor
  inf = stdin;
  outf = stdout;
}
commands::~commands()
{
	//deconstructor
}

int commands::execute(char *filename, char *argv[])
{
	//if there's a path to the executable
	if(filename[0] == '/')
	{
		//write search function for the file it will search for an executable file
		if(execute_exist(filename) == true)
		{
			if(execve(filename, argv, environ) < 0)
			{
				cerr<<"Error executing."<<endl;
				return EXIT_FAILURE;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return EXIT_FAILURE;
		}
		//else the executable exists in the same directory or among the paths
	}else if(strncmp(filename, "./", 2) == 0)
	{
		if(execute_exist(filename) == true)
		{
			if(execve(filename, argv, environ) < 0)
			{
				cerr<<"Error executing."<<endl;
			}
			else
			{
				return 1;
			}
		}
		else
		{
			return EXIT_FAILURE;
		}
	}
	else
	{
		
    if ( argv[0] == NULL )
    {
      cout << "This is a NULL" << endl;
    }

		execvpe(filename, argv, environ);
	}
	cerr<<"The command was not found."<<endl;
	return EXIT_FAILURE;

}

bool commands::execute_exist(string filename)
{
	if(access(filename.c_str(), F_OK) != 0)
	{
		cerr<<"File does not exist."<<endl;
		return false;
	}

	if(access(filename.c_str(), X_OK) != 0)
	{
		cerr<<"File cannot be executed."<<endl;
		return false;
	}

	return true;

}

void commands::set(char *place, char *pathchan)
{
	if(!place || !pathchan)
	{
		cerr<<"Need path or home."<<endl;
		return;
	}

	if(strncmp(place, "home", 4) == 0)
	{
		if(setenv("HOME", pathchan, 1) == -1)
		{
			cerr<<"Error with home."<<endl;
		}
	}
	else if(strncmp(place, "path", 4) == 0)
	{
		if(setenv("PATH", pathchan, 1) == -1)
		{
			cerr<<"Error with path."<<endl;
		}
	}
}

void commands::cd(char *buf)
{
	cout<<"Change directories."<<endl;
	if(buf == NULL)
	{
		cout<<"No directory was specified, going home."<<endl;
		if(chdir(getenv("HOME")) != 0)
		{
			cerr<<"Couldn't go home."<<endl;
		}
	}else//buf is not empty, try to cd to the directory
	{
		if(buf[0] == '/')
		{
			if(chdir(buf) != 0)
			{
				cerr<<"Couldn't change the directory."<<endl;
			}
		}else
		{
			char temp[512];
			if(getcwd(temp, sizeof(temp)) == NULL)
			{
				cerr<<"getcwd error."<<endl;
			}
			string str = temp;
			if(chdir((str + "/" + buf).c_str()) != 0)
			{
				cerr<<"Couldn't find the next directory."<<endl;
			}
		}
	}
}

void commands::quit()
{

	cout<<"Now exiting Quash, have a nice day!"<<endl;
	cout<<"...... . . . .  .  .   .    .     ."<<endl;
	exit(0);

}

void commands::help()
{
	cout<<"************************************************************************************"<<endl;
	cout<<"Quash: Quite a Shell                                                               *"<<endl;
	cout<<"EECS 678 Project 1                                                                 *"<<endl;
	cout<<"Shannon Bisges & Josh Collins                                                      *"<<endl;
	cout<<"************************************************************************************"<<endl;
	cout<<"1. Run executables without arguments (10)                                          *"<<endl;
	cout<<"  ./lab1 or /Users/sebisges/git/EECS678Project1/lab1 or ls                         *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"2. Run executables with arguments (10)                                             *"<<endl;
	cout<<"  ./lab1 input.txt or /Users/sebisges/git/EECS678Project1/lab1 input.txt           *"<<endl;
	cout<<"                                                                                   *"<<endl;
	cout<<"3. set for HOME and PATH work properly (5)                                         *"<<endl;
	cout<<"  set home /Users/sebisges/git/EECS678Project1                                     *"<<endl;
	cout<<"  set path /Users/sebisges/git/EECS678Project1:/Users/sebisges/git/EECS678Project1 *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"4. exit and quit work properly (5)                                                 *"<<endl;
	cout<<"  quit or exit                                                                     *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"5. cd (with and without arguments) works properly (5)                              *"<<endl;
	cout<<"  cd or cd filename or cd /Users/sebisges/git/EECS678Project1/filename             *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"6. PATH works properly. Give error messages when the executable is not found (10)  *"<<endl;
	cout<<"  Utilized when executing.                                                         *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"7. Child processes inherit the environment (5)                                     *"<<endl;
	cout<<"  Utilized when executing.                                                         *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"8. Allow background/foreground execution (&) (5)                                   *"<<endl;
	cout<<"  yourcommand&                                                                     *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"9. Printing/reporting of background processes, (including the jobs command) (10)   *"<<endl;
	cout<<"  jobs                                                                             *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"10. Allow file redirection (> and <) (5)                                           *"<<endl;
	cout<<"  cd > test.txt                                                                    *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"11. Allow (1) pipe (|) (10)                                                        *"<<endl;
	cout<<"  command | command or command arg | command arg                                   *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"12. Supports reading commands from prompt and from file (10)                       *"<<endl;
	cout<<"  ./quash <input.txt or ./quash input.txt                                          *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"13. Report (10)                                                                    *"<<endl;
	cout<<"  Included.                                                                        *"<<endl;
  cout<<"                                                                                   *"<<endl;
	cout<<"14. Bonus points:                                                                  *"<<endl;
	cout<<"      Support multiple pipes in one command. (10)                                  *"<<endl;
	cout<<"        Not implemented. Code is there, but commented out. We believe              *"<<endl;
  cout<<"        we got close the correct method.                                           *"<<endl;
  cout<<"                                                                                   *************"<<endl;
	cout<<"      Kill command delivers signals to background processes. The kill command has the format:  *"<<endl;
	cout<<"           kill SIGNUM JOBID, where SIGNUM is an integer specifying the signal number,         *"<<endl;
	cout<<"           and JOBID is an integer that specifies the job that should receive the signal. (5)  *"<<endl;
  cout<<"        kill pid                                                                               *"<<endl;
  cout<<"************************************************************************************************"<<endl;
	cout<<endl;
}

int commands::stdin_from_file(string filename)
{
	if(filename.length() > 0)
	{
		inf = fopen(filename.c_str(), "r");
		if(inf == NULL)
		{
			cerr<<"Couldn't open the file for input."<<endl;
			inf = stdin;
      return 0;
		}

		dup2(fileno(inf), STDIN_FILENO);
		fclose(inf);
	}
	return 1;
}

int commands::stdout_to_file(string filename)
{
	if(filename.length() > 0)
	{
		outf = fopen(filename.c_str(), "w");
		if(outf == NULL)
		{
			cerr<<"Couldn't open the file for output."<<endl;
            outf = stdout;
			return 0;
		}

		dup2(fileno(outf), STDOUT_FILENO);
		fclose(outf);
	}
	return 1;
}

vector<string> commands::splitstring(string str, char splitter)
{
	stringstream ss(str);
	string part;
	vector<string>portions;

	while(getline(ss, part, splitter))
	{
		if(!part.empty())
		{
			portions.push_back(part);
		}
	}

	/*for(int i = 0; i<portions.size(); i++)
	{
		cout<<"portions "<<portions[i]<<endl;
	}*/
	return portions;
}

string commands::removechars(string str, string chars)
{
	int start = str.find_first_not_of(chars);
	int finish = str.find_last_not_of(chars);
	int range = finish - start +1;

	return str.substr(start, range);
}

//< a : take input from file a
bool commands::inputcheck(vector<string> strs)
{
	for(int i = 0; i<strs.size(); i++)
	{
		if(strs[i] == "<")
		{
			return true;
		}
	}
	return false;
}

//ls > a : output from ls goes to file a
bool commands::outputcheck(vector<string> strs)
{
	for(int i = 0; i<strs.size(); i++)
	{
		if(strs[i] == ">")
		{
			return true;
		}
	}
	return false;
}

int commands::inoutloc(vector<string> strs)
{
	for(int i = 0; i<strs.size(); i++)
	{
		if(strs[i] == ">" || strs[i] == "<")
		{
			return i;
		}
	}
	return -1;
}

int commands::pipecheck(vector<string> strs)
{
	for(int i = 0; i<strs.size(); i++)
	{
		if(strs[i] == "|")
		{
			return i;
		}
	}
	return -1;
}

int commands::connect(char *file1, char* argv1[], char *file2, char *argv2[])
{
	pid_t pid1, pid2;
	int fds1[2];
	pipe(fds1);
	int status;

	pid1 = fork();
	if(pid1 == 0)
	{
		dup2(fds1[1], STDOUT_FILENO);
		close(fds1[0]);
		char cmdbuf[512];
		bzero(cmdbuf, 512);
		execute(file1, argv1);
		exit(0);
	}

	pid2 = fork();
	if(pid2 == 0)
	{
		dup2(fds1[0], STDIN_FILENO);
		close(fds1[1]);
		char cmdbuf[512];
		bzero(cmdbuf, 512);
		execute(file2, argv2);
		exit(0);
	}
	close(fds1[1]);
	close(fds1[0]);

	if((waitpid(pid1, &status, 0)) == -1)
	{
		cerr<<"Process 1 encountered an error."<<endl;
		return EXIT_FAILURE;
	}
	if((waitpid(pid2, &status, 0)) == -1)
	{
		cerr<<"Process 2 encountered an error."<<endl;
		return EXIT_FAILURE;
	}

	return 0;
}

/*
int commands::multconnect(vector<string> str)
{
  //cout<<"PIPING"<<endl;
  // Initialize Variables
  int num = pipecount(str);
  //cout<<num<<endl;
  string arguments = "";
  char * argv[ str.size() + 2 ];
  int it = 0;
  for ( int k = 0; k != ( str.size() + 2 ); k++)
  {
    argv[k] = NULL;
  }
  
  //int num = pipecount(str);
	int size = num+1;
	//cout<<size<<endl;
  pid_t pid[size];
  int fds[size][2];
	int status;
	for(int i = 0; i<size; i++)
	{
		//cout<<"pipe"<<endl;
    pipe(fds[i]);
	}
	for(int i = 0; i<size; i++)
	{
    if( i == num )
    {
      while( !str.empty() )
          {
            arguments = str.front();
            char * theCommand = new char [ arguments.size() + 1 ];
            arguments[ arguments.size() ] = 0;
            memcpy( theCommand, arguments.c_str(), arguments.size());
            argv[it] = theCommand;
            it++;
            str.erase( str.begin() );
          }
    }
    else
    {
      while( str.front() != "|" )
          {
            arguments = str.front();
            char * theCommand = new char [ arguments.size() + 1 ];
            arguments[ arguments.size() ] = 0;
            memcpy( theCommand, arguments.c_str(), arguments.size());
            argv[it] = theCommand;
            it++;
            str.erase( str.begin() );
          }
          str.erase( str.begin() );
    }
          argv[it] = NULL;
          it = 0;
          while (argv[it] != NULL)
          {
             cout<<argv[it] << " "; //<<endl;
             it++;
          }
          cout << "    :   End of command list" << endl;
          it = 0;
      if(argv[1] == NULL)
      {
        cout<<"NULL"<<endl;
      }
      else
      {
        cout<<argv[1]<<endl;
      }
    //cout<<i<<endl;
		if(i == 0)
		{
      //cout<<"1111"<<endl;
			pid[i] = fork();
			if(pid[i] == 0)
			{
        //cout<<"in1"<<endl;
				dup2(fds[i][1], STDOUT_FILENO);
				close(fds[i][0]);
				for(int j = 0; j<size; j++)
        {
          if(j != i)
          {
            close(fds[j][0]);
            close(fds[j][1]);
          }
        }
        //cout<<"1"<<endl;
				char cmdbuf[512];
				bzero(cmdbuf, 512);
        execute(argv[0], argv);
				exit(0);
			}
		} else if(i == num)
		{
      //cout<<"2222"<<endl;
			pid[i] = fork();
      if(pid[i] == 0)
      {  
        //cout<<"in2"<<endl;
        
        dup2(fds[i][0], STDIN_FILENO);
				close(fds[i][0]);
				for(int j = 0; j<size; j++)
        {
          if(j != i)
          {
            close(fds[j][0]);
            close(fds[j][1]);
          }
        }
				char cmdbuf[512];
				bzero(cmdbuf, 512);	
        execute(argv[0], argv);
				exit(0);
      }
		}
		else
		{
      //cout<<"3333"<<endl;
			pid[i] = fork();
      if(pid[i] == 0)
      {
        //cout<<"in3"<<endl;
				dup2(fds[i-1 ][0], STDIN_FILENO);
        dup2(fds[i][1], STDOUT_FILENO);
        close(fds[i-1][1]);
				close(fds[i][0]);
				for(int j = 0; j<size; j++)
        {
          if(j != i && j != i-1)
          {
            close(fds[j][0]);
            close(fds[j][1]);
          }
        }
				char cmdbuf[512];
				bzero(cmdbuf, 512);
        execute(argv[0], argv);
        exit(0);
			}
		}
	}
  for(int i = 0; i<size; i++)
  {
    close(fds[i][0]);
    close(fds[i][1]);
  }
  for(int i = 0; i<size; i++)
  {
    if((waitpid(pid[i], &status, 0)) == -1)
    {
      cerr<<"Process " <<i<<"encountered an error."<<endl;
      return EXIT_FAILURE;
    }
  }
}
int commands::pipecount(vector<string> str)
{
	int i = 0;
	int count = 0;
	while(i < str.size ())
	{
    cout<<str.at(i)<<endl;
		if(str.at(i) == "|")
		{
			count++;
		}
		i++;
	}
	return count;
}*/

void commands::pidsave(int num)
{
	pidstore.push_back(num);
}

void commands::jobidsave(int num)
{
	jobidstore.push_back(num);
}

void commands::commandsave(string str)
{
	commandstore.push_back(str);
}

void commands::backgroundstartmessage()
{
	cout<<"["<<jobidstore.back()<<"] "<<pidstore.back()<<" running in background."<<endl;
}

void commands::backgroundendmessage()
{
  if( !jobidstore.empty() && !commandstore.empty() && !pidstore.empty() )
  {
    for(int i = 0; i<jobidstore.size(); i++)
	  {
		  if(waitpid(pidstore[i], NULL, WNOHANG) != 0)
		  {
			  cout<<"["<<jobidstore.at(i)<<"] "<<pidstore.at(i)<<" finished "<<commandstore[i]<<endl;
			  jobidstore.erase( jobidstore.begin() + i );
			  commandstore.erase( commandstore.begin() + i );
			  pidstore.erase( pidstore.begin() + i );
		  }
		
	  }
  }
}

void commands::jobsprint(int origStdin, int origStdout)
{

  dup2( origStdin, STDIN_FILENO );
  dup2( origStdout, STDOUT_FILENO );

  
	cout<<"Current jobs: "<<endl;
	for(int i = 0; i<jobidstore.size(); i++)
	{
		cout<<"["<<jobidstore.at(i)<<"] "<<pidstore.at(i)<<" "<<commandstore.at(i)<<endl;
	}
  
}



void commands::killProcess(int contract)
{
	 if( kill( contract, SIGKILL ) !=  0 )
   {
     cout << "Mercy my lord. I have failed to kill process: " << contract << endl;
   }
   else
   {
     cout << "The process: " << contract << " has been dealt with." << endl;
   }
}
