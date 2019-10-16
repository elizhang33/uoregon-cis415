/*
Description:
    CIS 415 Project 1 shell.h file. Contains function declarations for running shell functionality.
    This includes both interactive and file modes.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Factor out tokenizing, parsing and processing of input contents into separate functions.
*/

void shellInteractive();

void shellFile(char *fname);

int parse(char *input);