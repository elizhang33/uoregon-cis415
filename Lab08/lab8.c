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

#define MAXNAME 100

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