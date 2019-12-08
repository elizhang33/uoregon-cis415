/*
Description:
    CIS 415 Project 3 InstaQuack!
    quacker.h

Author: Joseph Goh
Last updated: 2019/12/08

Notes:
    N/A

TO DO:
    N/A
*/

#ifndef QUACKER_H_
#define QUACKER_H_

#define _XOPEN_SOURCE 700

#include <pthread.h>
#include <sys/time.h>
#include "topicstore.h"

#define NUMPROXIES 5
#define MAXPUBS 20
#define MAXSUBS 20

// Thread pool struct for pubPool and subPool
typedef struct proxyPool {
    int numFiles;
    pthread_t threads[NUMPROXIES];
    int isFree[NUMPROXIES];
    int nextFile;
    pthread_mutex_t lock;
} proxyPool;

// Publisher proxy thread function
int pubProxy(char *pubFiles);

// Subscriber proxy thread function
int subProxy(char *subFiles);

// Old entry clean-up thread function
int clean(suseconds_t *delta);

// Initialize pubPool
int initPubPool(proxyPool *pubPool, char *pubFiles);

// Initialize subPool
int initSubPool(proxyPool *subPool, char *subFiles);

// Wait for proxy thread termination then destroy mutex lock
int destroyPool(proxyPool *pool);

// Parse the commands from stdin and store jobs in appropriate variables
int cmdParse(char *pubFiles, char *subFiles, suseconds_t *delta);

int quacker();

#endif
