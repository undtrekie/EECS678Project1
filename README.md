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

II. Syntax Specification
    For all path specification, an ending '/' is required.
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

    >
       -Used in conjunction with '&', '>' allows all output of the bacground executing
        program to be redirected to a file. Does not allow the same with programs not
	runing in the background.

    |
       -Allows output of one command to be piped as input to the next command.

III. Conclusion
     Thank you for trying quash.  As a work in progress, this program is subject to
revision.  Please contact either Tyler Steiner (tsteiner@ku.edu) or Michael Esry 
(m716e163@ku.edu) for any questions or comments.