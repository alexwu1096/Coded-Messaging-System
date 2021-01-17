/*  Queues.c - Implementation for the queue functions
 *  By: Alex Wu, Garnet Koebel
 *  
 */

 /* 
  *	Program Name: 			    Queues.c
  *	Program Authors: 			Alex Wu, Garnet Koebel
  *	Program Version: 		    V0.2
  *	Program Header Update:		2020-11-29
  *	Program Description:		This file defines all functions related to Queue's and Queue manipulaton
  * 
 */

#include "Queues.h"

static link pHead, pTail;

/*
         *	Function Name:          
         *	Function Author:        Garnet Koebel
         *	Function Type:          void
         *	Function Inputs:        none
         *	Function Outputs:       
         *  Error Handling:         No
         *	Function Description:   
        */

//Super Functions

int getQueueMsg(queue* targetQueue, char* output[]) { //mainly useful for debugging
    /*
         *	Function Name:
         *	Function Author:        Garnet Koebel
         *	Function Type:          int
         *	Function Inputs:        none
         *	Function Outputs:
         *  Error Handling:         Yes
         *	Function Description:
        */
    
    queue tempQueue = deQueue(&targetQueue); //get deQueued element
    

    if (tempQueue != NULL) {
        Item* msg;

        msg = tempQueue->dataStruct;    //get datastruct from de-queued element
        strcpy(output, &msg->message);    //retrieve msg and write to output array
        return 0; //function successful return 0

    } else {
        return 1; //input was NULL return 1

    }

}

int getQueueEntry(queue* targetQueue, Item* output) { //Use this one for getting entry data
    /*
         *	Function Name:
         *	Function Author:        Garnet Koebel
         *	Function Type:          int
         *	Function Inputs:        none
         *	Function Outputs:
         *  Error Handling:         Yes
         *	Function Description:
        */

    queue tempQueue = deQueue(targetQueue); //get deQueued element


    if (tempQueue != NULL) {
        Item* entry;

        entry = tempQueue->dataStruct;    //get datastruct from de-queued element
        *output = *entry;    //retrieve msg and write to output array
        return 0; //function successful return 0

    }
    else {
       output->message[0] = '\0'; //null output variable to avoid appearence of deQueue not working (hours were lost)
        targetQueue = NULL; //null input queue

        return 1; //input was NULL return 1
    
    }

}



//Functions


 queue createEmptyQueue() {
     /*
         *	Function Name:          initQueue
         *	Function Author:        Garnet Koebel
         *	Function Type:          void
         *	Function Inputs:        none
         *	Function Outputs:       void* head, pointer to desired element for head of list
         *  Error Handling:         No
         *	Function Description:   This function creates an empty list and adds the head to that list.
        */
    //Item tempItem;
    //tempItem.message[0] = '\0';
    queue tempQueue = createList(); //init empty list
    //addElement(0, &tempItem, &tempQueue, -1, sizeof(Item));
    //tempQueue->dataStruct = malloc(sizeof(Item));
    return tempQueue; //return pointer to head of queue
}

 queue createQueue(Item headData) {
     /*
         *	Function Name:          initQueue
         *	Function Author:        Garnet Koebel
         *	Function Type:          void
         *	Function Inputs:        none
         *	Function Outputs:       void* head, pointer to desired element for head of list
         *  Error Handling:         No
         *	Function Description:   This function creates an empty list and adds the head to that list.
        */
  
     queue temp = createList(); //init empty list
     addElement(0, &headData, &temp, 0, sizeof(Item));  //add head to head
     temp->dataStruct = malloc(sizeof(Item));
     return temp; //return pointer to head of queue
 }

int isQueueEmpty(queue targetQueue) {
    /*
         *	Function Name:          isQueueEmpty
         *	Function Author:        Alex Wu, Garnet Koebel
         *	Function Type:          int
         *	Function Inputs:        linkedList* targetQueue, pointer to head of a queue
         *	Function Outputs:       none
         *  Error Handling:         No
         *	Function Description:   This function returns 1 if targetQueue is NULL and 0 if it is not NULL).
        */
    if (targetQueue == NULL) {
        return 1;
    } else {
        return 0;
    }

   // return (targetQueue->next == NULL);
}

int atQueueTail(queue targetQueue) {
    /*
         *	Function Name:
         *	Function Author:        Garnet Koebel
         *	Function Type:          int
         *	Function Inputs:        queue* targetQueue
         *	Function Outputs:       
         *  Error Handling:         No
         *	Function Description:
        */

    return isQueueEmpty(targetQueue);
    //return (targetQueue->next == NULL);
}

int atQueueHead(queue targetQueue) {
    /*
         *	Function Name:
         *	Function Author:        Garnet Koebel
         *	Function Type:          int
         *	Function Inputs:        queue* targetQueue
         *	Function Outputs:
         *  Error Handling:         No
         *	Function Description:
        */

    return (targetQueue->prev == NULL);

}

