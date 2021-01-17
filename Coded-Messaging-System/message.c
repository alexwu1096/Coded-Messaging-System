/*
 *	Program Name:
 *	Program Author: 		Garnet Koebel
 *	Program Version: 		V0.2
 *	Last Program Update:	2020-11-29
 *	Program Description:
 *
 *	Updates for V0.1
 *  -Added frandNum function
 *  -Added fquoteIndices
 *  -Added getIndexFromList
 *  -Added getMessageFromFile
 * Updates for V0.2
 * -Standardized naming conventions for functions to camelCase
 *		-> frandNum renamed to randNum
 *		-> GetMessageFromFile renamed to getMessageFromFile
 *		-> fquoteIndices renamed to quoteIndices
 * -Rewrote randNum to fix issue where it would always return the same value withing a program run
 *		-> removed srand() call from function srand() is now called elsewhere
 *		-> removed int r declaration, function now generates and returns a random number in one line
 *		-> added silent error correction for low out of bounds error
 * -Added getMessageFromList function as a companion to getIndexFromFile
 *		-> takes in a pointer to a target list and a target index in that list
 *		-> returns a pointer to a character array containing the message at targetIndex in targetList
 * -Found a bug in getMessageFromList (bug to be squashed later) function ignores %% terminators and continuest to get characters till limit is reached
 * -Added a bunch of comments
 * 
 *
*/

#include "message.h"

int quoteCount = -1; //Final "quote" is actually just the terminating %% of file. Therefore start at -1;

messageList createMsgList() {
	quote tempQuote;
	messageList tempList = createList();
	addElement(0, &tempQuote, &tempList, -1, sizeof(quote));
	tempList->dataStruct = malloc(sizeof(quote));
	return tempList;
}


// Function gets a random number between min and max (max is the number of quotes in the file)
int randNum(int min, int max) {
	/*
		 *	Function Name: random number generator
		 *	Function Author:		Mike Wu, Garnet Koebel
		 *	Function Type:			int
		 *	Function Inputs:		int min, int max
		 *	Function Outputs:		pseudo random integer number
		 *  Error Handling:			Yes, function silently corrects out of bounds max and min values by setting them to the most extreme in bounds value.
		 *	Function Description:	This function takes in a min and max value these values are used to
		 *							set the range for the random number generator. In order to ensure
		 *							randomness srand() must be called at least once in the program before
		 *							using this function.
		 *
		*/
	
	if (max > quoteCount) {		//check if max is out of bounds and silently correct if true.
		max = quoteCount;		//if max out of bounds then set max to highest in bounds value
	}

	if (min < 0) {				//check if min is out of bounds and silently correct if true.
		min = 0;				//if min out of bounds then set min to lowest in bounds value
	}

	return  (rand() % max) + min;  //generate a random number between min and max

}

