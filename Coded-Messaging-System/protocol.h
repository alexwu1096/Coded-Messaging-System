/* Header Name:			Kilo Watt Squared Coded Messaging System Protocol
*  Header Author:		Garnet Koebel
*  Header Version:		V0.1
*  Header Created:		2020-11-26
*  Last Library Update:	2020-11-26
*  Library Description:	This file contains the declarations for the CMS protocol
*/

#ifndef PROTOCOL_H
#define PROTOCOL_H


#include "RS232Comm.h"
#include "Queues.h"
#include "compression.h"


//Typedefs
typedef char BYTE;				//typedef char to byte
typedef struct Header header;	//typedef so it is easier to type
typedef struct Packet packet;
typedef linkedList* packetList;

//Custom Datatypes
struct Header {					//Datatype for packet headers
	long		lSignature;		// must be 0xDEADBEEF
	BYTE		bReceiverAddr;	// receiver ID. 0xff=broadcast
	BYTE		bVersion;		// must be 1 for version 1
	long		lDataLength;	// size of message
	char		bTBD[4];		// stores info about message (i.e compression type
	BYTE		bPattern[4];	// must be 0xaa 0x55 0xaa 0x55
};

struct Packet {					//Datatype for message packets
	header packetHeader;		//Stores packet header
	Item packetData;			//Stores packet transmission data

};

typedef struct PacketTypeB packetB;

struct PacketTypeB {					//Datatype for message packets with void pointer
	header packetHeader;		//Stores packet header
	void* packetData;			//Stores packet transmission data

};

//Packet Handling Functions
header generateHeader(Item pData);			//Generate header based off packet contents and CMS settings
void getPacketSettings(packet getFrom, char* encStat, char* encType, char* compStat, char* compType, char* errStat, char* errType, int pNum);

packet createPacket(Item pData);			//Creates a packet for transmission

packet compressPacket(packet toCompress);	//compress packet data (message only)
packet decompressPacket(packet toCompress); //decompress packet data (message only)

//Experimental
packetB compressPacketTypeB(packetB pData);
packetB decompressPacketTypeB(packetB pData);

packetList createPacketList(); //Creates a list of packets for bulk sending of messages (May not use)


//Transmit Functions.
int transmitTextPacket(Item toSend);						//Transmits a text message 140 bytes at a time
//int transmitAudio(queue transmitQueue); //Transmits an audio file 140 bytes at a time

//Receive Functions
int receiveTextPacket(packet* output);						//Receives a text message 140 bytes at a time
//int receiveAudio(queue receiveQueue);	//Receives an audio file  140 bytes at a time


int transmitAudPacket(Item toSend);
int receiveAudPacket(packet* output);

#endif