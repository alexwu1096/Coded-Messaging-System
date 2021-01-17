/*
 *	Header Name: 			message.h	
 *	Header Author: 			Garnet Koebel
 *	Header Version: 		V0.2
 *	Last Header Update:		2020-11-29
 *	Header Description:		This header declares the functions and libraries needed for message.c
 *
 *	Updates for V0.1
 *  - Added function declarations
 *  Updates for V0.2
 *  - Updated function declarations
 *	
*/

#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_QUOTE_LENGTH 140 //used in various functions handling the character arrays

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>			// Dynamic memory allocation for arrays that store quote location and length
#include "linkedList.h"		// LinkedList functionality

typedef linkedList* messageList;

typedef struct quoteData { //Datatype for storing quote data
	char quote[140];
	int	 quoteLen; //Quote Length Actual Length
	long int  quoteInd; //Quote Start Index
	long int  quotSInd; //Quote Stop Index
};

typedef struct quoteData quote;


//Function Declarations
messageList createMsgList();

char* getMessageFromList(linkedList** targetList, int targetElement);	//gets message character array of a quote at targetElement in targetLists

int getMessageFromFile(long int targetIndex, char* output[]);							//gets MAX_QUOTE_LENGTH characters of quote at targetIndex in a file
int randNum(int min, int max);											// Function gets a random number between min and max (max is the number of quotes in the file)
int quoteIndices(linkedList** targetList);								//Indexs quote file locations, counts quote lengths, and pulls quote text into a linked list
int addMessage(quote temp, messageList targetList);

long int getIndexFromList(linkedList** targetList, int targetElement);	//gets the file index of a quote at targetElement in targetList

void addBlankMsg(messageList* targetList);


#endif