/*
Description:
    CIS 415 Project 2 MCP Part 3 file part3.c

Author: Joseph Goh
Date: 2019/11/10

Notes: 
    N/A

TO DO:
    1. Create handler for SIGALRM
    2. Create scheduler loop that will round robin until all children are terminated
*/

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
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
    
    pid_t pid, parent_pid, current_pid;
    int numprograms = 0;
    pid_t pidv[15];
    
    // Part 2: sigset_t for sigwait function and sigaction struct to catch SIGUSR1
    sigset_t set_usr1, set_old;
    
    int i, sig, status, escape = 0;

    parent_pid = getpid();

    // Part 2: Create our sigset_t
    sigemptyset(&set_usr1);
    sigaddset(&set_usr1, SIGUSR1);

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
            // Part 2: Wait to receive SIGUSR1 from parent before exec
            sigprocmask(SIG_BLOCK, &set_usr1, &set_old);
            printf("DEBUG: Child (PID %d) waiting to receive SIGUSR1 from parent before exec\n", getpid());
            i = sigwait(&set_usr1, &sig);
            if (i == 0) {
                printf("DEBUG: Child (PID %d) has received SIGUSR1 from parent!\n", getpid());
            }

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

    // Part 2: Send SIGUSR1 to children to resume them from their sigwait
    sleep(1);
    for (i = 0; i < numprograms; i++) {
        printf("DEBUG: Parent (PID %d) sending SIGUSR1 to child (PID %d)\n", parent_pid, pidv[i]);
        kill(pidv[i], SIGUSR1);
    }

    // Part 3: Send SIGSTOP (but not SIGCONT yet) to all children that are still alive
    for (i = 0; i < numprograms; i++) {
        printf("DEBUG: Parent (PID %d) sending SIGSTOP to child (PID %d)\n", parent_pid, pidv[i]);
        kill(pidv[i], SIGSTOP);
    }
    // Part 3: Replace original termination check loop with scheduler loop
    // FIXME
    while (!escape) {
        escape = 1;
        for (i = 0; i < numprograms; i ++) {
            current_pid = pidv[i];
            waitpid(current_pid, &status, WNOHANG);
            if (WIFEXITED(status) == 0) {
                escape &= 0;
                alarm(1);
                pause();
            }
        }
    }

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