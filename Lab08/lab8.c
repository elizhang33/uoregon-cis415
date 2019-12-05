/*
Description:
    CIS 415 Lab 8
    lab8.c

Author: Joseph Goh

Last updated: 2019/12/04

Notes:
    N/A

To Do:
    N/A
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 100
#define MAXQUEUES 10

struct mealTicket {
    int ticketNum;
    char *dish;
};

struct MTQ {
    char name[MAXNAME];
    struct mealTicket *const buffer;
    int head;
    int tail;
    const int length;
};

// Global MTQ registry
struct MTQ *registry[MAXQUEUES];

struct MTQ buildMTQ(char *name, int length) {
    struct mealTicket *buffer = malloc(sizeof(struct mealTicket) * length);
    struct MTQ newMTQ = {{name}, buffer, -1, -1, length};

    return newMTQ;
}

void freeBuffer(struct MTQ *MTQ) {
    free(MTQ->buffer);
}

int enqueue(char *MTQ_ID, struct mealTicket *MT) {
    for (int i; i < 4; i++) {
        if (strcmp(MTQ_ID, registry[i]->name) == 0) {
            // If the MTQ is empty, update the head and tail to index zero
            if (registry[i]->tail == -1) {
                registry[i]->head = 0;
                registry[i]->tail = 0;
            }
            // If the tail is after or the same as the head...
            else if (registry[i]->tail >= registry[i]->head) {
                // and if the tail hasn't reached the end of the buffer yet, increment the tail
                if (registry[i]->tail < registry[i]->length - 1) {
                    registry[i]->tail++;
                }
                // and if the tail has reached the end of the buffer...
                else {
                    // and if the head is index 0 (we have no room at the front), fail and return zero
                    if (registry[i]->head == 0) {
                        return 0;
                    }
                    // and if there is room at the front, index zero is the new tail
                    else {
                        registry[i]->tail = 0;
                    }
                }
            }
            // If the tail comes before the head...
            else {
                // and if the tail is just in front of the head, fail and return zero
                if (registry[i]->tail + 1 == registry[i]->head) {
                    return 0;
                }
                // and we still have room until we reach the head, increment the tail
                else {
                    registry[i]->tail++;
                }
            }
            // If we got this far without returning 0, register the new meal ticket at the updated tail index
            registry[i]->buffer[registry[i]->tail] = *MT;
        }
    }
    return 1;
}

int dequeue(char *MTQ_ID, int ticketNum, struct mealTicket *MT) {

}

int main(int argc, char *argv[]) {
    struct MTQ breakfast = buildMTQ("Breakfast", 5);
    struct MTQ lunch = buildMTQ("Lunch", 5);
    struct MTQ dinner = buildMTQ("Dinner", 5);
    struct MTQ bar = buildMTQ("Bar", 5);

    registry[0] = &breakfast;
    registry[1] = &lunch;
    registry[2] = &dinner;
    registry[3] = &bar;


}