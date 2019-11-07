/*-----------------------------------------------------------------------------
File name: Lab6.c
Description: A simple lab showing the use of signals.
Author: Jared Hall
        Joseph Goh
-----------------------------------------------------------------------------*/

/*---------------------------Preprocessor Directives-------------------------*/
#define _GNU_SOURCE
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
/*---------------------------------------------------------------------------*/

/*---------------------------------Functions---------------------------------*/
/*---------------------------------------------------------------------------*/

/*--------------------------------Program Main-------------------------------*/
int main(void) 
{
    //variable declarations
    pid_t pid, w;
    int wstatus, eStatus;

    //create a child process
    pid = fork();
    if(pid < 0) {
        perror("Error! could not create a new process.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0) {
        int mypid = getpid();
        int ctr = 0;
        //This code runs in the child process only
	    printf("    Child process: %d - Starting...\n", mypid);
        printf("Child process %d- Still alive after for 0 seconds\n", mypid);
	    //Add your while loop here
        while(ctr < 10) {
            sleep(1);
            ctr++;
            printf("Child process %d- Still alive after for %d seconds\n", mypid, ctr);
        }
	    exit(EXIT_SUCCESS);
    }
    else if(pid > 0) {
        //This code runs in the parent process only
	    printf("Parent process: %d - Sending signals to child...\n", getpid());
	    //Add code to send your signals  in a loop here
        int *childstatus = WCONTINUED;
        sleep(1);
        while(childstatus != WEXITED) {
            printf("Parent process: %d - Sending signal %d to child %d\n", getpid(), SIGSTOP, pid);
            kill(pid, SIGSTOP);
            sleep(3);
            printf("Parent process: %d - Sending signal %d to child %d\n", getpid(), SIGCONT, pid);
            kill(pid, SIGCONT);
            sleep(1);
            waitpid(pid, childstatus, WNOHANG);
        }
	    printf("Parent process: %d - Waiting for child to complete...\n", getpid());
	    w = waitpid(pid, &wstatus, 0);
	    printf("Parent process: %d - Finished\n", getpid());
    }
    
    //exit out of program
    return EXIT_SUCCESS;
}
/*----------------------------------------------------------------------------*/
