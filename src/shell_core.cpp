#include "shell_core.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>

using namespace std;

vector<string> splitCommand(string commandLine);
string convertToLower(string word);
bool execute(vector<string> arguemnts);

const string EXIT = "exit";


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
        isExit = execute(arguments);

    } while (!isExit);
    exit(0);
}

vector<string> splitCommand(string commandLine) {
    vector<string> arguments;

    string argument = "";
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

bool execute(vector<string> arguments) {
    if (!arguments.empty()) {

        if (convertToLower(arguments[0]) == EXIT) {
            return false;
        }

        for (auto argument : arguments) {
        }
    }
    return false;
}

string convertToLower(string word) {
    transform(word.begin(), word.end(), word.begin(), ::tolower);
    return word;
}