void addToQueue(Item toAdd, queue targetQueue) {
    /*
         *	Function Name:          addToQueue
         *	Function Author:        Garnet Koebel, Alex Wu 
         *	Function Type:          void
         *	Function Inputs:        char
         *	Function Outputs:       none
         *  Error Handling:         No
         *	Function Description:   This function adds a character array to the queue
        */

    Item* temp = malloc(sizeof(Item));
    *temp = toAdd;
    addElement(0, temp, targetQueue, -1, sizeof(Item)); //add a message to the queue tail (FIFO) containing the data in temp

}

queue deQueue(queue* targetQueue) {
    /*
         *	Function Name:          deQueue
         *	Function Author:        Garnet Koebel, Alex Wu
         *	Function Type:          link
         *	Function Inputs:        targetQueue, pointer to a queue
         *	Function Outputs:       temp, pointer to a link element
         *  Error Handling:         Yes, can handle if list is empty
         *	Function Description:   This function returns a pointer to the head of a queue
         *                          (stored in a pTemp) then sets the head of the queue to the 
         *                          next message in the queue.
        */

   
    if (isQueueEmpty(*targetQueue)) {
        return (NULL);
    }
    else {
        queue* temp; //Hold the current head of the list
        temp = *targetQueue;        //phead is returned and then incremented to next node in list
        linkedList* currentElement = *targetQueue;
       
            currentElement = currentElement->next; //move temp to new head pointer
            *targetQueue = currentElement; //Point targetQueue to new head pointer
       

            return temp;

       
    }
}

/*int printMessageQueue(queue targetQueue) { //Depracated
    /*
         *	Function Name:          printMessageQueue
         *	Function Author:        Garnet Koebel
         *	Function Type:          void
         *	Function Inputs:        struct node* targetList
         *	Function Outputs:       returns 0 on success, prints queue contetns to screen
         *  Error Handling:         No
         *	Function Description:   This function prints the content of targetList to the screen.
        */
/*
    struct node* currentList = targetQueue;

    printf("\n=== LIST CONTENTS ===\n\n");

    while (currentList->pNext != NULL) {
        printf("Quote Text: %s\n\n", currentList->Data.message);
        currentList = currentList->pNext;
    }
    printf("Quote text %s\n", currentList->Data.message);

    printf("\n===== LIST END =====\n\n");

    return 0;
}*/

//void traverseFromHeadInit() {
//    /*
//         *	Function Name:          traverseFromHeadInit
//         *	Function Author:        Garnet Koebel
//         *	Function Type:          void
//         *	Function Inputs:        none
//         *	Function Outputs:       none
//         *  Error Handling:         none
//         *	Function Description:   This function allows for traverseFromHead to be called from another file.
//        */
//
////    traverseFromHead(pHead);
//}
//
//// Execute a function on each onde in order
//void traverseFromHead(link h, queue targetQueue) {
//    /*
//         *	Function Name:          traverseFromHead
//         *	Function Author:        Alex Wu, Garnet Koebel
//         *	Function Type:          void
//         *	Function Inputs:        link h
//         *	Function Outputs:       none, output is done by visit call
//         *  Error Handling:         No
//         *	Function Description:   This function take a pointer to the head of a list
//         *                          through recursion (and a call to visit) prints the
//         *                          contents of the list from head to tail.
//        */
//
//    if (h == NULL) {
//        return;
//    }
//    visit(h, targetQueue);      // calls 'visit()' before recursive call
//    //traverseFromHead(h->pNext); //call self with h offset to next element
//}
//
//
//void traverseFromTailInit() {
//    /*
//         *	Function Name:          traverseFromTailInit
//         *	Function Author:        Garnet Koebel
//         *	Function Type:          void
//         *	Function Inputs:        none
//         *	Function Outputs:       none
//         *  Error Handling:         No
//         *	Function Description:   This function allows for traverseFromTail to be called from another file.
//        */
//
//    traverseFromTail(pHead);
//}
//
//
//// Execute a function on each node in reverse
//
////void traverseFromTail(link h) {
//    /*
//         *	Function Name:          traverseFromTail
//         *	Function Author:        Alex Wu, Garnet Koebel
//         *	Function Type:          void
//         *	Function Inputs:        link h
//         *	Function Outputs:       none, output done by call to visit
//         *  Error Handling:         Yes, handles getting nULL pointer 
//         *	Function Description:   This function take a pointer to the head of a list
//         *                          through recursion (and a call to visit) prints the
//         *                          contents of the list from tail to head.
//        */
//
//    if (h == NULL) {
//        return;
//    }
//    traverseFromTail(h->pNext); //call self with h offset to next element
//   // visit(h);           // only called after last node is reached
//}

//void visit(link h, queue targetQueue) {
//    /*
//		 *	Function Name:          visit
//		 *	Function Author:        Garnet Koebel
//		 *	Function Type:			void
//		 *	Function Inputs:		link h
//		 *	Function Outputs:		none, prints contents of list
//		 *  Error Handling:		    No	
//		 *	Function Description:	This function takes a pointer to a link element and
//         *                          prints the message contents.
//		*/
//
//    link pTemp; //Hold the current head of the list
//    if (isQueueEmpty(&targetQueue)) {
//        return (NULL);
//    }
//    else {
//        pTemp = h;
//        printf("\n The message in queue is: %s ", pTemp->Data.message); //print message
//    }
//
//    return;
//}
