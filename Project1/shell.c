/*
Description:
    CIS 415 Project 1 shell.c file. Contains function definitions for running shell functionality.
    This includes both interactive and file modes.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Implement parse(), invalid input handling.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"
#include "command.h"

void shellInteractive() {
    setbuf(stdout, NULL);
	
	int exit = 0;
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	size_t bufsize = sizeof(char) * 200;
	
	// Main input loop
	while (!exit) {
		// Print >>> then get the input string
		printf(">>> ");
		getline(&buffer, &bufsize, stdin);
		
		if (!strcmp(buffer, "exit\n")) {
			exit = 1;
		}
		else {
			// Parse the input string
			parse(buffer);
		}
	}
	
	// Free the allocated memory
	free(buffer);

	return;
}

void shellFile(char *fname) {
    setbuf(stdout, NULL);

	FILE *current_stream;
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	size_t bufsize = sizeof(char) * 200;
	
	// Main input loop
	current_stream = fopen(fname, "r");
	while (!feof(current_stream)) {
		getline(&buffer, &bufsize, current_stream);
		// Parse the input string
		parse(buffer);
	}
	fclose(current_stream);
	
	// Free the allocated memory
	free(buffer);

	return;
}

int parse(char *input) {
	const char delim[3] = " \n";
	char *token;
	char *param1, param2;
	
	token = strtok(input, delim);
	while (token != NULL) {
		// Parse the tokens
		// FIXME
	}
}