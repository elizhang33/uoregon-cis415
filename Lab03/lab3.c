/*
* Description: CIS 415 Lab 3 - 
*
* Author: Joseph Goh (skeleton and main() was pre-provided)
*
* Date: 2019/10/17
*
* Notes: 
* 1. <add notes we should consider when grading>
*/

/*-------------------------Preprocessor Directives---------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <linux/limits.h>
#include "command.h"
/*---------------------------------------------------------------------------*/

/*----------------------------Function Definitions---------------------------*/
void lfcat()
{
	/* Define your variables here */
	char *cwd_name;
	DIR *cwd;
	struct dirent *current_entry;
	FILE *current_stream;
	FILE *output;
	char *buffer = NULL;
	size_t bufsize = sizeof(char) * 200;
	
	/* Open the file output.txt for writing */
	output = fopen("output.txt", "w");

	/* Get the current directory*/
	getcwd(cwd_name, PATH_MAX);
	
	/* Open the dir using opendir() */
	cwd = opendir(cwd_name);
	
	current_entry = readdir(cwd);
	/* use a while loop to read the dir */
	while(current_entry != NULL) {
		/* Hint: use an if statement to skip any names that are not readable files (e.g. ".", "..", "lab-3.c", "lab3.exe", "output.txt" */
		if ((current_entry->d_name != ".") || (current_entry->d_name != "..") 
		|| (current_entry->d_name != "lab.c") || (current_entry->d_name != "lab3.exe") || (current_entry->d_name != "output.txt")) {
			/* Open the file */
			current_stream = fopen(current_entry->d_name, "r");

			while (!feof(current_stream)) {
				/* Read in each line using getline() */
				getline(&buffer, &bufsize, current_stream);
				/* Write each line to output.txt */
				fputs(buffer, output);
			}
			
			/* print 80 "-" characters to output.txt */
			for (int i = 0; i <80; i++) {
				fputc('-', output);
			}
			
			/* close the read file and free/null assign your line buffer */
			fclose(current_stream);
			free(buffer);
		}

		current_entry = readdir(cwd);
	}

	/*close both the output file and the directory you were reading from using closedir() and fclose() */
	closedir(cwd);
	fclose(output);

	free(cwd_name);
	free(output);
}

/*---------------------------------------------------------------------------*/
/*-----------------------------Program Main----------------------------------*/
int main() {
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
		
		if (!strcmp(buffer, "exit\n") || !strcmp(buffer, "exit")) {
			exit = 1;
		}
		else if (!strcmp(buffer, "lfcat\n") || !strcmp(buffer, "lfcat")) {
			lfcat();
		}
		else {
			printf("Error: Command not recognized\n");
		}
	}
	
	// Free the allocated memory
	free(buffer);

	return 0;
}
/*-----------------------------Program End-----------------------------------*/
