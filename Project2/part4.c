/*
Description:
    CIS 415 Project 2 MCP Part 4 file part4.c

Author: Joseph Goh
Date: 2019/11/10

Notes: 
    A lot of the progress indicator/debug prints are commented out 
    in order to make room for the process information display.

TO DO:
    1. Actually figure out what information I'd like to display and how frequently
    2. Implement it
    3. Be happy that I'm done!
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

void alrm_handler(int signum) {
    /*
    if (signum == SIGALRM) {
        write(STDOUT_FILENO, "DEBUG: Parent received SIGALRM\n", 32);
    }
    */
    return;
}

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
    
    // Part 2: sigset_t for sigwait function to catch SIGUSR1
    sigset_t set_usr1, set_old;
    
    int i = 0, j = 0, sig, escape = 0;

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
            // printf("DEBUG: Child (PID %d) waiting to receive SIGUSR1 from parent before exec\n", getpid());
            i = sigwait(&set_usr1, &sig);
            /*
            if (i == 0) {
                printf("DEBUG: Child (PID %d) has received SIGUSR1 from parent!\n", getpid());
            }
            */

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
        // printf("DEBUG: Parent (PID %d) sending SIGUSR1 to child (PID %d)\n", parent_pid, pidv[i]);
        kill(pidv[i], SIGUSR1);
    }

    // Part 3: Send SIGSTOP (but not SIGCONT yet) to all children that are still alive
    sleep(1);
    for (i = 0; i < numprograms; i++) {
        // printf("DEBUG: Parent (PID %d) sending SIGSTOP to child (PID %d)\n", parent_pid, pidv[i]);
        kill(pidv[i], SIGSTOP);
    }

    // Part 4: Set up variables and whatnot for reading stuff from /proc
    // FIXME

    // Part 3: struct sigaction for SIGALRM handling
    struct sigaction alrm_action = {0};
    alrm_action.sa_handler = &alrm_handler;
    sigaction(SIGALRM, &alrm_action, NULL);
    
    // Part 3: Replace original termination check loop with scheduler loop
    while (escape == 0) {
        // Collect zombie processes each loop so that kill can return accurate checks
        waitpid(-1, NULL, WNOHANG);
        // Escape flag is set to true each loop and will be set to false (0) if at least one child is still alive
        escape = 1;
        for (i = 0; i < numprograms; i ++) {
            // If the child pidv[i] is still alive...
            if (kill(pidv[i], 0) != -1) {
                // resume it for one second
                kill(pidv[i], SIGCONT);
                alarm(1);
                // printf("DEBUG: Parent (PID %d) resumed suspended child (PID %d)\n", parent_pid, pidv[i]);
                pause();
                // Check once more whether the child is alive after our time is up
                if (kill(pidv[i], 0) != -1) {
                    kill(pidv[i], SIGSTOP);
                    // printf("DEBUG: Parent (PID %d) suspended unfinished child (PID %d)\n", parent_pid, pidv[i]);
                    // Set the escape flag to false since we're not done
                    escape &= 0;
                }
            }
        }
        // Part 4: Display info about running processes every n loops
        if(j % 2) {
            // FIXME
        }
        j++;
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