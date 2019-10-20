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
    char cwd[PATH_MAX];
    DIR *cwd_dir;
    struct dirent *dir_item;

    getcwd(cwd, sizeof(cwd));
    cwd_dir = opendir(cwd);

    dir_item = readdir(cwd_dir);
    while (1) {
        write(STDOUT_FILENO, dir_item->d_name, strlen(dir_item->d_name));
        dir_item = readdir(cwd_dir);
        if (dir_item == NULL) {
            break;
        }
        else {
            write(STDOUT_FILENO, " ", sizeof(char));
        }
    }
    write(STDOUT_FILENO, "\n", sizeof(char));

    closedir(cwd_dir);

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
    int success;
    char failure_message[] = "Error! Failed to open directory: ";
    success = chdir(dirName);

    if (success == -1) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, dirName, sizeof(dirName));
        write(STDOUT_FILENO, "\n", sizeof(char));
    }

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