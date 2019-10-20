/*
Description:
    CIS 415 Project 1 shell.c file. Contains function definitions for running shell functionality.
    This includes both interactive and file modes.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Implement calling of command functions in parse().
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
			getline(&buffer, &bufsize, input_stream);
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

void parse(char *input) {
	const char delim[3] = " \n";
	char *token;
	char *command_str;
	enum input_type command;
	int *paramc;
	char *paramv[2];
	
	// Get first token and make sure we don't start on a control code
	token = strtok(input, delim);
	if (getInputType(token) == control_code) {
		printf("Error! Incorrect syntax. Cannot begin with control code.\n");
		return;
	}

	// Parse the tokens
	while (token != NULL) {
		// If we start the loop with a control code, jump to the next token before parsing
		if (getInputType(token) == control_code) {
			token = strtok(NULL, delim);
			// However, if the line has ended on a control code, error and break
			if ((token == NULL) || (getInputType(token) == control_code)) {
				printf("Error! Unrecognized command: \n");
				break;
			}
		}

		// If the first token is a valid command...
		if (getInputType(token) > other) {
			command_str = token;
			command = getInputType(command_str);
			// Grab up to two parameters until end of line or a control code is reached
			paramc = 0;
			for (int i = 0; i < 2; ++i) {
				token = strtok(NULL, delim);
				if (getInputType(token) == other) {
					paramv[i] = token;
					++paramc;
				}
				else if (getInputType(token) > other) {
					printf("Error! Incorrect syntax. No control code found.\n");
					// Eek! It's a goto!!!
					goto error_break;
				}
			}
			// If we grabbed the maximum of params, advance the token and check for errors
			if (paramc == 2) {
				token = strtok(NULL, delim);
				if (getInputType(token) > other) {
					printf("Error! Incorrect syntax. No control code found.\n");
					break;
				}
				else if (getInputType(token) == other) {
					printf("Error! Unsupported parameters for command: %s\n", command_str);
					break;
				}
			}
			// Call command functions while checking for correct number of parameters
		}
		// Make sure that unless we decided to error and break,
		// the token is advanced for the next stage of the loop

		// If the first token in the command is unrecognized, print error and break
		else if (getInputType(token) == other) {
			printf("Error! Unrecognized command: %s\n", token);
			break;
		}
	}
error_break:
	return;
}

enum input_type getInputType(char *token) {
	if (token == NULL) {
		return null;
	}
	else if (strcmp(token, "ls") == 0) {
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
	else {
		return other;
	}
}