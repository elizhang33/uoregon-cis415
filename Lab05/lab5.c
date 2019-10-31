/*
* Description: A simple lab showing signal processing
*
* Author: Jared Hall
*         Joseph Goh
*
* Date: 10/21/2019
*       10/31/2019* updated 
* Notes:
* 1. Lab desc says second reciept of SIGUSR1 should exit but also that
*    parent should send SIGUSR1 to resume child processes and then SIGINT
*    to terminate all children.
* 2. Is child processes recieving the SIGUSR1 signal before the infinite
*    loop a strict requirement?
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
	sigset_t usr_set;
	sigemptyset(&usr_set);
	sigaddset(&usr_set, SIGUSR1);
	sigaddset(&usr_set, SIGINT);
	if (signum == SIGUSR1) {
		printf("Child Process: %i - Received signal: SIGUSR1\n", getpid());
		sigwait(&usr_set, SIGUSR1);
		exit(EXIT_SUCCESS);
	}
	return;
}
/*---------------------------------------------------------------------------*/

/*----------------------------Signaler---------------------------------*/
void signaler(pid_t *pool, int signum) {
	int i, j;
	for (i = 0; i < 2; ++i) {
		for (j = 0; j < 5; ++j) {
			kill(pool[j], signum);
		}
	}
	for (j = 0; j < 5; ++j) {
		kill(pool[j], SIGINT);
	}
	return;
}
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int	main()
{
	//variable definitions
	pid_t pid, w;
	int wstatus, i;
	i = 0;

	// Define SIGUSR1 behavior before we do anything else
	struct sigaction usr_action;
	usr_action.sa_sigaction = &usr_handler;
	sigaction(SIGUSR1, &usr_action, NULL);

	// Have the pool be a malloc'ed pointer so we don't copy unnecessary data
	pid_t* pool = malloc(sizeof(pid_t) * 5);

	int j, k;
	for (j = 0; j < 5; ++j) {
		pid = fork();
		// If pid == 0, I am a child process, so don't fork anymore and break
		if (pid == 0) {
			break;
		}
		// If pid == -1, something went wrong so we might as well abort
		else if (pid == -1) {
			printf("DEBUG: Apparently we couldn't fork for some reason...\n");
			for (k = 0; k < j; ++k) {
				kill(pool[k], SIGKILL);
			}
			free(pool);
			exit(EXIT_FAILURE);
		}
		// Otherwise, I am the parent and will record the returned child pid in my pool
		else if (pid > 0) {
			pool[j] = pid;
		}
	}

	if (pid != 0) {
		signaler(pool, SIGUSR1);
	}

	// We want the parent to successfully send SIGUSR1 before the below executes
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
