/*
* Description: Lab 02 - lab2-a.c
* Get input from console and display tokens.
*
* Author: Joseph Goh
*
* Date: 2019/10/10
*
* Notes: 
* N/A
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
	char *token = NULL;
	
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
				printf("\nT%d: %s", tokenctr, token);
				token = strtok(NULL, delim);
				++tokenctr;
			}
			tokenctr = 0;
			printf("\n");
		}
	}
	
	/*Free the allocated memory*/
	free(token);
	free(buffer);

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
