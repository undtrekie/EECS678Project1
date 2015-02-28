#include <string>
#include <vector>
#include <sys/wait.h>

using namespace std;

class commands
{
private:
  
public:

  FILE *inf;
  FILE *outf;

	commands();
	~commands();

	vector<int>pidstore;
	vector<int>jobidstore;
	vector<string>commandstore;

	int execute(char *filename, char *argv[]);
	bool execute_exist(string filename);

	int in_from_file(string filename);
	int out_to_file(string filename);

	void set(char *place, char *pathchan);
	void cd(char *buf);
	void jobs();
	void quit();
	void help();

	int stdin_from_file(string filename);
	int stdout_to_file(string filename);

	vector<string> splitstring(string str, char splitter);
	string removechars(string str, string chars);

	bool inputcheck(vector<string> cmds);
	bool outputcheck(vector<string> cmds);
	int inoutloc(vector<string> strs);

	int pipecheck(vector<string> strs);
	int connect(char *file1, char* argv1[], char *file2, char *argv2[]);
  int multconnect(vector<string> str);
  int pipecount(vector<string> str);

	void pidsave(int num);
	void jobidsave(int num);
	void commandsave(string str);
	void backgroundstartmessage();
	void backgroundendmessage();
	void jobsprint(int origStdin, int origStdout);

	void killProcess(int contract);
};
