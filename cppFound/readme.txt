Quite a Shell (quash) READ.ME

GRADING POLICY
Partial credits will be given for incomplete eﬀorts. However, a program that cannot compile will get 0 points. Point breakdown for features is below:

(X) 1. Run executables without arguments (10)
(X) 2. Run executables with arguments (10)
(X) 3. set for HOME and PATH work properly (5)
(X) 4. exit and quit work properly (5)
(X) 5. cd (with and without arguments) works properly (5)
(X) 6. PATH works properly. Give error messages when the executable is not found (10)
(X) 7. Child processes inherit the environment (5)
(X) 8. Allow background/foreground execution (&) (5)
(X) 9. Printing/reporting of background processes, (including the jobs command) (10)
(X) 10. Allow file redirection (> and <) (5)
(X) 11. Allow (1) pipe (|) (10)
(X) 12. Supports reading commands from prompt and from file (10)
(X) 13. Report (10)


(-) 14. Bonus points (you can get bonus points only if you have everything else working (or very close to working))
	(-) Support multiple pipes in one command. (10)
	(X) kill command delivers signals to background processes. The kill command has the format:
		kill SIGNUM JOBID, where SIGNUM is an integer specifying the signal number, and JOBID is an integer that specifies the job that should receive the signal. (5)

Compiling and Testing:
	Compile using the make command.
	Test using the make test command and alter the input accordingly. Right now there is no input file present. The help command will display the input used to test file input or see the read commands from a file feature discussed earlier in this report.