int quoteIndices(linkedList** targetList) {
	/*
		 *	Function Name:			quoteIndices
		 *  Function Author:		Garnet Koebel
		 *	Function Type:			int
		 *	Function Inputs:		linkedList** targetList (containing quoteData elements)
		 *	Function Outputs:		returns 0 on success, writes data retreived from file directly to targetList
		 *  Error Handling:			No
		 *	Function Description:	This funtion does three things simultaneously. First, this function scans through a (hardcoded) file and
		 *							writes the current file index value whenever currChar and lastChar both equal % (this is the terminator value)
		 *							this index value is written to targetList. Second, this function counts how many characters have been read since
		 *							the last terminator when the next terminator is reached this value is written to targetList.
		 *							Third, while reading characters this function adds every character read to a character array in targetList. When MAX_QUOTE_LENGTH-1 characters have
		 *							been read and a new terminator hasn't been reached yet it adds a \0 (for string termination) to the character array, further characters read will be
		 *							ignored.
		 *
		*/
	
	//printf("INDEXING FILE\n"); //Debug line

	//we're going to put each quote as it's own linked list element
	quote qTemp; //Temp variable for holding quote data
	qTemp.quoteInd = 0;
	qTemp.quoteLen = 0;
	linkedList* currentElement = *targetList; //Get pointer to head of list

	//Create and allocate memory to a temp element to store the data to replace
	linkedList* tempElement = (linkedList*)malloc(sizeof(linkedList)+sizeof(quote));

	//Index file.
	//Step through file line by line. Note lines where %% is present, these are the start and stop index's
	//Write index values to array of quotes (quoteData datatype) (or use a linked list)
	FILE* temp = fopen("FortuneCookies.txt", "r");
	//go character by character till %% is read
	char lastChar = ' ';
	char currChar = ' ';

	int  charCount = 0;
	long int lastIndx = 0; //used for populating stop index value
	int maxCount = 0;

	if (temp != NULL) {
		while (!feof(temp)) {
			//Get character from file
			lastChar = currChar;
			currChar = fgetc(temp);
			//quote* tempQuote = malloc(sizeof(quote));
			//tempQuote = currentElement->dataStruct;

			//Write currChar to quoteList
			if (maxCount != MAX_QUOTE_LENGTH - 1 && currChar != '%') {
				qTemp.quote[maxCount] = currChar;
				//tempQuote->quote[maxCount] = currChar;
				//printf("CHAR: %c\n", tempQuote->quote[maxCount]); //debug
				maxCount++; //Increment counter
			}
			else {
				qTemp.quote[MAX_QUOTE_LENGTH - 1] = '\0';
				//tempQuote->quote[MAX_QUOTE_LENGTH-1] = '\0';
				//printf("STRING: %s\n", tempQuote->quote); //debug
			}

			//printf("SCANNING AT: %d\n", ftell(temp));

			//Check if %%			
			if (currChar == '%' && lastChar == '%') {
				//Write index at current point to array
				//printf("FOUND QUOTE STOP POINT\n"); //debug line
				
				qTemp.quoteInd = ftell(temp);
				//tempQuote->quoteInd = ftell(temp); //write index value to currently selected linkedList element

				//Add new blank element to list
				//addElement(0, &qTemp, targetList, -1, sizeof(quote));
				//addBlankMsg(targetList);
				//quote* tmp = currentElement->next->dataStruct;
				//printf("TAIL: %s\n", tmp->quote);
				if (quoteCount != -1) {
					qTemp.quoteLen = charCount - 2;
					//tempQuote->quoteLen = charCount - 2;		 //write length of quote into quoteList. (-2 subtracts %% from length)
				}

				addMessage(qTemp, targetList);
				//incremente pointer
				currentElement = currentElement->next;

				quoteCount++; //increment quote counter

				charCount = 0; //Reset counter
				maxCount = 0; //Reset counter

				
			}

			
			charCount++; //increment character counter
		}

	}

		fclose(temp);

	return 0;
}


long int getIndexFromList(linkedList** targetList, int targetElement) {

	/*
		 *	Function Name:			getIndexFromList
		 *  Function Author:		Garnet Koebel
		 *	Function Type:			long int
		 *	Function Inputs:		linkedList** targetList, int targetElement
		 *	Function Outputs:		long int
		 *  Error Handling:			No
		 *	Function Description:   This function takes a pointer to a targetList containing quoteData elements as well as
		 *							a targetElement (index) in that list. This function then iterates through targetList until
		 *							it reaches targetElement at this point it accesses the quoteInd value at targetElement and
		 *							returns the value stored there.
		 *
		*/

		//Increment through list till targetElement and return quoteInd.
	linkedList* currentElement = *targetList; //Get pointer to head of list
	//Create and allocate memory to a temp element to store the data to replace
	linkedList* tempElement = (linkedList*)malloc(sizeof(linkedList));

	for (int i = 0; i < targetElement; i++) {
		currentElement = currentElement->next;
	}
	quote* temp = currentElement->dataStruct;
	//printf("INDEX: %d\n", temp->quoteInd); //debug
	return temp->quoteInd;

}

