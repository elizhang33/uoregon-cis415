/*
Description:
    CIS 415 Project 1 main file. Calls to shell.c to implement all pseudo-shell functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    1. Main shell functionality was factored out in shell.h and shell.c

TO DO:
    N/A
*/

#include <stdio.h>
#include <string.h>
#include "shell.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        shellInteractive();
    }
    else if (argc == 3) {
        if (strcmp(argv[1], "-f")) {
            printf("Usage: pseudo-shell [-f filename]\n");
        }
        else {
            shellFile(argv[2]);
        }
    }
    else {
        printf("Usage: pseudo-shell [-f filename]\n");
    }

    return 0;
}