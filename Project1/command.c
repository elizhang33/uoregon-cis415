/*
Description:
    CIS 415 Project 1 command.c file. Contains function definitions for syscall functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Implement listDir()
    2. Implement makeDir()
    3. Implement changeDir()
    4. Implement copyFile()
    5. Implement moveFile()
    6. Implement deleteFile()
    7. Implement displayFile()
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <linux/limits.h>
#include "command.h"

// For the ls command
void listDir() {
    char *cwd = NULL;
    char *dirbuf = NULL;
    size_t bufsize = sizeof(char) * 200;
    
    return;
}

// For the pwd command
void showCurrentDir() {
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    write(STDOUT_FILENO, cwd, strlen(cwd));
    write(STDOUT_FILENO, "\n", sizeof(char));
}

// For the mkdir command
void makeDir(char *dirName) {
    return;
}

// For the cd command
void changeDir(char *dirName) {
    return;
}

// For the cp command
void copyFile(char *sourcePath, char *destinationPath) {
    return;
}

// For the mv command
void moveFile(char *sourcePath, char *destinationPath) {
    return;
}

// For the rm command
void deleteFile(char *filename) {
    return;
}

// For the cat command
void displayFile(char *filename) {
    return;
}