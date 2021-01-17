#include "menu.h"
#include "protocol.h"
#include "crc.h"
#include "Header.h"
#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RS232Comm.h"

// Declare constants, variables and communication parameters
const int BUFSIZE = 140;							// Buffer size
wchar_t COMPORT_Rx[] = L"COM7";						// COM port used for Rx 
wchar_t COMPORT_Tx[] = L"COM6";						// COM port used for Tx 
HANDLE hComRx;										// Pointer to the selected COM port (Receiver)
HANDLE hComTx;										// Pointer to the selected COM port (Transmitter)
int nComRate = 9600;								// Baud (Bit) rate in bits/second 
int nComBits = 8;									// Number of bits per frame
COMMTIMEOUTS timeout;								// A commtimeout struct variable

//char selection;													// Tx or Rx (can run two instances of this program - double click the exe file)
Header txHeader;												// Header transmitted 
Header rxHeader;												// Header received
void* rxPayload = NULL;											// Received payload (buffer) - void so it can be any data type
DWORD bytesRead;												// Number of bytes received


unsigned int iEncStat = 0;

queue testList;


unsigned int iCompStat = 0;	//compression status
unsigned int iCompType = 0; //compression type
unsigned char cCompType = 'R'; //compression type

unsigned int iErrStat = 0; //error detection status
unsigned int iErrType = 0; //error detection type

unsigned int iHeadStat = 1; //header status (i.e enable or disable)




void mainMenu() {

	testList = createEmptyQueue();

	int ver = 1;
	char encStat[] = "Disabled";
	char compStat[] = "Disabled";
	char headStat[] = "Enabled";

		while (1) {
			system("cls");
			char charSelect[3];
			int intSelect = 0;

		printf("Kilo Watt Squared\nCoded Messaging System:\nSystem Version: %d\nCompression Status: %s\nEncryption Status: %s\nHeader Status: %s\n\n", ver, encStat, compStat, headStat);
		if (!iHeadStat) {
			printf("WARNING: Running Headerless!\n\n");
		} 


		if (!iCompStat) {
			printf("WARNING: Running Without Compression Messages Will Take Longer To Send!\n\n");
		}
		if (!iEncStat) {
			printf("WARNING: Running Without Encryption DO NOT Transmit Sensitive Information!\n\n");
		} else {
			printf("WARNING: Our Encryption IS TERRIBLE DO NOT Transmit Sensitive Information!\n\n");

		}


		printf("\n1. Send text message\n");
		printf("2. Receive text message\n");
		printf("3. Check message history\n");
		printf("4. Send audio message\n");
		printf("5. Receive audio message\n");
		printf("6. Change Settings\n");
		printf("7. Diagnostics menu\n"); //disable this by default and require a diagnostic flag to be enabled to see it
		printf("8. Quit\n");
		printf("Type in number for selection: ");
		//fflush(stdin);

			//system("cls");


			scanf("%s", charSelect);
			intSelect = atoi(charSelect);

			switch (intSelect) {

			case 1:
				txTxtMenu();
				break;

			case 2:
				rxTxtMenu();
				break;

			case 3:
				msgHistory();
				break;

			case 4:
				txAudMenu();
				break;

			case 5:
				rxAudMenu();
				break;

			case 6:
				settingsMenu();
				break;

			case 7:
				diagMenu();
				break;

			case 8:
				exit(0);
				break;

			default:

				printf("Invalid selection\n");
				//mainMenu();
				break;
			}
		}
}



