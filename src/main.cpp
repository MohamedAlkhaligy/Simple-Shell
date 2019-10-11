#include "main.h"
#include "shell_core.h"
#include <iostream>

using namespace std;

main::main()
{
    //ctor
}

main::~main()
{
    //dtor
}

/**
* Start the shell.
*/
int main()
{

    shell_core shell;
    shell.initializeShell();

    return EXIT_SUCCESS;
}
