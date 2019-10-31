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
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int	 main()
{
	//variable definitions
	pid_t  pid, w;
	int wstatus, i;
	i=0;
	
	pid = fork();
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