void txTxtMenu() {
	
	char charSelect[3];
	int intSelect = 0;
	int crc=0;
	while (1) {
		system("cls");
		charSelect[0] = '\0';
		printf("\n1. Input a custom message\n");
		printf("2. Back\n");
		printf("\nType in number for selection: ");
		//fflush(stdin);

		scanf("%s", charSelect);
		intSelect = atoi(charSelect);

		messageList testMsgList = createMsgList();
		char msg[MAX_MESSAGE_LENGTH];

		//quoteIndices(&testList);


		switch (intSelect) {
		case 1:
			printf("\nInput your message: ");
			
			char temp;
			scanf("%c", &temp);
			fgets(msg, 140, stdin);
	


			//char txPayload[] = "\nHi there this is a great message for you\n"; 	// Payload is a text message in this example but could be any data	

			// Header (sample data type is text but this should work with audio and images as well)
			txHeader.sid = 1;
			txHeader.rid = 2;
			txHeader.payloadSize = strlen(msg) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
			txHeader.compression = 0;									// None
			txHeader.encryption = 0;									// None
			txHeader.payLoadType = 0;									// Text

			transmit(&txHeader, msg, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
			break;
		case 2:
			return 0;
			break;
		}
	}
}

void rxTxtMenu() {
	system("cls");
	printf("Waiting for Message");
	//receive();
	
	
	//Item testItem;
	//testItem.message[0] = '\0';
	

	char charSelect[3];
	int intSelect = 0;
	while (1) {
		system("cls");
		charSelect[0] = '\0';
		printf("\n1. Receive messages\n");
		printf("2. Display received messages\n");
		printf("3. Back\n");
		printf("\nType in number for selection: ");
		//fflush(stdin);

		scanf("%s", charSelect);
		intSelect = atoi(charSelect);

		Item tmp;
		Item rxMsg;


		switch (intSelect) {
		case 1:
			printf("Waiting for Message");
			
			bytesRead = receive(&rxHeader, &rxPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);
			///packet pRec;
			//receiveTextPacket(&pRec); //get packet from COM buffer
			strcpy(rxMsg.message, &rxPayload);
			addToQueue(rxMsg, &testList); //add packet data to queue
			break;

		case 2:
			
			while (!getQueueEntry(&testList, &tmp)) {
				printf("The message in the queue is: %s", tmp.message);
				system("pause");
			}

			break;
		case 3:
			return 0;
			break;
		default:
			break;
		}
	}
}

void msgHistory() {
	printf("Retreiving Message History");

}

void rxAudMenu() {
	packet pRec;
	receiveAudPacket(&pRec);

}

void txAudMenu() {
	Item toTransmit;
	//char temp;
	sendAudioMsg();

	FILE* input;

	input = fopen("D:\\recording.dat", "rb");

	if (!input) {
		printf("Unable to open %s\n", "D:\\recording.dat");
		return;
	}

	char* buffer = NULL;
	size_t size = 0;

	//Get the buffer size
	fseek(input, 0, SEEK_END);				// Go to end of file 
	size = ftell(input);					// Find number of bytes passed

	printf("Size of file: %d bytes\n", size);

	rewind(input);							// Set position of stream to the beginning 

	/* Allocate the buffer (no need to initialize it with calloc) */
	buffer = (malloc((size + 1) * sizeof(*buffer)));		// size + 1 byte for the \0 

	/* Read the file into the buffer */
	fread(buffer, size, 1, input);		// Read 1 chunk of size bytes from fp into buffer 


	buffer[size] = '\0';            //NULL-terminate the buffer

	fclose(input);
	
	
	strcpy(toTransmit.audioSize, &buffer);
	
	//printf("COMP DATA: %d\n", sizeof(toTransmit));
	transmitAudPacket(toTransmit);
	/*
	extern HANDLE hCom;

	initPortTX();

	outputToPortTX(buffer, size);	// Transmit packet
	//Sleep(1000);
	purgePortTX();								// Purge the port
	CloseHandle(hCom);
	*/
	system("pause");
	
	
}

void settingsMenu() {


	char charSelect[3];
	int intSelect = 0;
	
	while (1) {
		system("cls");
		printf("\n1. Set Receive COM Port\n");
		printf("2. Set Transmit COM PORT\n");
		printf("3. Set BAUD Rate\n");
		printf("4. Toggle Compression\n");
		printf("5. Toggle Encryption\n");
		printf("6. Toggle Headers\n");
		printf("7. Save Settings\n");
		printf("8. Reset Settings to Default\n");
		printf("9. Back\n");
		printf("\nType in number for selection: ");

		scanf("%s", charSelect);
		intSelect = atoi(charSelect);

		switch (intSelect) {
		case 1:	//Set transmit COM Port
			printf("Oops this feature isn't implemented yet\n");
			break;


		case 2: //Set receive COM Port
			printf("Oops this feature isn't implemented yet\n");
			break;
		case 3: //Set BAUD Rate
			printf("Oops this feature isn't implemented yet\n");
			break;
		case 4:	//Toggle Compression
			if (iCompStat) {
				printf("Disabling Compression\n");
				iCompStat = 0;
			} else {
				printf("Enabling Compression\n");
				iCompStat = 1;
			}
			break;

		case 5:	//Toggle Encryption
			if (iEncStat) {
				printf("Disabling Encryption\n");
				iEncStat = 0;
			}
			else {
				printf("Enabling Encryption\n");
				iEncStat = 1;
			}
			break;

		case 6:	//Toggle Headers (i.e run old packetless system)
			printf("Oops this feature isn't implemented yet\n");
			if (iHeadStat) {
				printf("Disabling Headers\n");
				iHeadStat = 0;
			} else {
				printf("Enabling Headers\n");
				iHeadStat = 1;
			}
			break;

		case 7: //Save settings (may remove in favour of saving after every change)
			printf("Oops this feature isn't implemented yet\n");
			break;

		case 8:	//Reset settings to default (store in a file)
			printf("Oops this feature isn't implemented yet\n");
			break;

		case 9: //go up a menu level
			return 0;
			break;
		}
	}
	
}

void diagMenu() {
	char charSelect[3];
	charSelect[2] = '\0';
	int intSelect = 0;

	messageList testList = createMsgList();

	quoteIndices(&testList); //preemptivley index file
	

	char msg[MAX_MESSAGE_LENGTH];
	int i = 0;
	int j = 0;
	Item tmp;
	Item rxMsg;

	while (1) {
		system("cls");
		printf("\n1. Transmit Random Message\n");
		printf("2. Transmit Batch Random Messages\n");
		printf("3. Receive Batch Random Messages\n");
		printf("4. Dry Transmission Run\n");
		printf("5. Run Systems Check\n");
		printf("6. Back\n");
		printf("Type in number for selection: ");
		//fflush(stdin);
		scanf("%s", charSelect);
		intSelect = atoi(charSelect);
		

		switch (intSelect) {
		case 1:
			printf("Transmitting Random Message\n");
			getMessageFromFile(getIndexFromList(&testList, randNum(0, 1000)), &tmp.message);
			printf("SENDING RAND MSG: %s\n", tmp.message);
			transmitTextPacket(tmp);
			//printf("The random message is: &s\n");
			break;

		case 2: 
			
			printf("Transmitting Batch Random Messages");
			while (i < 50) {
				transmitRandom();
				i++;
			}
			break;
			

		case 3:
			printf("Receiving Batch Random Messages");
			while (j < 50) {

				packet pRec;
				receiveTextPacket(&pRec); //get packet from COM buffer
				addToQueue(pRec.packetData, &testList); //add packet data to queue
				
			}

			while (!getQueueEntry(&testList, &tmp)) {
				printf("The message in the queue is: %s\n", tmp.message);
			}
			j++;
			break;

		case 4:

			printf("Performing Dry Run\n");
			printf("Dry Run Complete\n\n");

			break;
		case 5:
			printf("Running Systems Check\n");
			printf("Open Outgoing COM....\n");
			printf("Sucess Closing COM\n");
			printf("Open Incoming COM....\n");
			printf("Sucess Closing COM\n");
			printf("System Check Complete\n\n");
			system("pause");
			break;
		case 6:
			return 0;
			break;
		default:

			break;
		}
	}
}