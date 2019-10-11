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

using namespace std;

vector<string> splitCommand(string commandLine);
bool initializeExecution(vector<string> arguemnts);
void execute(vector<string> arguments, bool isisAsynchronous);
static void signalHandler(int signal);
char *convert(const string &s);

const string EXIT = "exit";
const string ASYNCHRONOUS = "&";
const char NULL_CHAR = '\0';
static logger lg;

shell_core::shell_core()
{

    //ctor
}

shell_core::~shell_core()
{
    //dtor
}

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

vector<string> splitCommand(string commandLine) {
    vector<string> arguments;

    string argument = "";
    commandLine += " ";
    for (auto character : commandLine) {
        if (character == ' ') {
            if (argument == "") continue;
            arguments.push_back(argument);
            argument = "";
        } else {
            argument += character;
        }
    }
    return arguments;
}

bool initializeExecution(vector<string> arguments) {
    if (!arguments.empty()) {
        bool isAsynchronous = false;

        if (arguments[0] == EXIT) {
            return true;
        }

        if (arguments[arguments.size() - 1] == ASYNCHRONOUS) {
            arguments.erase(arguments.begin() + arguments.size() - 1);
            isAsynchronous = true;
        }

        execute(arguments, isAsynchronous);

    }
    return false;
}

void execute(vector<string> arguments, bool isAsynchronous) {

    if (isAsynchronous) {
        signal(SIGCHLD, signalHandler);
    }

    pid_t pid;
    int status;

    vector<char*> convertedArguments;

    transform(arguments.begin(), arguments.end(), back_inserter(convertedArguments), convert);

    pid = fork();
    if (pid == 0) {
        if (execvp(convertedArguments[0], &convertedArguments[0]) < 0) {
            perror(convertedArguments[0]);
        }

    } else if (pid < 0) {
        perror(convertedArguments[0]);
    } else {
        if (!isAsynchronous) {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}

static void signalHandler(int signal) {
    pid_t chpid = wait(NULL);
    lg.log(chpid);
}

char *convert(const string &s) {
   char *pc = new char[s.size()+1];
   strcpy(pc, s.c_str());
   return pc;
}
