/* Library Name:		Compression Library
*  Library Author:		Garnet Koebel
*  Library Version:		V0.1
*  Library Created:		2020-11-14
*  Last Library Update:	2020-11-14
*  Library Description:
*/

#ifndef COMPRESSION_LIB_H
#define COMPRESSION_LIB_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//Run Length Encoding Functions
int rleEncode(char* toEncode, int inLen, char* encoded, int* outLen, char escSeq); //RLE encodes data from an array of bytes
void encode(char* out, int runLen, char cChar, char escSeq);					   //used inside rleEncode to cut down on code reuse
int rleDecode(char* toDecode, int inLen, char* decoded, int* outLen, char escSeq); //RLE decodes data from an array of bytes
int rleCompressFile(char fileToCompress[]);		//Uses RLE encoding to compress a file
int rleDecompressFile(char fileToDecompress[]);	//Uses RLE decoding to decompress a files
char* computeSeqChk(char oper1, char oper2);	//Depracated

//Huffman Encoding Functions
int huffmanCompress();
int huffmanDecompress();

//Super Compression Functions
int compressFile(char compType[]);
int compressData(void* data, char compType, void* compData);

int decompressFile(char compType[]);
int decompressData(void* data, char compType);


#endif