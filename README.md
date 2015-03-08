*********************************************
*   #####  #     #    #     #####  #     #  *
*  #     # #     #   # #   #     # #     #  *
*  #     # #     #  #   #  #       #     #  *
*  #     # #     # #     #  #####  #######  *
*  #   # # #     # #######       # #     #  *
*  #    #  #     # #     # #     # #     #  *
*   #### #  #####  #     #  #####  #     #  *
*********************************************

I. Introduction
    Welcome to the quash, quite a shell.  Quash was designed as a programming assignment
for EECS 678: Introduction to Operating Systems at the University of Kansas.  This
README was intended to be opened upon using the provided Makefile to compile the project.

II. Building and launching quash
    In order to build quash, please type "make" into the command line of a unix based 
computer.  This README should launch in vi to assist with the syntax of quash.  Please
see section III for syntax specification.  To launch quash, type:
    	    "./quash </home directory/> </path directory/>"
where home directory is the directory that you would like to set as home, and path
directory is the directory or directories (separated by ':') that you would like to have
as your path(s).

III. Syntax Specification
    For all path specification, AN ENDING '/' IS REQUIRED.
    The following commands (and their proper syntax) are included with quash.

    cd [PATH]
       -Changes current working directory to specified.
       -PATH can be empty, a folder within the current working directory, or be a full
        path specification.

    set {[PATH/HOME]=/<directory>}
       -Sets path or home environment variables accordingly.
       -PATH or HOME must be specified.
       -if more than one directory is to be added to path, separate by ':'
       	   Example: set PATH=/usr/bin/:/usr/

    jobs
       -Displays any running jobs.

    quit
       -Exits quash.

    exit
       -Exits quash.

    ./[EXECUTABLE] [OPTIONAL ARGUMENTS]
       -Any executable can be given as long as it is either in path, or current working
        directory.  Additional optional arguments can also be given which are passed to
	the program.

    &
       -Appending '&' on to any executable allows for background execution

    <
       -Can be used to run a list of commands from a file.  NOTE: the file must have a final
        command of either exit or quit.

    >
       -Used in conjunction with '&', '>' allows all output of the background executing
        program to be redirected to a file. Does not allow the same with programs not
	running in the background.

    |
       -Allows output of one command to be piped as input to the next command.  Up to 4
        pipes can be used.

    kill [pid]
       -Allows for termination of ongoing background process. 

IV. Conclusion
     Thank you for trying quash.  As a work in progress, this program is subject to
revision.  Please contact either Tyler Steiner (tsteiner@ku.edu) or Michael Esry 
(m716e163@ku.edu) for any questions or comments.