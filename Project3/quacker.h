/*
Description:
    CIS 415 Project 3 InstaQuack!
    quacker.h

Author: Joseph Goh
Last updated: 2019/12/04

Notes:
    N/A

TO DO:
    N/A
*/

#ifndef QUACKER_H_
#define QUACKER_H_

#include <stdio.h>
#include <sys/time.h>

#define URLSIZE 281
#define CAPSIZE 281
#define MAXTOPICS 100

struct topicEntry {
    int entryNum;
    struct timeval timeStamp;
    int pubID;
    char photoURL[URLSIZE]; // URL to photo
    char photoCaption[CAPSIZE]; // photo caption
};

int enqueue(struct topicEntry *newEntry);

int getEntry(int lastEntry, struct topicEntry *entry);

int dequeue();

#endif
