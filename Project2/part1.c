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

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "header.h"

int mcp(char *fname) {
    FILE *fptr;
    char *buffer = NULL;
    size_t bufsize = sizeof(char) * 100;
    
    const char delim[3] = " \n";
    char *command, *token;
    char *argv[12];
    
    pid_t pid, parent_pid;
    int numprograms = 0;
    pid_t pidv[15];
    
    int i;
    
    parent_pid = getpid();

    // Open the input file
    fptr = fopen(fname, "r");
    
    // Exit returning -1 if fopen fails
    if (fptr == NULL) {
        printf("ERROR: Failed to open file. Aborting...\n");
        return -1;
    }

    // Parse file and fork + exec appropriately
    while (!feof(fptr)) {
        getline(&buffer, &bufsize, fptr);
        token = strtok(buffer, delim);
        if (token != NULL) {
            command = token;
            argv[0] = token;
        }
        i = 1;
        token = strtok(NULL, delim);
        while(token != NULL && i < 12) {
            argv[i] = token;
            token = strtok(NULL, delim);
            ++i;
        }
        argv[i] = NULL;

        pid = fork();
        if (pid < 0) {
            printf("ERROR: Parent (PID %d) ailed to fork for some reason. Aborting...\n", parent_pid);
            return -1;
        }
        else if (pid == 0) {
            execvp(command, argv);
            // We shouldn't be going here if exec succeeded, so it's an error
            printf("ERROR: Child (PID %d) failed to exec program \"%s\". Child exiting...\n", getpid(), command);
            exit(EXIT_FAILURE);
        }
        else {
            pidv[numprograms] = pid;
            printf("DEBUG: Parent (PID %d) has created child (PID %d)\n", parent_pid, pid);
            numprograms++;
        }
    }
    free(buffer);
    fclose(fptr);

    for (i = 0; i < numprograms; i++) {
        printf("DEBUG: Parent (PID %d) waiting for child (PID %d) to exit...\n", parent_pid, pidv[i]);
        waitpid(pidv[i], NULL, 0);
    }

    printf("DEBUG: All processes are finished! Parent (PID %d) exiting...\n", parent_pid);
    exit(EXIT_SUCCESS);
}

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
    return EXIT_FAILURE;
}