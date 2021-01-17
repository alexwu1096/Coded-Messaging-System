/* main.cpp - Client for the Tx/Rx Program the sets up framed data where the size of payload is FLEXIBLE 
 *            as it is given to the receiving side in the Header portion of the frame 
 * Author: Michael A. Galle
 *
 */

#include <Windows.h>    
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "RS232Comm.h"
#include "Header.h"
#include <time.h>			// Needed for srand
#include "message.h"		// Needed for getting list of messages
#include "Queues.h"			// Needed for creating queues
#include "linkedList.h"		// Needed for creating general linkedLists
#include "audio.h"
#include "protocol.h"
#include "huffman.h"

const int BUFSIZE = 140;							// Buffer size
wchar_t COMPORT_Rx[] = L"COM5";						// COM port used for Rx 
wchar_t COMPORT_Tx[] = L"COM3";						// COM port used for Tx 
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

short int stationId = 0x0001;

/*
 *	Program Name:			main.c
 *	Program Author: 		Alex Wu, Garnet Koebel
 *	Program Version: 		V0.1
 *	Last Program Update:	2020-11-29
 *	Program Description:	This program is the testing mainline for Queue manipulation functions.
 *
*/



//First quote will always have a start index of 0

//NOTE: rewrite code so to use custom datatypes again (have a custom create function for each one
//NOTE: 
void transmitRandom();
void msgHistory();
void diagMenu();
void txTxtMenu();
void rxTxtMenu();
void rxAudMenu();
void txAudMenu();
void settingsMenu();


extern unsigned char cCompType;

int main() {

	// Header (sample data type is text but this should work with audio and images as well)
	txHeader.sid = 1;
	txHeader.rid = 1;
	txHeader.compression = 0;									// None
	txHeader.encryption = 0;									// None
	txHeader.payLoadType = 0;									// Text


	srand(time(0));


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
		}
		else {
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



	printf("\n");
	system("pause");
	return(0);


}


void transmitRandom() {
	messageList testList = createMsgList(); //create a list with an empty head element
	Item tmp;
	char msg[MAX_MESSAGE_LENGTH];

	quoteIndices(&testList);

	getMessageFromFile(getIndexFromList(&testList, randNum(0, 1000)), &tmp.message);

	//transmitTextPacket(tmp);
}


