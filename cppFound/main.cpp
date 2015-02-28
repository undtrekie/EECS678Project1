#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>

#include <sys/wait.h>
#include <unistd.h>

#include "commands.h"

using namespace std;

extern char **environ;

// Declaration for main as given in project parameters
int main (int argc, char **argv, char **envp)
{
  commands name;
  ifstream ifile;
  int jobNumber = 0;

  cout << "Quash Shell Start!" << endl;
  // Initialize Variables
  char buf[512];
  //char buf2[512];
  int argCount = 2;

  if ( argc == 2 )
  {
    cout << "Using commands from the file: " << argv[1] << endl;
    ifile.open( argv[1] );
  }

  //Save Standard Input Output
  int origStdin = dup( STDIN_FILENO );
  int origStdout = dup( STDOUT_FILENO );

  while(1)
  {
    // Check to see if background processes are complete
    name.backgroundendmessage();

    //Print the Shell Prompt
    printf("Quash: $$ ");
    memset( buf, ' ', 512 );

    // Are we taking commands from a file? Or are we taking them from
    // terminal?
    if ( ifile.is_open() )
    {
      if ( !ifile.eof() )
      {
        ifile.getline( buf, 512 );
      }
      else
      {
        ifile.close();
      }
    }
    else
    {
      // the fgets function gets user input and stores it in buf
      char* k = fgets( buf, 512, name.inf ); // This line causes a seg fault
      if( k == NULL )
      {
         //cout << "Redirecting!!!" << endl;
         // Redirect standard input back to what it was
         name.inf = stdin;
         dup2( origStdin, STDIN_FILENO );
     }
    }

    //Convert the buffer to a string
    string str(buf);


    // Handle the case where the input is the line break character
    if( str[0] != '\n' && str[0] != ' '  )
    {
      str.erase(remove(str.begin(), str.end(), '\n'), str.end());
      name.removechars(str, " ");
      //cout << str << endl;
    }
    
    vector<string>compare = name.splitstring(str, ' ');
 
    // Find out if we need to redirect standard output
    int counter = 0;
    if ( name.outputcheck( compare ) )
    {
         // check for output
         counter = name.inoutloc( compare );
         if ( compare[ counter ] != compare.back() )
         {
             if( compare.back() == compare[ counter + 1 ] )
             {
                 // Redirect standard output then erase the last two elements
                 // in the Vector so that it doesn't mess with command
                 // execution
                 name.stdout_to_file( compare.back() );
                 compare.erase( compare.end() );
                 compare.erase( compare.end() );
             }
         }
         //name.stdout_to_file( compare[ counter + 1 ] );
    }

    // Check to see if we need to pipe
    int pipeCheck = name.pipecheck( compare );

    if( compare.empty() )
    {
     // cout << "empty" << endl;
    }
    else if( compare.front() == "exit" || compare.front() == "quit" )
    {
      name.quit();
    }
   /* else if( pipeCheck != -1 )
    {
      name.multconnect(compare);
    }
*/
    else if( pipeCheck != -1 )
    {
      if( compare.back() != compare[ pipeCheck ] )
      {
         string command1 = compare.front();
         string command2 = compare[pipeCheck + 1];
         string arguments = "";

         char * argv2[ compare.size() + 2];
         int j = 0;
         while( compare.front() != "|" )
         {
             arguments = compare.front();
             char * firstCommand = new char[ arguments.size() + 1 ];
             arguments[ arguments.size() ] = 0;
             memcpy( firstCommand, arguments.c_str(), arguments.size() );
             argv2[j] = firstCommand;
             compare.erase( compare.begin() );
             j++;
         }
         //Make sure we are terminating the argv array with a NULL
         argv2[j] = NULL;

         if( compare.front() == "|" )
         {
           //cout << "Parsed the first command correctly!" << endl;
           compare.erase( compare.begin() );
         }

         j = 0;
         char * argv3 [ compare.size() + 2 ];
         while( !compare.empty() )
         {
             arguments = compare.front();
             char * secondCommand = new char[ arguments.size() + 1];
             arguments[ arguments.size() ] = 0;
             memcpy( secondCommand, arguments.c_str(), arguments.size() );
             argv3[j] = secondCommand;
             compare.erase( compare.begin() );
             j++;
         }
         argv3[j] = NULL;

         // Convert command 1 and command 2 to const char *
         // NEED TO FINISH THIS SHIT
         char * convert1 = new char[ command1.size() + 1 ];
         char * convert2 = new char[ command2.size() + 1 ];
         convert1[ command1.size() ] = 0;
         convert2[ command2.size() ] = 0;
         memcpy( convert1, command1.c_str(), command1.size() );
         memcpy( convert2, command2.c_str(), command2.size() );
         name.connect(convert1, argv2, convert2, argv3 );
      }
      else
      {
        cout << "Error: Please enter two commands with | " << endl;
      }
    }
    else if( "cd" == compare.front() )
    {
      //cout << "This is where we change directories!" << endl;
      compare.erase(compare.begin());
      if( !compare.empty() )
      {
         //cout << "The next pch value is: " << compare.front() << endl;
         string temp = compare.front();
         compare.erase(compare.begin());
         if( compare.empty() )
         {
           char * other = new char[ temp.size() + 1 ];
           other[ temp.size() ] = 0;
           memcpy( other, temp.c_str(), temp.size() );
           name.cd( other );
         }
         else
         {
           cout << "Invalid Arguments: cd must have one argument!" << endl;
         }
      }
      else
      {
        name.cd( NULL );
      }
    }
    else if( compare.front() == "<" )
    {
      compare.erase( compare.begin() );
      if ( !compare.empty() )
      {
          string theString = compare.front();
          compare.erase( compare.begin() );
          if( compare.empty() )
          {
            /*
            char * theFile = new char [ theString.size() + 1 ];
            theFile[ theString.size() ] = 0;
            memcpy( theFile, theString.c_str(), theString.size() );
            ifile.open( theFile );*/
            cout << "Reading input from the file: " << theString<<endl;
            name.stdin_from_file( theString );
          }
          else
          {
            cout << "Error: run must take only one argument!" << endl;
          }
      }
      else
      {
        cout << "Error: run command must specify a file!" << endl;
      }
    }
    else if( compare.front() == "kill" )
    {
       cout << "This is where we brutally murder a process in its sleep!" << endl;
       compare.erase( compare.begin() );
       if ( !compare.empty() )
       {
         string contract = compare.front();
         compare.erase( compare.begin() );
         if( compare.empty() )
         {           
            char * theFile = new char [ contract.size() + 1 ];
            theFile[ contract.size() ] = 0;
            memcpy( theFile, contract.c_str(), contract.size() );
            int theContract = 0;
            theContract = atoi( theFile );
            name.killProcess( theContract );
         }
         else
         {
           cout << "Error: You ask too much my liege! I can only terminate one process at a time." << endl;
         }
       }
       else
       {
         cout << "Error: I cannot kill a process you do not specify my dark lord." << endl;
       }
    }
    else if( compare.front() == "help" )
    {
      name.help();
    }
    else if( compare.front() == "set" )
    {
      //cout << "This is where we set the home and path variable!" << endl;
      compare.erase( compare.begin() );
      if( !compare.empty() )
      {
          //cout << "The next pch value is: " << compare.front() << endl;
          string arg1 = compare.front();
          compare.erase( compare.begin() );
          if( !compare.empty() )
          {
            string arg2 = compare.front();
            compare.erase( compare.begin() );
            if( compare.empty() )
            {
              char * other1 = new char[ arg1.size() + 1 ];
              char * other2 = new char[ arg2.size() + 1 ];
              other1[ arg1.size() ] = 0;
              other2[ arg2.size() ] = 0;
              memcpy( other1, arg1.c_str(), arg1.size() );
              memcpy( other2, arg2.c_str(), arg2.size() );
              name.set(other1, other2);
            }
            else
            {
              cout << "Too many arguments!!" << endl;
            }
          }
          else
          {
            cout << "Set requires two arguments!" << endl;
          }
      }
      else
      {
         cout << "Set requires two arguments!" << endl;
      }
    }
    else if( compare.front() == "jobs" )
    {
        //cout << "List all of the jobs running in the background." << endl;
        name.jobsprint(origStdin, origStdout );
    }
    else if( compare.front() == "\n")
    {
    }
    else
    {
        //Convert to a char* Array
        string com = compare.front();

        bool isBackground = false;

        // If there is a '&' then process it in the background
        if ( '&' == *com.rbegin() )
        {
            com.erase( com.rfind( "&" ) );
            isBackground = true;
        }

        // Convert the string to a Char *
        char * executable = new char[ com.size() + 1 ];
        com[ com.size() ] = 0;
        memcpy( executable, com.c_str(), com.size() );
        
        // Erase the first element of the list
        compare.erase( compare.begin() );


        int pid = fork();
        if( pid != 0 )
        {
            if( isBackground )
            {
              name.pidsave( pid );
              name.jobidsave( jobNumber );
              name.commandsave( com );
              jobNumber++;
              isBackground = false;
              name.backgroundstartmessage();
            }
            else
            {
              wait( NULL );
            }
            // Redirect input output for the parent
            dup2( origStdin, STDIN_FILENO );
            dup2( origStdout, STDOUT_FILENO );
        }
        else
        {
            // If there is a background 
            if ( isBackground )
            {
               setpgid(0, 0);

               //cout << "Process in background" << endl;
               freopen( "/dev/null", "w", stdout );
               freopen( "/dev/null", "w", stderr );

               isBackground = false;
            }

            //Handle commands with arguments. Convert vector to char * array
            char * argv[ compare.size() + 2];
            argv[0] = executable;
            int i = 1;
            
            //compare.erase( compare.begin() );
            while( !compare.empty() )
            {
                com = compare.front();
                char * executable2 = new char[ com.size() + 1 ];
                com[ com.size() ] = 0;
                memcpy( executable2, com.c_str(), com.size() );
                argv[i] = executable2;
                compare.erase( compare.begin() );
                i++;
            }
               
            //Make sure we are terminating the argv array with a NULL
            argv[i] = NULL;

            // Call Commands
            name.execute( executable, argv );
        
        }
    }
    
    // Cleanup the vector after every iteration
    while ( !compare.empty() )
    {
        compare.erase( compare.begin() );
    }

    //Reset standard  output
    dup2( origStdout, STDOUT_FILENO );
  }
  return 0;
}
