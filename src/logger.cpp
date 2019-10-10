#include "logger.h"
#include <fstream>
#include <iostream>

using namespace std;

logger::logger()
{
    //ctor
}

logger::~logger()
{
    //dtor
}

fstream file;
void logger::log(int ID) {
    file.open ("log_file.txt", ios::in | ios::out | ios::app);
    if (file.is_open()) {
        file << "Child process was terminated. ID: " + to_string(ID) << endl;
    }
}
