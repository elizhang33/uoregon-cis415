/*
Description:
    CIS 415 Project 2 MCP Part 1 file part1.c

Author: Joseph Goh
Date: 2019/11/09

Notes: 
    N/A

TO DO:
    N/A
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int mcp_success = -1;

    if (argc != 2) {
        printf("ERROR: MCP called with invalid arguments. Exiting...\n");
        return EXIT_FAILURE;
    }
    else {
        mcp_success = mcp(argv[1]);
        if (mcp_success == -1) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}

int mcp(char *fname) {
    FILE *fptr;
    char *buffer = NULL;
    size_t bufsize = sizeof(char) * 100;
    const char delim[2] = " ";
    char *command, *token;
    char *argv[11];
    int pid;
    int numprograms = 0;
    int pidv[15];
    
    // Open the input file
    fptr = fopen(fname, "r");
    
    // Exit returning -1 if fopen fails
    if (fptr == NULL) {
        printf("ERROR: Failed to open file. Aborting...\n");
        return -1;
    }

    while (!feof(fptr)) {
        getline(&buffer, &bufsize, fptr);
        token = strtok(buffer, delim);
        if (token != NULL) {
            command = token;
        }
        int i = 0;
        token = strtok(NULL, delim);
        while(token != NULL) {
            argv[i] = token;
            token = strtok(NULL, delim);
            ++i;
        }
        argv[i] = NULL;

        pid = fork();
        if (pid < 0) {
            printf("ERROR: Failed to fork for some reason. Aborting...\n");
            return -1;
        }
        else if (pid == 0) {
            execvp(command, argv);
            // We shouldn't be going here if exec succeeded, so it's an error
            printf("ERROR: Child process (PID %d) failed to exec. Child exiting...\n", getpid());
            exit(EXIT_FAILURE);
        }
        else {
            pidv[numprograms] = pid;
            printf("DEBUG: Parent process has logged creation of child (PID %d)\n", pid);
            numprograms++;
        }
    }
}