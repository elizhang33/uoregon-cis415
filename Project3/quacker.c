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
#include <time.h>
#include "topicstore.h"
#include "quacker.h"

// Global topicStore
topicStore topics;

void *pubProxy(void *pubPool_v){
    proxyPool *pubPool = (proxyPool *) pubPool_v;
    int thread_idx, file_idx;

    for (thread_idx = 0; thread_idx < NUMPROXIES; thread_idx++) {
        if (pthread_self() == pubPool->threads[thread_idx]) {
            break;
        }
    }

    // First, lock the pool
    pthread_mutex_lock(&pubPool->lock);

    // If we did all the pub jobs then unlock and exit the thread
    if (pubPool->nextFile >= pubPool->numFiles) {
        pthread_mutex_unlock(&pubPool->lock);
        pthread_exit(NULL);
    }
    // If there is more left to do then increment the counter then unlock as well as calling pubParse
    else {
        file_idx = pubPool->nextFile;
        pubPool->nextFile++;
        pubPool->isFree[thread_idx] = 0;
        pthread_mutex_unlock(&pubPool->lock);

        pubParse(pubPool->files[file_idx]);
    }
    pubPool->isFree[thread_idx] = 1;

    return NULL;
}

void *subProxy(void *subPool_v){
    proxyPool *subPool = (proxyPool *) subPool_v;
    int thread_idx, file_idx;

    for (thread_idx = 0; thread_idx < NUMPROXIES; thread_idx++) {
        if (pthread_self() == subPool->threads[thread_idx]) {
            break;
        }
    }

    // First, lock the pool
    pthread_mutex_lock(&subPool->lock);

    // If we did all the sub jobs then unlock and exit the thread
    if (subPool->nextFile >= subPool->numFiles) {
        pthread_mutex_unlock(&subPool->lock);
        pthread_exit(NULL);
    }
    // If there is more left to do then increment the counter then unlock as well as calling pubParse
    else {
        file_idx = subPool->nextFile;
        subPool->nextFile++;
        subPool->isFree[thread_idx] = 0;
        pthread_mutex_unlock(&subPool->lock);

        subParse(subPool->files[file_idx]);
    }
    subPool->isFree[thread_idx] = 1;

    return NULL;
}

void *clean(void *delta_v) {
    suseconds_t *delta = (suseconds_t *) delta_v;
    int i;
    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 100000000;
    while (1) {
        for (i = 0; i < topics.numTopics; i++) {
            dequeue(&topics.topics[i], *delta);
        }
        nanosleep(&tim, NULL);
    }

    return NULL;
}

int initPubPool(proxyPool *pubPool) {
    int i;

    for (i = 0; i < NUMPROXIES; i++) {
        pthread_create(&pubPool->threads[i], NULL, &pubProxy, (void *) pubPool);
        pubPool->isFree[i] = 1;
    }

    pubPool->numFiles = 0;
    pubPool->nextFile = 0;

    pthread_mutexattr_t mtxattr;
    pthread_mutexattr_init(&mtxattr);
    pthread_mutexattr_settype(&mtxattr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setrobust(&mtxattr, PTHREAD_MUTEX_ROBUST);
    pthread_mutex_init(&pubPool->lock, &mtxattr);
    pthread_mutexattr_destroy(&mtxattr);

    return 1;
}

int initSubPool(proxyPool *subPool) {
    int i;

    for (i = 0; i < NUMPROXIES; i++) {
        pthread_create(&subPool->threads[i], NULL, &subProxy, (void *) subPool);
        subPool->isFree[i] = 1;
    }
    
    subPool->numFiles = 0;
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
        pthread_join(pool->threads[i], NULL);
    }
    pthread_mutex_destroy(&pool->lock);

    for (i = 0; i < pool->numFiles; i++) {
        free(pool->files[i]);
    }

    return 1;
}

