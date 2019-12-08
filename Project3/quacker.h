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
    int nextFile;
    char *files[MAXPUBS];
    pthread_t threads[NUMPROXIES];
    int isFree[NUMPROXIES];
    pthread_mutex_t lock;
} proxyPool;

// Publisher proxy thread function
void pubProxy(proxyPool *pubPool);

// Subscriber proxy thread function
void subProxy(proxyPool *subPool);

// Old entry clean-up thread function
void clean(suseconds_t *delta);

// Initialize pubPool
int initPubPool(proxyPool *pubPool);

// Initialize subPool
int initSubPool(proxyPool *subPool);

// Wait for proxy thread termination then destroy mutex lock
int destroyPool(proxyPool *pool);

// Parse the commands from stdin and store jobs in appropriate variables
int cmdParse(proxyPool *pubPool, proxyPool *subPool, suseconds_t *delta);

// Parse publisher command file and do publisher things
int pubParse(char *fname);

// Parse subscriber command file and do subscriber things
int subParse(char *fname);

int quacker();

#endif
