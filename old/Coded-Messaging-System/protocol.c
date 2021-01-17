/* Name:		Kilo Watt Squared Coded Messaging System Protocol
*  Author:		Garnet Koebel
*  Version:		V0.1
*  Created:		2020-11-26
*  Library Update:	2020-11-26
*  Library Description:	This file contains the definitions for the CMS protocol
*
*/

#include "protocol.h"

#define MAX_PACKET_SIZE 218 //max packet size in bytes

header generateHeader(Item pData) {
	/*
	 *	Function Name:			generatePacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			header
	 *	Function Inputs:		pData (packet data)
	 *	Function Outputs:
	 *	Error Handling:			No
	 *	Function Description:	This function generates a packet header based
	 *							on the contents of pData and the current
	 *							CMS settings
	 *
	*/
	extern unsigned int iCompStat;
	extern unsigned char cCompType;
	
	extern unsigned int iEncStat;
	//extern unsigned char encType;
	


	char cEncCompByte = '\0';			//byte for encrption and compression info
	char cCRCChecksumByte = '\0';		//byte for error detection information
	char cErrDetectandLengthByte = '\0'; //byte for error detection type and  4 bits of # of packets data
	char lengthByte = '\0';			//byte for # of packets
	

	header temp;
	temp.lSignature = 0xDEADBEEF;				//populate signature
	temp.bReceiverAddr = pData.rid;				//populate receiver ID
	temp.bVersion = 1;							//populate protocol version
	temp.lDataLength = sizeof(pData.message);	//populate message size
	temp.bPattern[0] = 0xaa;					//populate bPattern
	temp.bPattern[1] = 0x55;
	temp.bPattern[2] = 0xaa;
	temp.bPattern[3] = 0x55;

	/*	bTBD Structure
		bits 0-3 are encryption information
		bits 4-8 are compression information
		
		bits 9-12 are Error detection / correction information
		bits 13-20 are CRC / Checksum value storage
		bits 21-32 are # of packets encoding
	*/
	//populate bTBD with message information
	/* Theory of Operation
	*  1. Check CMS settings (ex. encryption status = none)
	*  2. Add relevant character to the byte associated with checked setting (i.e no encryption therfore binary should be 0000 0000 so add \0 to encryptionByte)
	*  3. Repeat for remaining settings
	*  4. Encode binary data into header bTBD
	*/

	//DESIGN NOTE: This process could be made much more efficient by writing directly to the header. It could also be made more efficient by adding all data for combined bytes (i.e bytes where the first 4 and last 4 bits are for diffent settings) at the same time (this is possible as there will always be a character with the needed binary number)

	//Encode Encryption Data
	/**if (iEncStat) { //Encrption present
		if (encType == 0) {
			
		} else if (encType == 1) { 
			
		}
	} else { //Encrpytion not Present
		//write 0 to all encryption bits since no encryption present
		cEncCompByte = '\0'; // ASCII \0 is 0000 0000
	}**/

	//Encode Compression Data
	if (iCompStat) { //Compression present
		if (cCompType == 'R') { //RLE Compression
			//Set last 4 bits of cEncCompByte to be 1000
			cEncCompByte += 'BS'; //Backspace Character //Only edit last 4 bits of this byte to avoid overwriting encrpytion information.

		} else if (cCompType == 'H') { //Huffman Compression
			//Set last 4 bits of cEncCompByte to be 1010
			cEncCompByte += 'LF'; //Line Feed character

		} else if (cCompType == 'B') { //RLE + Huffman Compression
			//Set last 4 bits of cEncCompByte to be 1100
			cEncCompByte += 'FF'; //Form Feed character

		} else if (cCompType == 'E') { //Huffman + RLE Compression
			//Set last 4 bits of cEncCompByte to be 1110
			cEncCompByte += 'SO'; //Shift Out character
		}
	}
	else { //Compression not Present
	 //write 0 to all encryption bits since no encryption present
		cEncCompByte = '\0'; // ASCII \0 is 0000 0000
	}

	//Write Data to header
	temp.bTBD[0] = cEncCompByte; //Write Encryption and Compression data
	temp.bTBD[1] = cCRCChecksumByte; //Write CRC or Checksum
	temp.bTBD[2] = cErrDetectandLengthByte; //Write error detection information and first 4 bits of length data
	temp.bTBD[3] = lengthByte; //Write remaining byte of length data


	return temp;
}

