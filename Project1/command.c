/*
Description:
    CIS 415 Project 1 command.c file. Contains function definitions for syscall functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Done!
    2. Done!
    3. Done!
    4. Implement copyFile()
    5. Done!
    6. Done!
    7. Done!
*/

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    int success = -1;
    char failure_message[] = "Error! Failed to create directory: ";
    
    // I have no idea which users should have permission to do what,
    // so everybody gets full permissions, I guess
    success = mkdir(dirName, S_IRWXU | S_IRWXG | S_IRWXO);

    if (success == -1) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, dirName, sizeof(dirName));
        write(STDOUT_FILENO, "\n", sizeof(char));
    }

    return;
}

// For the cd command
void changeDir(char *dirName) {
    int success = -1;
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
    int link_success = -1;
    int unlink_success = -1;
    char failure_message[] = "Error! Failed to move file: ";
    
    link_success = link(sourcePath, destinationPath);
    // If creating new hard link failed, error
    if (link_success == -1) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, sourcePath, sizeof(sourcePath));
        write(STDOUT_FILENO, "\n", sizeof(char));
    }
    else {
        unlink_success = unlink(sourcePath);
        // If unlinking the originial path failed, get rid of the new link so that we don't have two links
        if (unlink_success == -1) {
            unlink(destinationPath);
            write(STDOUT_FILENO, failure_message, sizeof(failure_message));
            write(STDOUT_FILENO, sourcePath, sizeof(sourcePath));
            write(STDOUT_FILENO, "\n", sizeof(char));
        }
    }

    return;
}

// For the rm command
void deleteFile(char *filename) {
    int success = -1;
    char failure_message[] = "Error! Failed to remove file: ";

    success = unlink(filename);

    if (success == -1) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, filename, sizeof(filename));
        write(STDOUT_FILENO, "\n", sizeof(char));
    }

    return;
}

// For the cat command
void displayFile(char *filename) {
    // I don't know why, but setting the buffer size to anything other than
    // 8n - 1 will result in the last character in the buffer breaking...
    char *buffer = malloc(sizeof(char) * 127);
    ssize_t bytes_read;
    int filedesc = -1;
    char failure_message[] = "Error! Failed to open file: ";

    // Hopefully 0 means that I don't set any flags as opposed to setting all of them
    filedesc = open(filename, 0);
    if (filedesc == -1) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, filename, sizeof(filename));
    }
    else {
        bytes_read = read(filedesc, buffer, sizeof(buffer));
        while (bytes_read > 0) {
            write(STDOUT_FILENO, buffer, strlen(buffer));
            bytes_read = read(filedesc, buffer, sizeof(buffer));
        }
    }
    write(STDOUT_FILENO, "\n", sizeof(char));
    close(filedesc);
    free(buffer);

    return;
}