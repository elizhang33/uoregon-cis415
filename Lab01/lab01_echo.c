/*	CIS 415 Fall 2019
	Lab 01 Task 5
	Author: Joseph Goh
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLENGTH 100

int main() {
	int exit = 0;
	char *input = malloc(sizeof(char) * MAXLENGTH);

	printf("Welcome! I'll echo anything you type up to %d characters.\nEnter \'getmeouttahere\' to exit the program.\n", MAXLENGTH);

	while(!exit) {
		scanf("%s", input);
		if (!strcmp(input, "getmeouttahere")) {
			exit = 1;
		}
		else {
			printf("%s\n", input);
		}
	}

	free(input);

	return 0;
}