void getPacketSettings(packet getFrom, char* encSet, char* compSet, char* errSet, int pNum) { //gets packet settings from a header
	////char halving code
	//char a = 'A';
	//char lowB = a >> 4;
	//char highB = a & 15;

	char headerSettings[4];
	headerSettings[0] = getFrom.packetHeader.bTBD[0];
	headerSettings[1] = getFrom.packetHeader.bTBD[1];
	headerSettings[2] = getFrom.packetHeader.bTBD[2];
	headerSettings[3] = getFrom.packetHeader.bTBD[3];

	halveByte(headerSettings[0], &encSet, &compSet); //output encrpytion and compression settings
	
	char lengtgHBits = '\0';
	halveByte(headerSettings[1], &errSet, &lengtgHBits); //output error detection settings. get high 4 bits of length data
	
}

char getHighBits(char getFrom) {
	return getFrom & 15;
}

char getLowBits(char getFrom) {
	return getFrom >> 4;
}

int halveByte(char toHalve, char* highBits, char* lowBits) {
	lowBits = toHalve >> 4;
	highBits = toHalve & 15;

	return 0;
}

packet createPacket(Item pData) {
	/*
	 *	Function Name:			createPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			packet
	 *	Function Inputs:		pData (packet data)
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	packet temp;				//Create temporary variable
	temp.packetHeader = generateHeader(pData);	//Add packet header data to packet
	temp.packetData = pData;	//Add packet data to data
	return temp;				//Return packet

}

packetList createPacketList() { //unused

}

packet compressPacket(packet toCompress, int* outSize) { //compType will be determined by global settings
	/*
	 *	Function Name:			compressPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	extern unsigned char cCompType;

	//Internal Temporary Variables
	Item pData = toCompress.packetData; //holder for packet
	char compData[140]; //array for message data (will remove when Item compression added)
	compData[0] = '\0'; //null terminate so string functions work
	int compSize = 0;  //stores compressed size
	
	printf("Compressing Data using %c\n", cCompType);
	
	rleEncode(&pData.message, sizeof(pData.message), &compData, &compSize, 'ESC'); //compress data
	//printf("test size: %d\n", compSize);
	//compData[compSize-1] = '\0'; //ensure string is still null terminated
	//printf("Comp Size: %d\n", compSize);
	
	strcpy(toCompress.packetData.message, compData); //write compressed data to packet
	//printf("COMP DATA: %s\n", toCompress.packetData.message);
	printf("Successfully Compressed Data");

	*outSize = compSize;
	//printf("compsize is: %d\n", *outSize);

	return toCompress; //return compressed packet

}

packetB compressPacketTypeB(packetB toCompress) { //Experimental
	/*
	 *	Function Name:			compressPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/
	extern unsigned char cCompType;

	//Internal Temporary Variables
	void* pData = toCompress.packetData; //holder for packet data
	int compSize = 0;  //stores compressed size
	void* compData = malloc(sizeof(pData));
	printf("Compressing Data using %c\n", cCompType);

	rleEncode(&pData, sizeof(pData), &compData, &compSize, 'ESC'); //compress data
	//printf("Comp Size: %d\n", compSize);

	toCompress.packetData = compData;
	
	//printf("COMP DATA: %s\n", toCompress.packetData.message);
	printf("Successfully Compressed Data\n");
	return toCompress; //return compressed packet

}

packet decompressPacket(packet toDecompress) { //compType will be determined by global settings
	/*
	 *	Function Name:			compressPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	extern unsigned char cCompType;

	Item pData = toDecompress.packetData;
	char decompData[140]; 
	decompData[0] = '\0';
	int decompSize = 0;

	printf("Decompressing Data using %c\n", cCompType);

	decompSize = rleDecode(&pData.message, strlen(pData.message), &decompData, &decompSize, 'ESC'); //compress data
	decompData[decompSize] = '\0';

	//printf("Decomp Size: %d\n", decompSize);

	strcpy(toDecompress.packetData.message, decompData);//write compressed data to packet
	printf("COMP DATA: %s\n", toDecompress.packetData.message);

	printf("Successfully Decompressed Data\n");
	return toDecompress; //return decompressed packet

}

packetB decompressPacketTypeB(packetB toDecompress) { //Experimental
	/*
	 *	Function Name:			compressPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	extern unsigned char cCompType;

	void* pData = toDecompress.packetData; //holder for packet data
	int compSize = 0;  //stores compressed size
	void* compData = malloc(sizeof(pData));
	printf("Compressing Data using %c\n", cCompType);

	rleDecode(&pData, sizeof(pData), &compData, &compSize, 'ESC'); //compress data
	//printf("Comp Size: %d\n", compSize);

	packetB compPacket;
	compPacket.packetData = compData;

	//printf("COMP DATA: %s\n", toCompress.packetData.message);
	printf("Successfully Decompressed Data\n");
	return compPacket; //return compressed packet


};

int transmitTextPacket(Item toSend) { //format packet for passing to transmit
	/*
	 *	Function Name:			transmitTextPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			int
	 *	Function Inputs:		Item toSend, data to be sent by CMS
	 *	Function Outputs:		returns 0 on sucesss
	 *	Error Handling:			No
	 *	Function Description:	This function handles transmission of text packets as well as calling
	 *							functions for compressing, encrypting, and checksum generation.
	 *
	*/

	/* Theory of Operation
	*  1. create packet with toSend as the data
	*  2. convert packet to character array
	*  3. pass character array to transmit
	* 
	* 
	*/


	extern HANDLE hCom;

	//packet* tst = malloc(sizeof(packet));
	packet pToSend = createPacket(toSend); //create packet

	//Perform Compression, Encryption, and Error detection functions
	int pCompSize = 0;
	pToSend = compressPacket(pToSend, &pCompSize); //compress pToSend message using whatever compression type is set in CMS settings

	//printf("SIZPK: %d", sizeof(pToSend));
	//printf("SIZPK: %d", sizeof(header));
	initPortTX();
	//outputToPortTX(&pToSend, sizeof(header) + (sizeof(packet) + (pCompSize - MAX_MESSAGE_LENGTH)));	// Transmit packet
	outputToPortTX(&pToSend, sizeof(header) + (sizeof(packet) + (pCompSize - MAX_MESSAGE_LENGTH)));	// Transmit packet
	//Sleep(1000);
	purgePortTX();								// Purge the port
	CloseHandle(hCom);
	system("pause");
	
	//transmit(&pToSend); //depracated
	
}

