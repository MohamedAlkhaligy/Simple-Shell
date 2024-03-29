#include "logger.h"
#include <fstream>
#include <iostream>
#include <ctime>

using namespace std;

fstream file;

logger::logger()
{
    file.open ("log_file.txt", ios::in | ios::out | ios::app);
}

logger::~logger()
{
    file.close();
}

/**
* Logs child termination to a log file
*@param ID of the process being terminated.
*/
void logger::log(int ID) {
    if (file.is_open()) {
        time_t currentTime = time(0);
        char* dt = ctime(&currentTime);
        file << dt << "Child process was terminated. ID: " + to_string(ID) << endl;
    }
}
