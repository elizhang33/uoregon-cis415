/*
Description:
    CIS 415 Project 3 InstaQuack!
    quacker.h

Author: Joseph Goh
Last updated: 2019/12/07

Notes:
    N/A

TO DO:
    N/A
*/

#ifndef QUACKER_H_
#define QUACKER_H_

#include <pthread.h>
#include <sys/time.h>

#define URLSIZE 281
#define CAPSIZE 281
#define NAMESIZE 20
#define MAXENTRIES 100
#define MAXTOPICS 20

typedef struct topicEntry {
    int entryNum;
    struct timeval timeStamp;
    int pubID;
    char photoURL[URLSIZE]; // URL to photo
    char photoCaption[CAPSIZE]; // photo caption
} topicEntry;

// Each instance of topicQueue stores the topicEntries for a given topic 
typedef struct topicQueue {
    char name[NAMESIZE];
    int entryCtr;
    topicEntry buffer[MAXENTRIES];
    // Set head to -1 (and tail to 0) to signify an empty queue
    int head;
    int tail;
    pthread_mutex_t lock;
} topicQueue;

int buildTQ(char *name, topicQueue *newQueue);

int enqueue(topicEntry *newEntry, topicQueue *TQ);

int getEntry(int lastEntry, topicQueue *TQ, topicEntry *entry);

int dequeue(topicQueue *TQ, suseconds_t delta);

#endif
