#include "shell_core.h"
#include "logger.h"
#include <iostream>
#include <string.h>
#include <vector>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <signal.h>

using namespace std;

vector<string> splitCommand(string commandLine);
bool initializeExecution(vector<string> arguemnts);
void execute(vector<string> arguments, bool isisAsynchronous);
static void signalHandler(int signal);
char *convert(const string &s);

const string EXIT = "exit";
const string ASYNCHRONOUS = "&";
const string CHANGE_DIRECTORY = "cd";
const int CD_ARGUMENTS_SIZE = 2;

shell_core::shell_core()
{

    //ctor
}

shell_core::~shell_core()
{
    //dtor
}

/**
* Intializes the basic shell loop of: getting command, splitting the command
* to its arguments and finally executing it.
*/
void shell_core::initializeShell() {
    string commandLine;
    vector<string> arguments;
    bool isExit = false;

    do {
        printf("> ");
        getline(cin, commandLine);
        arguments = splitCommand(commandLine);
        isExit = initializeExecution(arguments);

    } while (!isExit);
    exit(0);
}

/**
* Splits the command into its arguments.
*@param commandLine; the command line from the user.
*@return arguments of the command line as a vector.
*/
vector<string> splitCommand(string commandLine) {
    vector<string> arguments;

    string argument = "";
    commandLine += " ";
    for (auto character : commandLine) {
        if (character == ' ') {
            if (argument == "") continue;
            arguments.push_back(argument);
            argument = "";
        } else if (character == '\n' || character == '\t') {
            continue;
        } else {
            argument += character;
        }
    }
    commandLine.clear();
    return arguments;
}

/**
* Determines whether this is an asyncrhonous or syncrhonous execution. Also
* executes some non-execvp-supported commands.
*@param arguments of the command line in vector form.
*@return true if executable, or false incase of exit.
*/
bool initializeExecution(vector<string> arguments) {
    if (!arguments.empty()) {
        bool isAsynchronous = false;

        if (arguments[0] == EXIT) {
            return true;
        } else if (arguments[0] == CHANGE_DIRECTORY) {
            if (arguments.size() == CD_ARGUMENTS_SIZE) {
                vector<char*> convertedArguments;
                transform(arguments.begin(), arguments.end(), back_inserter(convertedArguments), convert);
                int cd = chdir(convertedArguments[1]);
                if (cd < 0) {
                   perror(CHANGE_DIRECTORY.c_str());
                }
            } else {
                cout << "CD: Too many arguments" << endl;
            }
            return false;
        }

        if (arguments[arguments.size() - 1] == ASYNCHRONOUS) {
            arguments.erase(arguments.begin() + arguments.size() - 1);
            isAsynchronous = true;
        }

        execute(arguments, isAsynchronous);

    }
    return false;
}

/**
* Executes the command.
*@param arguements of the command line, and execution type.
*/
void execute(vector<string> arguments, bool isAsynchronous) {
    pid_t pid;
    int status;

    vector<char*> convertedArguments;
    transform(arguments.begin(), arguments.end(), back_inserter(convertedArguments), convert);
    convertedArguments.push_back(NULL);

    arguments.clear();

    pid = fork();
    if (pid == 0) {
        execvp(convertedArguments[0], &convertedArguments[0]);
        perror(convertedArguments[0]);
        //cout << "CHILD ERROR" << endl;
    } else if (pid < 0) {
        perror(convertedArguments[0]);
    } else {
        if (!isAsynchronous) {
            signal(SIGCHLD, SIG_IGN);
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        } else {
            signal(SIGCHLD, signalHandler);
        }
    }
}

/**
* Handles SIGCHLD incase of child terminates and logs that to a file.
*@param singal ID which is mainly SIGCHLD.
*/
static void signalHandler(int signal) {
    logger lg;
    pid_t chpid = wait(NULL);
    lg.log(chpid);
}

/**
* Converts a string to char* for execvp execution.
* Used with tranform fucntion.
*@param command argument in form of string.
*@return command argument in form of char*.
*/
char *convert(const string &s) {
   char *pc = new char[s.size()+1];
   strcpy(pc, s.c_str());
   return pc;
}
