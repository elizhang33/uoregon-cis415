/*
Description:
    CIS 415 Project 3 InstaQuack!
    quacker.c

Author: Joseph Goh
Last updated: 2019/12/07

Notes:
    0. As a general rule, functions return 1 on success and 0 on failure.

TO DO:
    1. Implement thread safety
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "quacker.h"

int buildTQ(char *name, topicQueue *newQueue) {
    if (strlen(name) >= NAMESIZE) {
        printf("ERROR: Failed to build topicQueue. Name too long.\n");
        return 0;
    }
    strcpy(newQueue->name, name);
    // Queue starts out empty
    newQueue->entryCtr = 1;
    newQueue->head = -1;
    newQueue->tail = 0;

    return 1;
}

int enqueue(topicEntry *newEntry, topicQueue *TQ) {
    int is_full = 0;
    
    int head = TQ->head;
    int tail = TQ->tail;

    // Check whether queue is full and set appropriate tail value
    // Case 1: topicQueue is empty
    if (head == -1) {
        TQ->head = 0;
        TQ->tail = 0;
    }
    // Case 2: topicQueue is NOT full (incrementing the tail wouldn't overwrite the head)
    else if ((tail + 1) % MAXENTRIES == head) {
        if (tail + 1 == MAXENTRIES) {
            TQ->tail = 0;
        }
        else {
            TQ->tail++;
        }
    }
    // Case 3: topicQueue is full (since it's not empty or not-full)
    else {
        is_full = 1;
    }

    // enqueue the topic entry into buffer at updated tail index unless the queue is full
    if (is_full) {
        return 0;
    }
    else {
        TQ->buffer[TQ->tail].entryNum = TQ->entryCtr;
        TQ->entryCtr++;
        gettimeofday(&TQ->buffer[TQ->tail].timeStamp, NULL);
        TQ->buffer[TQ->tail].pubID = newEntry->pubID;
        strcpy(TQ->buffer[TQ->tail].photoURL, newEntry->photoURL);
        strcpy(TQ->buffer[TQ->tail].photoCaption, newEntry->photoCaption);

        return 1;
    }
}

int getEntry(int lastEntry, topicQueue *TQ, topicEntry *entry) {
    int found = 0;
    int numEntries;
    int index;
    int i;

    if (TQ->head <= TQ->tail) {
        numEntries = TQ->tail - TQ->head + 1;
    }
    else {
        numEntries = TQ->tail - TQ->head + MAXENTRIES + 1;
    }
    
    // Case 1: topicQueue is empty
    if (TQ->head == -1) {
        return 0;
    }
    else {
        for (i = 0; i < numEntries; i++) {
            index = (TQ->head + i) % MAXENTRIES;
            if (TQ->buffer[index].entryNum == lastEntry + 1) {
                found = 1;
                break;
            }
        }

        // Case 2: The next entry to get is found in the topicQueue
        if (found) {
            entry->entryNum = TQ->buffer[index].entryNum;
            entry->timeStamp = TQ->buffer[index].timeStamp;
            entry->pubID = TQ->buffer[index].pubID;
            strcpy(entry->photoURL, TQ->buffer[index].photoURL);
            strcpy(entry->photoCaption, TQ->buffer[index].photoCaption);

            return 1;
        }
        // Case 3: topicQueue is not empty but next entry is not found...
        else {
            for (i = 0; i < numEntries; i++) {
                index = (TQ->head + i) % MAXENTRIES;
                if (TQ->buffer[index].entryNum > lastEntry + 1) {
                    found = 1;
                    break;
                }
            }

            // Case 3-ii: The lastEntry + 1 must have been dequeued but we have an entry added after that
            if (found) {
                entry->entryNum = TQ->buffer[index].entryNum;
                entry->timeStamp = TQ->buffer[index].timeStamp;
                entry->pubID = TQ->buffer[index].pubID;
                strcpy(entry->photoURL, TQ->buffer[index].photoURL);
                strcpy(entry->photoCaption, TQ->buffer[index].photoCaption);

                return TQ->buffer[index].entryNum;
            }
            // Case 3-i: Our desired entry is yet to arrive
            else {
                return 0;
            }
        }
    }

    // We're not supposed to reach this part...
    return -1;
}

int dequeue(topicQueue *TQ, suseconds_t delta) {
    struct timeval currenttime, age;
    int numEntries;
    int index;
    int i;

    if (TQ->head <= TQ->tail) {
        numEntries = TQ->tail - TQ->head + 1;
    }
    else {
        numEntries = TQ->tail - TQ->head + MAXENTRIES + 1;
    }
    
    // If topicQueue is empty, we don't dequeue anything
    if (TQ->head == -1) {
        return 0;
    }
    else {
        // Parsing the entries from oldest to newest...
        for (i = 0; i < numEntries; i++) {
            index = (TQ->head + i) % MAXENTRIES;
            timersub(&currenttime, TQ->buffer[index].timeStamp, age);
            // If we hit an entry that's not old enough, all entries after that aren't either so break
            if (age.tv_usec < delta) {
                break;
            }
        }

        // Case 1: The head (and every other entry by extension) is too new to be dequeued
        if (i == 0) {
            return 0;
        }
        // Case 2: Every entry is old enough to dequeue (since the for loop iterated past the tail)
        else if (i == numEntries) {
            TQ->head = -1;
            TQ->tail = 0;
            return 1;
        }
        // Case 3: Dequeue up to [index - 1] by updating the head to index
        else {
            TQ->head = index;
        }
    }

    // We're not supposed to reach this part...
    return -1;
}