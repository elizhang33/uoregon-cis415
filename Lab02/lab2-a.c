/*
* Description: <write a brief description of your lab>
*
* Author: Joseph Goh
*
* Date: 2019/10/10
*
* Notes: 
* 1. <add notes we should consider when grading>
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*---------------------------------------------------------------------------*/

/*-----------------------------Program Main----------------------------------*/
int main() {
	setbuf(stdout, NULL);
	
	/* Main Function Variables */
	int exit = 0;
	size_t bufsize = sizeof(char) * 100;
	const char delim[3] = " \n";
	int tokenctr = 0;
	char *token = malloc(sizeof(char) * 20);
	
	/* Allocate memory for the input buffer. */
	char *buffer = malloc(bufsize);
	
	/*main run loop*/
	while (!exit) {
		/* Print >>> then get the input string */
		printf(">>> ");
		getline(&buffer, &bufsize, stdin);
		
		if (!strcmp(buffer, "exit\n")) {
			exit = 1;
		}
		else {
			/* Tokenize the input string */
			token = strtok(buffer, delim);
			while (token != NULL) {
				/* Display each token */
				printf("T%d: %s\n", tokenctr, token);
				token = strtok(NULL, delim);
				++tokenctr;
			}
			tokenctr = 0;
		}
	}
	
	/*Free the allocated memory*/
	free(token);
	free(buffer);

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
