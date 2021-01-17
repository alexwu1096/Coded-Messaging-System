#include "diagnostic.h"

/**
	FILE* output;

	queue tstQ = createEmptyQueue();
	char tst[MAX_QUOTE_LENGTH];
	Item tmp;
	strcpy(tmp.message, "HELLO");
	addToQueue(tmp, &tstQ);
	//getQueueMsg(tstQ, &tst);
	//printf("STRING: %s\n", tst);
	strcpy(tmp.message, "WORLD");
	addToQueue(tmp, &tstQ);

	Item out;
	out.message['\0'];
	getQueueEntry(tstQ, &out);


	printf("size is %d", sizeof(Item));


	output = fopen("D:\\queue.txt", "w");			//opens the file to be compressed

	if (!output) {								// error checking file
		printf("Unable to open D:\\queue.txt\n");
		return;
	}




	fwrite(&out, sizeof(Item), 1 , output);				// write the contents of the compressed data from array to file







	printf("STRING: %s\n", out.message);




	fclose(output);

	**/

/* //Debugging Tests
	//create a testElement
	quote testQuote;
	char testString[140];
	testString[0] = 'H';
	testString[1] = 'I';
	testString[2] = '\0';
	strcpy(testQuote.quote, testString);
	testQuote.quoteInd = 2;
	testQuote.quoteLen = 3;
	testQuote.quotSInd = 6;


	messageList testList = createMsgList(); //create a list with an empty head element
	replaceElement(2001, &testQuote, &testList, 0);




	//print data
	quote* msg = testList->dataStruct;
	printf("FIRST ELEMENT: %s\n", msg->quote);

	quoteIndices(&testList);

	quote* msg2 = testList->next->next->dataStruct;
	char msg3[140];
	long int index = getIndexFromList(&testList, 1);
	strcpy(msg3, msg2->quote);
	//printf("SECOND ELEMENT: %s\n", );
	getMessageFromFile(getIndexFromList(&testList, 1140));
	 *///End Debugging Tests
/*

	 //Queues Debug Testing
queue testList = createEmptyQueue();
Item testItem;
testItem.message[0] = '\0';
strcpy(testItem.message, "HELLO");
addToQueue(testItem, &testList);

Item* msg = testList->dataStruct;
char cMsg[140];
strcpy(cMsg, msg->message);
//printf("ITEM: %s\n", cMsg);

strcpy(testItem.message, "WORLD");
addToQueue(testItem, &testList);

queue tempQueue = deQueue(&testList); //get deQueued element
msg = tempQueue->dataStruct;
strcpy(cMsg, &msg->message);
printf("ITEM: %s\n", cMsg);

tempQueue = deQueue(&testList); //get deQueued element
msg = tempQueue->dataStruct;
strcpy(cMsg, &msg->message);
printf("ITEM: %s\n", cMsg);

tempQueue = deQueue(&testList); //get deQueued element
if (tempQueue != NULL) {
	msg = tempQueue->dataStruct;
	strcpy(cMsg, &msg->message);
	printf("ITEM: %s\n", cMsg);
}

//End of Queue Debug Tests

*/

void packetCompressionTest() { //Validation function for packet compression
	/*
	 *	Function Name:			packetCompressionTest
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			void
	 *	Function Inputs:		none
	 *	Function Outputs:		none
	 *	Error Handling:			No
	 *	Function Description:	This function contains the minimum testing code for validation of the packet
	 *							compression functions.
	 *
	*/
							   
	//packet compression testing
	Item testItem;
	strcpy(testItem.message, "AHHHHHHHHHHHHHH\0");
	packet tst;
	tst.packetData = testItem;

	packet compressed;
	packet decompressed;

	compressed =  compressPacket(tst);
	printf("Compressed Data: %s\n", compressed.packetData.message);
	decompressed = decompressPacket(compressed);

	Item output;
	output = decompressed.packetData;
	printf("GOT DATA: %s\n", output.message);

	//packetB compression testing
// Item testItem;
// strcpy(testItem.message, "BBBBBBBBBBBBBBHHHHHHHHHHHHHHHHH");
// packetB tst;
// tst.packetData = &testItem;
//
// packetB compressed;
//
// compressed = compressPacketTypeB(tst);
// Item* compItem;
// compItem = compressed.packetData;
// char* compMsg;
// compMsg = compItem->message;
// printf("COMP MSG: %s\n", compMsg);
//
// packetB decompressed;
// decompressed = decompressPacketTypeB(compressed);
// Item* decompItem;
// decompItem = decompressed.packetData;
// char* decompMsg;
// decompMsg = decompItem->message;
// printf("DECOMP MSG: %s\n", decompMsg);
}

void packetEncryptionTest() {

}

void packetErrorDetectText() {


}