/*
* Description: A simple lab showing signal processing
*
* Author: Jared Hall
*         Joseph Goh
*
* Date: 10/21/2019
*       10/31/2019* updated 
* Notes:
* 1. to be done in lab
*/

/*-------------------------Preprocessor Directives---------------------------*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
/*---------------------------------------------------------------------------*/

/*----------------------------Signal Handler---------------------------------*/
void usr_handler(int signum) {
	if (signum == SIGUSR1) {
		printf("Child Process: %d - Received signal: SIGUSR1\n", getpid());
	}
}
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int	main()
{
	//variable definitions
	pid_t  pid, w;
	int wstatus, i;
	i=0;

	// Define SIGUSR1 behavior before we do anything else
	struct sigaction ;
	sigaction(SIGUSR1);

	// Have the pool be a malloc'ed pointer so we don't copy unnecessary data
	pid_t* pool = malloc(sizeof(pid_t) * 5);

	int j;
	for (j = 0; j < 5; ++j) {
		pid = fork();
		// If pid == 0, I am a child process, so don't fork anymore and break
		if (pid == 0) {
			break;
		}
		// If pid == -1, something went wrong so we might as well abort
		else if (pid == -1) {
			printf("DEBUG: Apparently we couldn't fork for some reason...\n");
			exit(-1);
		}
		// Otherwise, I am the parent and will record the returned child pid in my pool
		else if (pid > 0) {
			pool[j] = pid;
		}
	}


	// We want to sigwait before we start this block if we recieve SIGUSR1 
	if (pid == 0) {
		printf("	Child Process: %i - Starting infinite loop...\n", getpid());
		while(1) {
			i++;
			if(i%10000) {
			printf("	Child Process: %i - Running infinite loop...\n", getpid());
			i=0;
			}
		}
	} else {
		//else this is an existing proc i.e. the parent
		printf("Parent Process: %i, Waiting for child to finish...\n", getpid());
		w = waitpid(pid, &wstatus, WUNTRACED | WCONTINUED);
		printf("All child processes joined. Exiting.\n");
	}
}
/*-----------------------------Program End-----------------------------------*/
