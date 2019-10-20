/*
Description:
    CIS 415 Project 1 command.c file. Contains function definitions for syscall functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    N/A
*/

#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <linux/limits.h>
#include "command.h"

// For the ls command
void listDir() {
    char *cwd = NULL;
    char *dirbuf = NULL;
    size_t *bufsize = sizeof(char) * 200;
}

// For the pwd command
void showCurrentDir() {
    char *cwd = NULL;
    getcwd(cwd, 0);
    write(1, cwd, PATH_MAX);
    free(cwd);
}

// For the mkdir command
void makeDir(char *dirName) {

}

// For the cd command
void changeDir(char *dirName) {

}

// For the cp command
void copyFile(char *sourcePath, char *destinationPath) {

}

// For the mv command
void moveFile(char *sourcePath, char *destinationPath) {

}

// For the rm command
void deleteFile(char *filename) {

}

// For the cat command
void displayFile(char *filename) {

}