char* getMessageFromList(linkedList** targetList, int targetElement) {
	
	/*
		 *	Function Name:			getMessageFromList
		 *  Function Author:		Garnet Koebel
		 *	Function Type:			char*
		 *	Function Inputs:		linkedList** targetList, int targetElement
		 *	Function Outputs:		char* - pointer to character array at targetElement
		 *  Error Handling:			No
		 *	Function Description:   This function takes a pointer to a targetList containing quoteData elements as well as
		 *							a targetElement (index) in that list. This function then iterates through targetList until
		 *							it reaches targetElement at this point it accesses the quote character array at targetElement and
		 *							returns a pointer to that character array.
		 *
		*/

		//Increment through list till targetElement and return quoteInd.
	linkedList* currentElement = *targetList; //Get pointer to head of list
	//Create and allocate memory to a temp element to store the data to replace
	linkedList* tempElement = (linkedList*)malloc(sizeof(linkedList));

	for (int i = 0; i < targetElement; i++) {
		if (currentElement->next != NULL) {
			currentElement = currentElement->next;
		}
	}

	quote* temp = (quote*)currentElement->dataStruct;
	char msg[140];
	strcpy(msg, temp->quote);
	return msg; //return pointer

}



// Function that gets q random quote from the FortuneCookies file 
int getMessageFromFile(long int targetIndex, char* output[]) {
	//NOTE: edit to allow opening of arbitrary files

	/*
		 *	Function Name:			getMessageFromFile
		 *  Function Author:		Garnet Koebel
		 *	Function Type:			int
		 *	Function Inputs:		long int targetIndex
		 *	Function Outputs:		returns 0 on success. Prints retreived message to screen
		 *	Error Handling:			No
		 *	Function Description:	This function takes a target index for a file pointer as inputs.
		 *							The FortuneCookies.txt file in readonly mode and seeks to the targetIndex
		 *							value offset by +3 (this offset is to get passed the %% terminators and the 
		 *							newline characters). 139 characters are read from the file
		 *
		*/

	//Temp Variables
	char tempArr[MAX_QUOTE_LENGTH]; //declare
	tempArr[0] = '\0'; //init as string
	char currChar = ' ';
	char lastChar = ' ';

	//Open File
	FILE* temp = fopen("FortuneCookies.txt", "r");

	if (temp != NULL) {
		fseek(temp, targetIndex + 2, SEEK_SET); //Seek to quote at target index (+3 gets past terminators to beginning of quote
		//Read MAX_QUOTE_LENGTH characters
		for (int i = 0; i < MAX_QUOTE_LENGTH - 2; i++) {
			if (currChar == '%' && lastChar == '%') {
				tempArr[i] = '\0'; //terminate string before return
				strcpy(output, tempArr);
				return 0; //quote terminator hit end here
			} else {
				tempArr[i] = fgetc(temp);
			}
		}
		tempArr[MAX_QUOTE_LENGTH - 1] = '\0';
		//fclose(temp);	
	}
	fclose(temp);
	//printf("MESSAGE: %s", tempArr); //debug
	strcpy(output, tempArr);
	return 0;
}

void addBlankMsg(messageList* targetList) {
	/*
		 *	Function Name:
		 *	Function Author:        Garnet Koebel
		 *	Function Type:          void
		 *	Function Inputs:        
		 *	Function Outputs:
		 *  Error Handling:         No
		 *	Function Description:
		*/

	void* tempQuote = malloc(sizeof(quote));
	addElement(0, &tempQuote, targetList, -1, sizeof(quote));
	

}


int addMessage(quote temp, messageList targetList) {
	/*
		 *	Function Name:
		 *	Function Author:        Garnet Koebel
		 *	Function Type:          int
		 *	Function Inputs:        
		 *	Function Outputs:
		 *  Error Handling:         No
		 *	Function Description:
		*/

	quote* qTemp = malloc(sizeof(quote));
	*qTemp = temp;

	addElement(0, qTemp, targetList, -1, sizeof(quote));
}