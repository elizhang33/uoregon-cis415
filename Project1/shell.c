/*
Description:
    CIS 415 Project 1 shell.c file. Contains function definitions for running shell functionality.
    This includes both interactive and file modes.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Factor out tokenizing, parsing and processing of input contents into separate functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

void shellInteractive() {
    setbuf(stdout, NULL);
	
	/* Main Function Variables */
	int exit = 0;
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	
	/* Allocate memory for the input buffer. */
	size_t bufsize = sizeof(char) * 200;
	
	/*main run loop*/
	while (!exit) {
		/* Print >>> then get the input string */
		printf(">>> ");
		getline(&buffer, &bufsize, stdin);
		
		if (!strcmp(buffer, "exit\n")) {
			exit = 1;
		}
		else {
			parse(buffer);
			tokenctr = 0;
			printf("\n");
		}
	}
	
	/*Free the allocated memory*/
	free(buffer);

	return;
}

void shellFile(char *fname) {
    setbuf(stdout, NULL);
	
	/* Main Function Variables */
	FILE *current_stream;
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	
	/* Allocate memory for the input buffer. */
	size_t bufsize = sizeof(char) * 200;
	
	/*main run loop*/
	current_stream = fopen(fname, "r");
	while (!feof(current_stream)) {
		getline(&buffer, &bufsize, current_stream);
		/* Tokenize the input string */
		parse(buffer);
		tokenctr = 0;
	}
	fclose(current_stream);
	
	/*Free the allocated memory*/
	free(buffer);

	return;
}

int parse(char *input) {
	const char delim[3] = " \n";
	char *token;
	
	token = strtok(input, delim);
	while (token != NULL) {
		/* Parse the command */
			// FIXME
	}
}