int receiveTextPacket(packet* output) {
	/*
	 *	Function Name:
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	/* Theory of Operation
	*  1. receive a string from COM
	*  2. cast to packet
	*  3. extract message from packet
	* 
	*/


	extern HANDLE hCom;

	char recMsg[MAX_PACKET_SIZE];
	recMsg[0] = '\0';
	initPortRX();
	inputFromPortRX(recMsg, MAX_PACKET_SIZE); //receive packet from port
	purgePortRX();	//Purge port to ready it for next receive

	//Convert character (byte) array back to a packet
	recMsg[217] = '\0';
	printf("The received message is: %s\n", recMsg);
	
	packet* tmp = (packet*)recMsg;
	packet tst = *tmp;

	//Perform Decompression, Decryption, and Error detection on packet if needed
	header pHead;				  //holder variable for packet header
	getPacketHeader(tst, &pHead); //get contents of packet header

	tst = decompressPacket(tst);  //decompress packet data


	*output = tst;
	//printf("STRUCT: %s", tst.packetData.message);
	//purgePortRX();									// Purge the port
	CloseHandle(hCom);
	system("pause");
	
	//
	//packet* tst = (packet*)&recMsg; //cast to packet
	//Item iTst = tst->packetData;
	//char* cTst = iTst.message;
	// printf("DATA: %s", cTst);
	
	
	
	//packet* pRecMsg = malloc(sizeof(recMsg)); //allocate memory for message
	//pRecMsg = recMsg; //cast to packet
	//
	//Item iRecMsg;
	//iRecMsg = pRecMsg->packetData; //get packetData
	//char cRecMsg[140];
	//strcpy(cRecMsg, iRecMsg.message); //get message
	//printf("Received: %s", cRecMsg);
}