void txTxtMenu() {

	char charSelect[3];
	int intSelect = 0;
	int crc = 0;
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
			txHeader.encryption == 1;
			char temp;
			scanf("%c", &temp);
			fgets(msg, 140, stdin);

			char tmp[140];

			int* outLen;
			char encoded[1000];
			if (txHeader.compression == 1) {
				rleEncode(msg, strlen(msg), &encoded, &outLen, 'ESC');			
				if (txHeader.encryption == 1) {
					 strcpy(tmp, encryptDecrypt(encoded, 'Z'));
				}
				txHeader.payloadSize = strlen(&tmp) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
				txHeader.payLoadType = 0;									// Text
				transmit(&txHeader,&tmp, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
				system("pause");
				break;
			}
			else if (txHeader.compression == 2) {
				int compSize = 0;
				compSize = Huffman_Compress(&msg, &encoded, strlen(msg));
				printf("compressed size is: %d\n", compSize);
				encoded[compSize] = '\0';
				if (txHeader.encryption == 1) {
					encryptDecrypt(encoded, 'Z');
				}
				txHeader.payloadSize = compSize + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
				txHeader.payLoadType = 0;									// Text
				transmit(&txHeader, encoded, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
				system("pause");
				break;
			}

			else {
				if (txHeader.encryption == 1) {
					*msg = encryptDecrypt(msg, 'Z');
				}
				txHeader.payloadSize = strlen(msg) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
				txHeader.payLoadType = 0;									// Text

				transmit(&txHeader, msg, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
				system("pause");
				break;
			}

		case 2:
			return 0;
			break;
		}
	}
}


void rxTxtMenu() {
	system("cls");
	printf("Waiting for Message");



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
			char decoded[10000];
			int outLen = 10000;
			bytesRead = receive(&rxHeader, &rxPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);
			
			printf("Header rid is: %d\n", rxHeader.rid);
			printf("Station id is: %d\n", stationId);


			if (rxHeader.rid == 0xFFFF || stationId == rxHeader.rid) {


				printf("Received message is: %s\n", rxPayload);

				if (rxHeader.compression == 1) {
					rleDecode(rxPayload, bytesRead, &decoded, &outLen, 'ESC');
					//printf("Decoded String is: %s\n", decoded);
					strcpy(rxMsg.message, &decoded);
					printf("Decoded message is: %s\n", rxMsg.message);
					addToQueue(rxMsg, &testList); //add packet data to queue
				}

				else if (rxHeader.compression == 2) {

					Huffman_Uncompress(rxPayload, &decoded, bytesRead, bytesRead);
					decoded[bytesRead - 1] = '\0';
					printf("Decoded String is: %s\n", decoded);
					printf("Length of decoded: %d\n", strlen(decoded));
					//strcpy(rxMsg.message, decoded);
					//printf("2nd message is: %s\n", rxMsg.message);
					//addToQueue(rxMsg, &testList); //add packet data to queue
				}

				else {
					strcpy(rxMsg.message, rxPayload);
					addToQueue(rxMsg, &testList); //add packet data to queue
				}
				system("pause");
			}
			else {
				printf("Message not intended for this station. Discarding message...\n");
				system("pause");
				return 0;
			}	
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
	
	bytesRead = receive(&rxHeader, &rxPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);
	if (rxHeader.rid == 0xFFFF || stationId == rxHeader.rid) {

		printf("Length of payload: %d\n", bytesRead);
		//char decoded[96000];
		//int outLen = 96000;

		extern short iBigBuf[];												// buffer
		extern long  lBigBufSize;											// total number of samples
		short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

		char save[2];
		char replay;
		char c;																// used to flush extra input
		FILE* f;


		if (rxHeader.compression == 2) {
			Huffman_Uncompress(rxPayload, iBigBuf, bytesRead, 96500);
			//printf("Decoded String is: %s\n", decoded);
			//printf("Decoded size is: %d\n", strlen(iBigBufNew));


			printf("Would you like to save your audio recording? (y/n): ");
			scanf("%s", &save);
			char saveC = save[0];
			while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
			if ((saveC == 'y') || (saveC == 'Y')) {
				/* Open input file */
				fopen_s(&f, "newRecording.dat", "wb");
				if (!f) {
					printf("unable to open %s\n", "newRecording.dat");
					system("pause");
					return 0;
				}
				printf("Writing to sound file ...\n");
				fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
				fclose(f);
			}

			fopen_s(&f, "newRecording.dat", "rb");
			if (!f) {
				printf("unable to open %s\n", "newRecording.dat");
				system("pause");
				return 0;

			}
			printf("Reading from sound file ...\n");
			fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
			fclose(f);

			InitializePlayback();
			printf("\nPlaying recording from saved file ...\n");
			PlayBuffer(iBigBufNew, lBigBufSize);
			ClosePlayback();
		}
		else {
			InitializePlayback();
			printf("\nPlaying recording from buffer\n");
			PlayBuffer(rxPayload, bytesRead);
			ClosePlayback();
		}
		system("pause");
	}
	else {
		printf("Message not intended for this station. Discarding message...\n");
		system("pause");
		return 0;
	}
}


void txAudMenu() {

	sendAudioMsg();

	FILE* input;

	input = fopen("recording.dat", "rb");

	if (!input) {
		printf("Unable to open %s\n", "recording.dat");
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

	char encoded[100000];

	if (txHeader.compression == 2) {
		int compSize = 0;
		printf("Test\n");
		compSize = Huffman_Compress(buffer, encoded, size);
		printf("compressed size is: %d\n", compSize);
		encoded[compSize] = '\0';
		txHeader.payloadSize = compSize + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
		txHeader.payLoadType = 1;									// audio
		transmit(&txHeader, encoded, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
	}

	else {
		txHeader.payloadSize = size;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
		txHeader.payLoadType = 1;									// audio

		transmit(&txHeader, buffer, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
	}
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
		printf("8. Change Station ID\n");
		printf("9. Reset Settings to Default\n");
		printf("10. Back\n");
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
			if (txHeader.compression) {
				printf("Disabling Compression\n");
				txHeader.compression = 0;
			}
			else {
				char charSelectComp[3];
				int intSelectComp = 0;
				printf("Enabling Compression, choose compression method: \n");
				printf("1. RLE\n");
				printf("2. Huffman\n");
				scanf("%s", charSelectComp);
				intSelectComp = atoi(charSelectComp);
				if (intSelectComp == 1) {
					txHeader.compression = 1;
					printf("Compression saved as RLE\n");
				}
				else {
					txHeader.compression = 2;
					printf("Compression saved as Huffman\n");
				}
			}
			system("pause");
			break;

		case 5:	//Toggle Encryption
			if (txHeader.encryption) {
				printf("Disabling Encryption\n");
				txHeader.encryption = 0;
			}
			else {
				printf("Enabling Encryption\n");
				txHeader.encryption = 1;
			}
			system("pause");
			break;

		case 6:	//Toggle Headers (i.e run old packetless system)
			printf("Oops this feature isn't implemented yet\n");
			if (iHeadStat) {
				printf("Disabling Headers\n");
				iHeadStat = 0;
			}
			else {
				printf("Enabling Headers\n");
				iHeadStat = 1;
			}
			system("pause");
			break;

		case 7: //Save settings (may remove in favour of saving after every change)
			printf("Oops this feature isn't implemented yet\n");
			break;
		case 8: 
			printf("Set station ID\n");
			char charSelectId[3];
			scanf("%s", charSelectId);
			stationId = atoi(charSelectId);
			printf("Station ID set to: %d\n", stationId);
			system("pause");
			break;
		case 9:	//Reset settings to default (store in a file)
			printf("Oops this feature isn't implemented yet\n");
			break;


		case 10: //go up a menu level
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

			strcpy(msg, tmp.message);
			txHeader.payloadSize = strlen(msg) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
			txHeader.payLoadType = 0;									// Text

			transmit(&txHeader, msg, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
			system("pause");
			//printf("The random message is: &s\n");
			break;

		case 2:

			printf("Transmitting Batch Random Messages");
			while (i < 5) {
				getMessageFromFile(getIndexFromList(&testList, randNum(0, 1000)), &tmp.message);
				printf("SENDING RAND MSG: %s\n", tmp.message);

				strcpy(msg, tmp.message);

				txHeader.payloadSize = strlen(msg) + 1;				// Flexible payload size - Send size of payload inside header (payload can be anything) and enough memory will be malloc'd in the receive function
				txHeader.payLoadType = 0;									// Text

				transmit(&txHeader, msg, &hComTx, COMPORT_Tx, nComRate, nComBits, timeout);  // Transmit header and payload
				i++;
			}
			system("pause");
			break;


		case 3:
			printf("Receiving Batch Random Messages");
			while (j < 5) {
				bytesRead = receive(&rxHeader, &rxPayload, &hComRx, COMPORT_Rx, nComRate, nComBits, timeout);

				strcpy(rxMsg.message, rxPayload);
				printf("Received Message is: %s\n", rxMsg.message);
				addToQueue(rxMsg, &testList); //add packet data to queue
				j++;
			}

			system("pause");
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