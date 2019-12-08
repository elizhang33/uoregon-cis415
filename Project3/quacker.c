/*
Description:
    CIS 415 Project 3 InstaQuack! Part 2~5
    quacker.c

Author: Joseph Goh
Last updated: 2019/12/08

Notes:
    N/A

TO DO:
    1. Do Part 2
*/

#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include "topicstore.h"
#include "quacker.h"

// Global topicStore
topicStore topics;

int initPubPool(proxyPool *pubPool, char *pubFiles) {
    int i;

    for (i = 0; i < NUMPROXIES; i++) {
        pthread_create(pubPool->threads[i], NULL, pubProxy, pubFiles);
        pubPool->isFree[i] = 1;
    }

    pubPool->nextFile = 0;

    pthread_mutexattr_t mtxattr;
    pthread_mutexattr_init(&mtxattr);
    pthread_mutexattr_settype(&mtxattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&mtxattr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&pubPool->lock, &mtxattr);
    pthread_mutexattr_destroy(&mtxattr);

    return 1;
}

int initSubPool(proxyPool *subPool, char *subFiles) {
    int i;

    for (i = 0; i < NUMPROXIES; i++) {
        // FIX ME: update subProxy argument
        pthread_create(subPool->threads[i], NULL, subProxy, subFiles);
        subPool->isFree[i] = 1;
    }

    subPool->nextFile = 0;

    pthread_mutexattr_t mtxattr;
    pthread_mutexattr_init(&mtxattr);
    pthread_mutexattr_settype(&mtxattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&mtxattr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&subPool->lock, &mtxattr);
    pthread_mutexattr_destroy(&mtxattr);

    return 1;
}

int destroyPool(proxyPool *pool) {
    int i;
    for (i = 0; i < NUMPROXIES; i++) {
        pthread_join(&pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->lock);

    return 1;
}

int cmdParse(char *pubFiles, char *subFiles, suseconds_t *delta) {
    int exit = 0;
    int i;
    char *buffer = NULL;
    char *token;
    char *saveptr;
    size_t bufsize = (size_t) (sizeof(char) * 1000);

    // Topic variables
    int topicID;
    char topicName[NAMESIZE];
    int topicQueueLength;

    // Count number of pubs and subs
    int pubCt = 0;
    int subCt = 0;
    // filename variable
    char *filename;

    int valid;
    while (!exit) {
        getline(buffer, &bufsize, stdin);
        token = strtok_r(buffer, " ", &saveptr);
        valid = 0;
        if (strcmp(token, "create") == 0) {
            token = strtok_r(NULL, " ", saveptr);
            if (strcmp(token, "topic") == 0) {
                token = strtok_r(NULL, " ", saveptr);
                sscanf(token, "%d", &topicID);
                token = strtok_r(NULL, " ", saveptr);
                strcpy(topicName, token);
                token = strtok_r(NULL, " ", saveptr);
                sscanf(token, "%d", &topicQueueLength);

                buildTQ(topicID, topicName, &topics.topics[topics.numTopics]);
                topics.numTopics++;

                valid = 1;
            }
        }
        else if (strcmp(token, "add") == 0) {
            token = strtok_r(NULL, " ", saveptr);
            if (strcmp(token, "publisher" && pubCt <= MAXPUBS) == 0) {
                token = strtok_r(NULL, " ", saveptr);
                filename = (char *) malloc(FILENAME_MAX);
                strcpy(filename, token);
                pubFiles[pubCt] = filename;
                pubCt++;

                valid = 1;
            }
            else if (strcmp(token, "subscriber") == 0 && subCt <= MAXSUBS) {
                token = strtok_r(NULL, " ", saveptr);
                filename = (char *) malloc(FILENAME_MAX);
                strcpy(filename, token);
                subFiles[subCt] = filename;
                subCt++;

                valid = 1;
            }
        }
        else if (strcmp(token, "query") == 0) {
            if (strcmp(token, "topics") == 0) {
                printf("Topic ID\tTopic Name\n");
                for (i = 0; i < topics.numTopics; i++) {
                    printf("%d\t%s\n", topics.topics[i].id, topics.topics[i].name);
                }

                valid = 1;
            }
            else if (strcmp(token, "publishers") == 0) {
                printf("Pub#\tCommand filename\n");
                for (i = 0; i < pubCt; i++) {
                    printf("%d\t%s\n", i, pubFiles[i]);
                }

                valid = 1;
            }
            else if (strcmp(token, "subscribers") == 0) {
                printf("Sub#\tCommand filename\n");
                for (i = 0; i < subCt; i++) {
                    printf("%d\t%s\n", i, subFiles[i]);
                }

                valid = 1;
            }
        }
        else if (strcmp(token, "delta") == 0) {
            strtok_r(NULL, " ", &saveptr);
            sscanf(token, "%d", i);
            *delta = (suseconds_t) (i * 1000);

            valid = 1;
        }
        else if (strcmp(token, "start") == 0) {
            exit = 1;

            valid = 1;
        }

        if (!valid) {
            printf("ERROR: Command not recognized.\n");
        }
    }

    // Place NULL after the last file as an end marker
    pubFiles[pubCt] = NULL;
    subFiles[subCt] = NULL;

    free(buffer);

    return 1;
}

int quacker() {
    topics.numTopics = 0;

    pthread_t cleaner;
    proxyPool pubPool, subPool;
    suseconds_t delta;

    char *pubFiles[MAXPUBS + 1];
    char *subFiles[MAXSUBS + 1];

    cmdParse(pubFiles, subFiles, &delta);

    initPubPool(&pubPool, pubFiles);
    initSubPool(&subPool, subFiles);

    // Create clean-up thread
    pthread_create(&cleaner, NULL, clean, &delta);

    // Wait for pubs and subs to exit and destroy them
    destroyPool(&pubPool);
    destroyPool(&subPool);
    // Then, send sigabrt to the cleaner thread
    pthread_kill(cleaner, SIGABRT);

    // free char pointers in pubFiles and subFiles
    int i = 0;
    while (pubFiles[i] != NULL) {
        free(pubFiles[i]);
        i++;
    }
    i = 0;
    while (subFiles[i] != NULL) {
        free(subFiles[i]);
        i++;
    }

    // Destroy topicQueues in topicStore
    for (i = 0; i < topics.numTopics; i++) {
        destroyTQ(&topics.topics[i]);
    }
}