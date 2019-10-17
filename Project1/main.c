/*
Description:
    CIS 415 Project 1 main file. Calls to command.c and shell.c to implement all pseudo-shell functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    N/A
*/

#include <stdio.h>
#include <string.h>
#include "shell.h"
#include "command.h"

int main(int argc, char *argv[]) {
    if ((argc == 2) || (argc > 3)) {
        printf("Usage: pseudo-shell [-f filename]\n");
    }
    else if (argc == 1) {
        shellInteractive();
    }
    else if (argc == 2) {
        if (strcmp(argv[1], "-f")) {
            printf("Usage: pseudo-shell [-f filename]\n");
        }
        else {
            shellFile(argv[2]);
        }
    }

    return 0;
}