int getPacketHeader(packet getFrom, header* output) {
	/*
	 *	Function Name:
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	*output = getFrom.packetHeader;
}

int getPacketData(packet getFrom, Item* data) {
	/*
	 *	Function Name:
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	*data = getFrom.packetData;

}

int getPacketContents() {

}


int transmitAudPacket(Item toSend) { //format packet for passing to transmit
	/*
	 *	Function Name:			transmitTextPacket
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			int
	 *	Function Inputs:		Item toSend, data to be sent by CMS
	 *	Function Outputs:		returns 0 on sucesss
	 *	Error Handling:			No
	 *	Function Description:	This function handles transmission of text packets as well as calling
	 *							functions for compressing, encrypting, and checksum generation.
	 *
	*/

	/* Theory of Operation
	*  1. create packet with toSend as the data
	*  2. convert packet to character array
	*  3. pass character array to transmit
	*
	*
	*/
	printf("SIZPK: %d", sizeof(toSend));

	extern HANDLE hCom;

	//packet* tst = malloc(sizeof(packet));
	packet pToSend = createPacket(toSend); //create packet

	//Perform Compression, Encryption, and Error detection functions
	//int pCompSize = 0;
	//pToSend = compressPacket(pToSend, &pCompSize); //compress pToSend message using whatever compression type is set in CMS settings

	//printf("SIZPK: %d", sizeof(toSend));
	//printf("SIZPK: %d", sizeof(header));
	initPortTX();
	//outputToPortTX(&pToSend, sizeof(header) + (sizeof(packet) + (pCompSize - MAX_MESSAGE_LENGTH)));	// Transmit packet
	//outputToPortTX(&pToSend, sizeof(header) + (sizeof(packet)));	// Transmit packet
	outputToPortTX(&pToSend, sizeof(header) + (sizeof(packet)));	// Transmit packet
	//Sleep(1000);
	purgePortTX();								// Purge the port
	CloseHandle(hCom);
	//system("pause");

	//transmit(&pToSend); //depracated

}

int receiveAudPacket(packet* output) {

	extern HANDLE hCom;

	char recMsg[96000];
	recMsg[0] = '\0';
	initPortRX();
	inputFromPortRX(recMsg, 96000); //receive packet from port
	purgePortRX();	//Purge port to ready it for next receive

	//Convert character (byte) array back to a packet
	//recMsg[97999] = '\0';

	//packet* tmp = (packet*)recMsg;
	//packet tst = *tmp;

	//Perform Decompression, Decryption, and Error detection on packet if needed
	//header pHead;				  //holder variable for packet header
	//getPacketHeader(tst, &pHead); //get contents of packet header

	//tst = decompressPacket(tst);  //decompress packet data


	//*output = tst;
	//printf("STRUCT: %s", tst.packetData.message);
	//purgePortRX();									// Purge the port
	CloseHandle(hCom);
	

	//extern short iBigBuf[];												// buffer
	//extern long  lBigBufSize;											// total number of samples
	//short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file


	InitializePlayback();
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(recMsg, 96000);
	ClosePlayback();

	system("pause");



	//
	//packet* tst = (packet*)&recMsg; //cast to packet
	//Item iTst = tst->packetData;
	//char* cTst = iTst.message;
	// printf("DATA: %s", cTst);



	//packet* pRecMsg = malloc(sizeof(recMsg)); //allocate memory for message
	//pRecMsg = recMsg; //cast to packet
	//
	//Item iRecMsg;
	//iRecMsg = pRecMsg->packetData; //get packetData
	//char cRecMsg[140];
	//strcpy(cRecMsg, iRecMsg.message); //get message
	//printf("Received: %s", cRecMsg);
}

