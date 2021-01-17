/*
 *	Program Name:			main.c
 *	Program Author: 		Garnet Koebel
 *	Program Version: 		V0.1
 *	Last Program Update:	2020-12-02
 *	Program Description:	This header and its C file are used for calling standalone validation 
 *							and testing functions.
 *
*/

#ifndef DIAGNOSTIC_H
#define DIAGNOSTIC_H

#include "protocol.h"

void packetCompressionTest(); //validaton function for packet compression
void packetEncrpytionTest();	//validation function for packet encryption
void packetErrorDetectTest();	//validation function for packet error detection

//int diagnosticPrint(); //super function for calling diagnostic print functions












#endif