/*
Description:
    CIS 415 Project 1 command.c file. Contains function definitions for syscall functionality.

Author: Joseph Goh
Date: 2019/10/16

Notes: 
    N/A

TO DO:
    1. Get copyFile() to actually copy the contents of source into dest
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
    char *buffer = malloc(sizeof(char) * 31);
    ssize_t bytes_read;
    int is_dir = -1;
    char *filename = NULL;
    char *dest_new = NULL;
    int source_fd, dest_fd;
    char failure_message[] = "Error! Failed to copy file: ";
    
    // Check if destinationPath is an existing directory
    is_dir = open(destinationPath, __O_DIRECTORY);

    // If destinationPath is indeed an existing directory, close it first
    // then get filename and append it to dest_new
    if (is_dir != -1) {
        close(is_dir);
        filename = strrchr(sourcePath, (int) '/');
        // If the filename is at the end of a directory, we will get a valid pointer
        if (filename != NULL) {
            dest_new = malloc(sizeof(destinationPath) + sizeof(filename));
            strcpy(dest_new, destinationPath);
            strcat(dest_new, filename);
        }
        // Else, sourcePath has no slash in it so we'll add our own. 
        else {
            dest_new = malloc(sizeof(destinationPath) + sizeof(sourcePath) + sizeof(char));
            strcpy(dest_new, destinationPath);
            strcat(dest_new, "/");
            strcat(dest_new, sourcePath);
        }
    }
    // Otherwise, just strcpy the original dest param into new
    else {
        dest_new = malloc(sizeof(destinationPath));
        strcpy(dest_new, destinationPath);
    }

    // Now we start copying into the new path
    source_fd = open(sourcePath, 0);
    dest_fd = open(dest_new, O_CREAT | O_APPEND, S_IRWXU | S_IRWXG | S_IRWXO);
    
    if ((source_fd == -1) || (dest_fd == -1)) {
        write(STDOUT_FILENO, failure_message, sizeof(failure_message));
        write(STDOUT_FILENO, sourcePath, sizeof(sourcePath));
        write(STDOUT_FILENO, "\n", sizeof(char));

        if (source_fd != -1) {
            close(source_fd);
        }
        if (dest_fd != -1) {
            close(dest_fd);
        }
    }
    else {
        bytes_read = read(source_fd, buffer, sizeof(buffer));
        while (bytes_read > 0) {
            write(dest_fd, buffer, strlen(buffer));
            bytes_read = read(source_fd, buffer, sizeof(buffer));
        }
        close(source_fd);
        close(dest_fd);
    }

    free(buffer);
    free(dest_new);

    return;
}

// For the mv command
void moveFile(char *sourcePath, char *destinationPath) {
    int link_success = -1;
    int unlink_success = -1;
    int is_dir = -1;
    char failure_message[] = "Error! Failed to move file: ";
    char *filename = NULL;
    char *dest_new = NULL;
    
    // Check if destinationPath is an existing directory
    is_dir = open(destinationPath, __O_DIRECTORY);

    // If destinationPath is indeed an existing directory, close it first
    // then get filename and append it to dest_new
    if (is_dir != -1) {
        close(is_dir);
        filename = strrchr(sourcePath, (int) '/');
        // If the filename is at the end of a directory, we will get a valid pointer
        if (filename != NULL) {
            dest_new = malloc(sizeof(destinationPath) + sizeof(filename));
            strcpy(dest_new, destinationPath);
            strcat(dest_new, filename);
        }
        // Else, sourcePath has no slash in it so we'll add our own. 
        else {
            dest_new = malloc(sizeof(destinationPath) + sizeof(sourcePath) + sizeof(char));
            strcpy(dest_new, destinationPath);
            strcat(dest_new, "/");
            strcat(dest_new, sourcePath);
        }
    }
    // Otherwise, just strcpy the original dest param into new
    else {
        dest_new = malloc(sizeof(destinationPath));
        strcpy(dest_new, destinationPath);
    }

    link_success = link(sourcePath, dest_new);
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
            unlink(dest_new);
            write(STDOUT_FILENO, failure_message, sizeof(failure_message));
            write(STDOUT_FILENO, sourcePath, sizeof(sourcePath));
            write(STDOUT_FILENO, "\n", sizeof(char));
        }
    }

    free(dest_new);

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
    char *buffer = malloc(sizeof(char) * 31);
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