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

enum input_type {command, control_code, parameter, null};

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
	if (current_stream == NULL) {
		printf("Failed to open file: %s\n", fname);
	}
	else {
		while (!feof(current_stream)) {
			getline(&buffer, &bufsize, current_stream);
			// Parse the input string
			parse(buffer);
		}
		fclose(current_stream);
	
		// Free the allocated memory
		free(buffer);
	}

	return;
}

int parse(char *input) {
	const char delim[3] = " \n";
	char *token;
	char *param1, param2;
	
	// Get first token
	token = strtok(input, delim);
	while (token != NULL) {
		// Parse the tokens
		// All conditional blocks should advance to the token for the next command
		// unless there is an error in the command currently being parsed
		if (strcmp(token, "ls") == 0) {
			token = strtok(NULL, delim);
			if ((token == NULL) || (strcmp(token, ";") == 0)) {
				listDir();
			}
			else {
				printf("");
			}
		}
		// FIXME
	}
}

enum input_type isCommand(char *token) {
	if ((strcmp(token, "ls") == 0) || (strcmp(token, "pwd") == 0) || (strcmp(token, "mkdir") == 0) || (strcmp(token, "cd") == 0) 
	|| (strcmp(token, "cp") == 0) || (strcmp(token, "mv") == 0) || (strcmp(token, "rm") == 0) || (strcmp(token, "cat") == 0)) {
		return command;
	}
	else if (strcmp(token, ";")) {
		return control_code;
	}
	else if (token == NULL) {
		return null;
	}
	else {
		return parameter;
	}
}