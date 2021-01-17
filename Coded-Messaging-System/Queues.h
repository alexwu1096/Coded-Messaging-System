/*  Queues.h - Interface for the queue function
 *  By: Alex Wu, Garnet Koebel
 *  
 */

 /*
  *	Header Name: 			Queues.h
  *	Header Author: 			Alex Wu, Garnet Koebel
  *	Header Version: 		V0.2
  *	Last Header Update:		2020-11-29
  *	Header Description:		This header declares the functions and libraries needed for Queues and Queue manipulation
  *
 */


#ifndef QUEUES_H
#define QUEUES_H

#include <stdlib.h>
#include "linkedList.h"

#define MAX_MESSAGE_LENGTH 140

typedef linkedList* queue;
typedef struct item Item;
typedef Item item;
typedef struct node Node;
typedef Node* link;

struct item           //Custom datatype for items
{
    char message[MAX_MESSAGE_LENGTH];
    char audioSize[96000];
    short int sid;    //Sender ID
    short int rid;    //Receiver ID
    char priority;    //Priority of message
    short int seqNum; //Sequence number of the messsage
    char later[25];   //Character we might use later
};


// Queue function prototypes
queue createEmptyQueue();

queue createQueue(Item headData);
queue deQueue(queue* targetQueue);    //Prints from Queue then removes printed message from Queue

void exportQueue(queue targetQueue);


int getQueueEntry(queue* targetQueue, Item* output);    //De-Queues and grabs all data in entry
int getQueueMsg(queue* targetQueue, char* output[]);    //De-Queues and discards all but message
int isQueueEmpty(queue targetQueue);                    //Tells whether Queue is empty
int atQueueTail(queue targetQueue);                     //Checks if at tail of a queue
int atQueueHead(queue targetQueue);                     //Checks if at head of a queue
               
void addToQueue(Item toAdd, queue targetQueue);    //Adds a messsage to Queue
void traverseFromHeadInit();        //Allows calling traverseFromHead outside of Queues.c
void traverseFromHead(link h);      //Prints all messages in Queue starting from head
void traverseFromTailInit();        //Allows calling traverseFromTail outside of Queues.c
void traverseFromTail(link h);      //Prints all messages in Queue starting from tail
void visit(link h);                 //Prints a single message from Queue (provided by passing in h)

#endif
