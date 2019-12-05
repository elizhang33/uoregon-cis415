/*=============================================================================
 * Program Name: lab9
 * Author: Jared Hall, Joseph Goh
 * Date: 2019/12/05
 * Description:
 *     A simple program that implements a thread-safe queue of meal tickets
 *
 * Notes:
 *     1. DO NOT COPY-PASTE JARED'S CODE INTO MY PROJECT.
 *===========================================================================*/

//========================== Preprocessor Directives ==========================
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
//=============================================================================

//================================= Constants =================================
#define MAXNAME 15
#define MAXQUEUES 4
#define MAXTICKETS 3
#define MAXDISH 20
#define MAXPUBs 3
#define MAXSUBS 4
//=============================================================================

//============================ Structs and Macros =============================
typedef struct mealTicket{
	int ticketNum;
	char *dish;
} mealTicket;

typedef struct MTQ {
	char name[MAXNAME];
	struct mealTicket *buffer;
	int head;
	int tail;
	int length;
	int ticket;
	pthread_mutex_t mutex;
} MTQ;

typedef struct pubJob {
	mealTicket pubMT[MAXDISH];
	char *pubMTQ_ID[MAXDISH];
} pubJob;

typedef struct subJob {

} subJob;

//TODO: If you use the null ticket trick then add a null ticket to the end of the buffer.
//      If you use the heads pos-1 to determine empty/full case then you can skip this.
void init(int pos, char *MTQ_ID) {
	strcpy(registry[pos].name, MTQ_ID);
	registry[pos].buffer = (struct mealTicket *) malloc(sizeof(struct mealTicket) * (MAXTICKETS));
	registry[pos].head = -1;
	registry[pos].tail = 0;
	registry[pos].length = MAXTICKETS;
	registry[pos].ticket = 0;
	pthread_mutex_init(&registry[pos].mutex, PTHREAD_MUTEX_ERRORCHECK_NP);
}

void freeMTQ(int pos, char *MTQ_ID) {
	free(registry[pos].buffer);
}
	
int ticket = 1;

MTQ registry[MAXQUEUES]; //INFO: Changed to be an array of structs
//=============================================================================

//================================= Functions =================================
int enqueue(char *MTQ_ID, mealTicket *MT) {
	int ret = 0; 
	int i, flag = 0;
	
	//Step-1: Find registry
	for(i=0;i<MAXQUEUES;i++) { 
		if(strcmp(MTQ_ID, registry[i].name) == 0) { flag = 1; break; }
	}

	//STEP-2: Enqueue the ticket
	if(flag) {
		while (1) {
			if (pthread_mutex_trylock(&registry[i].mutex) == 0) {
				break;
			}
			else {
				sched_yield();
			}
		}
		int tail = registry[i].tail;
		if(registry[i].head != tail) {
			MT->ticketNum = ticket;
			ticket++;
			registry[i].buffer[tail] = *MT;
			if(tail == registry[i].length - 1) { registry[i].tail = 0; }
			
			else { registry[i].tail++; }
			ret = 1;
		}
		pthread_mutex_unlock(&registry[i].mutex);
	}
	return ret;
}

int dequeue(char *MTQ_ID, int ticketNum, mealTicket *MT) {
	int ret = 0;
	int i, flag = 0;
	
	//Step-1: Find registry
	for(i=0;i<MAXQUEUES;i++) { 
		if(strcmp(MTQ_ID, registry[i].name) == 0) { flag = 1; break; }
	}
	
	//Step-2: Dequeue the ticket
	if(flag) {
		while (1) {
			if (pthread_mutex_trylock(&registry[i].mutex) == 0) {
				break;
			}
			else {
				sched_yield();
			}
		}
		int head = registry[i].head;
		int tail = registry[i].tail;

		if (head == -1) {
			return 0;
		}
		else if (ticketNum == registry[i].buffer[head].ticketNum) {
			//copy the ticket
			MT->ticketNum = registry[i].buffer[head].ticketNum;
			strcpy(MT->dish, registry[i].buffer[head].dish);
			
			//change the head to -1 and tail to 0 to signify empty queue if we popped the last ticket
			if (head + 1 == tail) { 
				registry[i].head = -1;
				registry[i].tail = 0;
			}
			//otherwise increment the head
			else { 
				if (head == registry[i].length - 1) {
					registry[i].head = 0;
				}
				else {
					registry[i].head++;
				}
			}			
			ret = 1;
		}
		pthread_mutex_unlock(&registry[i].mutex);
	}
	return ret;
}

void *publisher(mealTicket to_push[], ) {
	/* TODO: The publisher will receive the following in the struct args:
	*        1. An array of mealTickets to push to the queue.
	*        2. For each meal ticket, an MTQ_ID. (Note: a publisher can push to multiple queues)
	*        3. The thread ID
	* The publisher will then print its type and thread ID on startup. Then it will push a meal ticket to
	* its appropriate queue before sleeping for 1 second. It will do this until there are no more meal tickets to push.
	*/
}

void *subscriber(void *args) {
	/* TODO:The subscriber will take the following:
	* 1. The MTQ_ID it will pull from.
	* 2. The thread ID
	* 3. An empty meal ticket.
	* The subscriber will print its type and thread ID on startup. Then it will pull a ticket from its queue
	* and print it. If the queue is empty then it will print an empty message along with its
	* thread ID and wait for 1 second. If the thread is not empty then it will pop a ticket and 
	* print it along with the thread id.
	*/
}
//=============================================================================

//=============================== Program Main ================================
int main(int argc, char argv[]) {
	//Variables Declarations
	char *qNames[] = {"Breakfast", "Lunch", "Dinner", "Bar"};
	char *bFood[] = {"Eggs", "Bacon", "Steak"};
	char *lFood[] = {"Burger", "Fries", "Coke"};
	char *dFood[] = {"Steak", "Greens", "Pie"};
	char *brFood[] = {"Whiskey", "Sake", "Wine"};
	int i, j, t = 0;
	int test[4];
	char dsh[] = "Empty";
	mealTicket bfast[3] = {[0].dish = bFood[0], [1].dish = bFood[1], [2].dish = bFood[2]};
	mealTicket lnch[3] = {[0].dish = lFood[0], [1].dish = lFood[1], [2].dish = lFood[2]};
	mealTicket dnr[3] = {[0].dish = dFood[0], [1].dish = dFood[1], [2].dish = dFood[2]};
	mealTicket br[3] = {[0].dish = brFood[0], [1].dish = brFood[1], [2].dish = brFood[2]};
	mealTicket ticket = {.ticketNum=0, .dish=dsh};
	pthread_attr_t pubSubAttr;
	pthread_attr_init(&pubSubAttr);
	
	//STEP-1: Initialize the registry
	for (i = 0; i < MAXQUEUES; i++) {
		init(i, qNames[i]);
	}
	
	//STEP-2: Create the publisher thread-pool
	for (j = 0; j < MAXPUBs; j++) {
		pthread_create(NULL, &pubSubAttr, publisher,)
	}
	
	//STEP-3: Create the subscriber thread-pool

	//STEP-4: Join the thread-pools
	
	//STEP-5: Free the registry
	
	return EXIT_SUCCESS;
}
//=============================================================================