int cmdParse(proxyPool *pubPool, proxyPool *subPool, suseconds_t *delta) {
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

    // filename pointer
    char *filename;

    int valid;
    while (!exit) {
        getline(&buffer, &bufsize, stdin);
        token = strtok_r(buffer, " ", &saveptr);
        valid = 0;
        if (strcmp(token, "create") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            if (strcmp(token, "topic") == 0) {
                token = strtok_r(NULL, " ", &saveptr);
                sscanf(token, "%d", &topicID);
                token = strtok_r(NULL, " ", &saveptr);
                strcpy(topicName, token);
                token = strtok_r(NULL, " ", &saveptr);
                sscanf(token, "%d", &topicQueueLength);

                buildTQ(topicID, topicName, &topics.topics[topics.numTopics]);
                topics.numTopics++;

                valid = 1;
            }
        }
        else if (strcmp(token, "add") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            if ( (strcmp(token, "publisher") == 0) && (pubPool->numFiles <= MAXPUBS) ) {
                token = strtok_r(NULL, " ", &saveptr);
                filename = (char *) malloc(FILENAME_MAX);
                strcpy(filename, token);
                pubPool->files[pubPool->numFiles] = filename;
                pubPool->numFiles++;

                valid = 1;
            }
            else if ( (strcmp(token, "subscriber") == 0) && (subPool->numFiles <= MAXSUBS) ) {
                token = strtok_r(NULL, " ", &saveptr);
                filename = (char *) malloc(FILENAME_MAX);
                strcpy(filename, token);
                subPool->files[subPool->numFiles] = filename;
                subPool->numFiles++;

                valid = 1;
            }
        }
        else if (strcmp(token, "query") == 0) {
            if (strcmp(token, "topics") == 0) {
                printf("Topic ID\tLength\n");
                for (i = 0; i < topics.numTopics; i++) {
                    printf("%d\t%d\n", topics.topics[i].id, topics.topics[i].length);
                }

                valid = 1;
            }
            else if (strcmp(token, "publishers") == 0) {
                printf("Pub#\tCommand filename\n");
                for (i = 0; i < pubPool->numFiles; i++) {
                    printf("%d\t%s\n", i, pubPool->files[i]);
                }

                valid = 1;
            }
            else if (strcmp(token, "subscribers") == 0) {
                printf("Sub#\tCommand filename\n");
                for (i = 0; i < subPool->numFiles; i++) {
                    printf("%d\t%s\n", i, subPool->files[i]);
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
    pubPool->files[pubPool->numFiles] = NULL;
    subPool->files[subPool->numFiles] = NULL;

    free(buffer);

    return 1;
}

int pubParse(char *fname) {
    char *buffer = NULL;
    size_t bufsize = (size_t) (sizeof(char) * 200);
    char *token, *saveptr;

    FILE *fptr = fopen(fname, "r");

    int exit = 0;

    struct timespec tim;
    tim.tv_sec = 0;

    int i, index, topicID, success;

    topicEntry newEntry;

    getline(&buffer, &bufsize, fptr);

    token = strtok_r(buffer, " ", &saveptr);

    while (!exit) {
        if (strcmp(token, "put") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            sscanf(token, "%d", &topicID);

            // Find the index of the topicQueue with topicID
            for (index = 0; index < topics.numTopics; index++) {
                if (topics.topics[index].id == topicID) {
                    break;
                }
            }
            // If the desired topic is not found, print an error and abort.
            if (index == topics.numTopics) {
                printf("ERROR: Publisher (%d) could not find topic with ID: %d\n", pthread_self(), topicID);
                return 0;
            }

            token = strtok_r(NULL, " ", &saveptr);
            strcpy(newEntry.photoURL, token);
            token = strtok_r(NULL, " ", &saveptr);
            strcpy(newEntry.photoCaption, token);
            newEntry.pubID = (int) pthread_self();

            // Try to get enqueue to topic 20 times, sleeping for 100 ms after each failed attempt
            for (i = 0; i < 20; i++) {
                success = enqueue(&newEntry, &topics.topics[index]);
                if (success) {
                    break;
                }
                tim.tv_nsec = 100000000;
                nanosleep(&tim, NULL);
            }

            // If we didn't succeed after all those iterations, give up, print an error, and move on
            if (!success) {
                printf("ERROR: Publisher (%d) failed to enqueue new topic entry with ID: %d\n", pthread_self(), topicID);
            }
        }
        else if (strcmp(token, "sleep") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            sscanf(token, "%d", i);
            tim.tv_nsec = (long) (i * 1000000);
            nanosleep(&tim, NULL);
        }
        else if (strcmp(token, "stop") == 0) {
            exit = 1;
        }
    }

    return 1;
}

int subParse(char *fname) {
    char *buffer = NULL;
    size_t bufsize = (size_t) (sizeof(char) * 200);
    char *token, *saveptr;

    FILE *fptr = fopen(fname, "r");

    int exit = 0;

    struct timespec tim;
    tim.tv_sec = 0;

    int i, index, topicID, lastEntry, success = 0;

    topicEntry fetchedEntry;

    getline(&buffer, &bufsize, fptr);

    token = strtok_r(buffer, " ", &saveptr);

    while (!exit) {
        if (strcmp(token, "get") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            sscanf(token, "%d", &topicID);

            // Find the index of the topicQueue with topicID
            for (index = 0; index < topics.numTopics; index++) {
                if (topics.topics[index].id == topicID) {
                    break;
                }
            }
            // If the desired topic is not found, print an error and abort.
            if (index == topics.numTopics) {
                printf("ERROR: Publisher (%d) could not find topic with ID: %d\n", pthread_self(), topicID);
                return 0;
            }
            else {
                // Try to get new entry from the topic 20 times, sleeping for 100 ms after each failed attempt
                for (i = 0; i < 20; i++) {
                    success = getEntry(lastEntry, &topics.topics[index], &fetchedEntry);
                    if (success) {
                        break;
                    }
                    tim.tv_nsec = 100000000;
                    nanosleep(&tim, NULL);
                }

                if (success) {
                    // FIX ME: Change to output to HTML file appropriately
                    printf("Subscriber (%d) got entry no. %d:\nURL: %s\nCaption: %s\n",
                            pthread_self(), fetchedEntry.entryNum, fetchedEntry.photoURL, fetchedEntry.photoURL);
                }
                // If we couldn't succeed for 20 times, give up, print an error and move on.
                else {
                    printf("ERROR: Publisher (%d) failed to get latest topic entry with ID: %d\n", pthread_self(), topicID);
                }
            }
        }
        else if (strcmp(token, "sleep") == 0) {
            token = strtok_r(NULL, " ", &saveptr);
            sscanf(token, "%d", i);
            tim.tv_nsec = (long) (i * 1000000);
            nanosleep(&tim, NULL);
        }
        else if (strcmp(token, "stop") == 0) {
            exit = 1;
        }
    }

    return 1;
}

int quacker() {
    int i;

    topics.numTopics = 0;

    pthread_t cleaner;
    proxyPool pubPool, subPool;
    suseconds_t delta = 1000000;

    initPubPool(&pubPool);
    initSubPool(&subPool);

    cmdParse(&pubPool, &subPool, &delta);

    // Create clean-up thread
    pthread_create(&cleaner, NULL, &clean, (void *) &delta);

    // Wait for pubs and subs to exit and destroy them
    destroyPool(&pubPool);
    destroyPool(&subPool);
    // Then, send sigabrt to the cleaner thread
    pthread_kill(cleaner, SIGABRT);

    // Destroy topicQueues in topicStore
    for (i = 0; i < topics.numTopics; i++) {
        destroyTQ(&topics.topics[i]);
    }
}