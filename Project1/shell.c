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

enum input_type {null, control_code, other, ls, pwd, mkdir, cd, cp, mv, rm, cat};

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
	
	FILE *input_stream;
	FILE *output_stream;
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	size_t bufsize = sizeof(char) * 200;
	
	output_stream = freopen("output.txt", "w", stdout);
	
	// Main input loop
	input_stream = fopen(fname, "r");
	if (input_stream == NULL) {
		printf("Failed to open file: %s\n", fname);
	}
	else {
		while (!feof(input_stream)) {
			getline(&buffer, &bufsize, current_stream);
			// Parse the input string
			parse(buffer);
		}
		fclose(input_stream);
	
		// Free the allocated memory
		free(buffer);
	}

	fclose("output.txt");

	return;
}

int parse(char *input) {
	const char delim[3] = " \n";
	char *token;
	enum input_type token_type;
	char *param1, param2;
	
	// Get first token
	token = strtok(input, delim);
	token_type = getInputType(token);

	// Parse the tokens
	while (token != NULL) {
		// All conditional blocks should be advance token for the next command
		// unless there is an error in the current command
		if (getInputType(token) > other) {
			// For commands that take zero 
		}
		// If the token is a control code, we can move on to the next command
		else if (getInputType(token) == control_code) {
			token = strtok(NULL, delim);
			// However, if the line has ended on a control code, error and break
			if (token == NULL) {

			}
		}
		// If the first token in the command is unrecognized, print error and break
		else if (getInputType(token) == other) {
			printf("Error! Unrecognized command: %s\n", token);
			break;
		}
	}
}

enum input_type getInputType(char *token) {
	if (strcmp(token, "ls") == 0) {
		return ls;
	}
	else if (strcmp(token, "pwd") == 0) {
		return pwd;
	}
	else if (strcmp(token, "mkdir") == 0) {
		return mkdir;
	}
	else if (strcmp(token, "cd") == 0)  {
		return cd;
	}
	else if (strcmp(token, "cp") == 0) {
		return cp;
	}
	else if (strcmp(token, "mv") == 0) {
		return mv;
	}
	else if (strcmp(token, "rm") == 0) {
		return rm;
	}
	else if (strcmp(token, "cat") == 0) {
		return cat;
	}
	else if (strcmp(token, ";")) {
		return control_code;
	}
	else if (token == NULL) {
		return null;
	}
	else {
		return other;
	}
}