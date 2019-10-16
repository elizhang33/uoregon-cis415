/*
* Description: Lab 02 - lab2-b.c
* Get input from file and display tokens.
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
int main(int argc, char *argv[]) {
	setbuf(stdout, NULL);
	
	/* Main Function Variables */
	FILE *current_stream;
	const char delim[3] = " \n";
	int tokenctr = 0;
	char *token;
	char *buffer = NULL;
	
	/* Allocate memory for the input buffer. */
	size_t bufsize = sizeof(char) * 100;
	
	/*main run loop*/
	for (int i = 1; i < argc; ++i) {
		current_stream = fopen(argv[i], "r");
		while (!feof(current_stream)) {
			getline(&buffer, &bufsize, current_stream);
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
		fclose(current_stream);
	}
	
	/*Free the allocated memory*/
	free(buffer);

	return 1;
}
/*-----------------------------Program End-----------------------------------*/
