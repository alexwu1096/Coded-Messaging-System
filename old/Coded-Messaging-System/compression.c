/* Library Name:		Compression Library
*  Library Author:		Garnet Koebel, Mike Wu
*  Library Version:		V0.1
*  Library Created:		2020-11-14
*  Last Library Update:	2020-11-14
*  Library Description:	
*/


/*
	 *	Function Name: 	
	 *	Function Author:
	 *	Function Type: 		
	 *	Function Inputs: 	
	 *	Function Outputs: 	
	 *	Error Handling:		
	 *	Function Description:	
	 *
	*/

#include "compression.h"

#define MIN_RUN 4 //minimum number of bytes to encode (prevents encoding to a longer string)
#define MAX_RUN 255 //max length of encoding run


int rleEncode(char* toEncode, int inLen, char* encoded, int* outLen, char escSeq) {
	/*
	 *	Function Name:		rleEncode
	 *	Function Author:	Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	/* Theory of Operation
	* 1. load toEncode string
	* 2. set currChar to first char of input
	* 3. count how many instances of currChar appear until currChar != lastChar
	* 4. if instances of currChar > minRun RLE encode to outputString, Else write character as it appears
	* 5. Repeat for rest of input.
	*/
	//printf("INPUT: %s\n\n", toEncode);

	//printf("BEGIN ENCODE\n");
	//int len = strlen(toEncode);
	//printf("LEN: %d\n", len);
	char currChar = toEncode[0];
	char lastChar = toEncode[0];
	char encodedString[100000]; //= malloc(sizeof(toEncode));// = strcpy(encodedString, toEncode); //make output string as long as input to avoid overflows
	encodedString[0] = '\0';
	int runCount = 0;	//start at -1 to remove duplicate character on first loop
	int outputLen = 1;

	for (int i = 0; i <= inLen; i++) {
		//printf("LOOP: %d\n", i);
		lastChar = currChar;
		currChar = toEncode[i];
		//printf("lastChar: %c\n", lastChar);
		//printf("currChar: %c\n\n", currChar);

		if (lastChar == currChar && i != inLen) {
			//printf("INCREMENT COUNT\n");
			runCount++;
		} else {
			if (runCount >= MIN_RUN) {
				if (runCount <= MAX_RUN) {
					encode(&encodedString, runCount, lastChar, escSeq);
					outputLen = outputLen + 4; //increment output length counter
				} else { //if runcount exceded encode max runs worth of characters then encode try again
					
					encode(&encodedString, MAX_RUN, lastChar, escSeq); //encode a max length run
					runCount = runCount - MAX_RUN; //subtract MAX_RUN from runCount
					//if on next run the runCount is still too high this will run again
					i = i - runCount;
					runCount = 1;
					outputLen = outputLen + 4; //increment output length counter
				}
				
				} else {
					for (int j = 0; j < runCount; j++) {
						//strcat unescaped characters to string
						char tmp[2];
						sprintf(tmp, "%c", lastChar);
						//printf("tmp: %s", tmp); //debug
						strcat(encodedString, tmp);
						outputLen++; //increment output length counter
					}
				}
			
			//outputLen = outputLen + 3; //increment output length counter
			runCount = 1; //reset counter
			
		}

	}

	//Encode remaining characters (characters at end of input)
	//encode(&encodedString, runCount, lastChar, escSeq);
	//printf("OUT LEN: %d\n", outLen);
	//printf("STRING: %s\n", encodedString);
	//printf("END ENCODE\n");
	*outLen = outputLen;
	strcpy(encoded, encodedString);
	return 0;
}

void encode(char* out, int runCount, char cChar, char escSeq) {
	/*
	 *	Function Name:			encode
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			void
	 *	Function Inputs:		out (pointer to output string), runCount (length of run to encode), cChar (run character), escSeq (escape sequence)
	 *	Function Outputs:		none, output is done by writing to address of out pointer
	 *	Error Handling:			No
	 *	Function Description:	This is a child function for rleEncode and exists to reduce code reuse in it's parent function.
	 *							This function contains the run encoding algorighm for RLE encode.
	 *
	*/

	//printf("ENCODE\n");
	char temp[4];
	sprintf(temp, "%c", escSeq);
	strcat(out, temp); //add escape character
	//printf("STRING ESC: %s\n", out);
	char runLen[5]; //string for holding run length
	sprintf(runLen, "%03d", runCount); //convert runCount to string
	strcat(out, runLen); //encode run length
	//printf("STRING LEN: %s\n", out);

		sprintf(temp, "%c", cChar);
		strcat(out, temp);
		//printf("STRING CHAR: %s\n", out);
	
}

int rleDecode(char* toDecode, int inLen, char* decoded, int* outLen, char escSeq) {

	/*
	 *	Function Name:			rleDecode
	 *	Function Author:		Garnet Koebel
	 *	Function Type:			int
	 *	Function Inputs:		toDecode (string to decode), decoded (pointer to output), outLen (pointer to output length int), escSeq (escape sequence)
	 *	Function Outputs:		returns zero on success, other output is done by pointers
	 *	Error Handling:			No
	 *	Function Description:	This function takes a pointer to a string that was RLE encoded and decodes it back into it's
	 *							original form.
	 *
	*/

	/* Theory of Operation
	* 1. load first char
	* 2. if ESC SEQ run decode()
	* 3. else write characters as is to decoded or temp array
	* 4. repeat
	*/

	//
	char decodedString[100000]; //= malloc(sizeof(toDecode));// = strcpy(encodedString, toEncode); //make output string as long as input to avoid overflows
	decodedString[0] = '\0';
	int decompSize = 0;
	char lastRun = '\0';

	char lastChar = toDecode[0]; //Doing this allowes
	char currChar = toDecode[1]; //for checking the first characters for the ESC SEQ
	//char escChk[4];
	//escChk[0] = lastChar;
	//escChk[1] = currChar;
	//escChk[2] = '\0';
	//har escStr[4];
	//sprintf(escStr, "%c", escSeq);

	//printf("LENGTH: %d\n", inLen);
	//printf("INPUT: %s\n", toDecode); //debug

	for (int i = 0; i < inLen; i++) {
	//	printf("To Decode: %c\n", toDecode[i]);
	//printf("I: %d\n", i); //Debug
		if (toDecode[i] == escSeq) {
			//printf("ESC\n"); //debug
			//escape sequence found run decode()
			char runStr[4];
			runStr[0] = toDecode[i + 1]; //move to run length counter
			runStr[1] = toDecode[i + 2];
			runStr[2] = toDecode[i + 3];
			lastRun = runStr[0]; //record what last run's char was
			int runLen = atoi(runStr); //convert counter back to an int
			decompSize = decompSize + runLen; //add run length to size of decompressed data

			char runChar = toDecode[i + sizeof(escSeq) + 3]; //get run cahracter
			for (int j = 0; j < runLen; j++) {
				//Write runLen number of runChar's to decoded
				char runStr[4];
				sprintf(runStr, "%c", runChar);
				strcat(decodedString, runStr);
			}

				i += 4;	//increment past current encoded run

		} else if (toDecode[i] != lastRun){ //if char after run is different than the last runChar and is not an escape sequence then it is an unencoded run.
			//printf("NON ESC\n"); //debug
			//Grab non escaped characters
				char temp[2];
				sprintf(temp, "%c", toDecode[i]);
				strcat(decodedString, temp);
		}

		
		//printf("STRING: %s\n", decodedString); //debug
	}
	
	//printf("STRING size: %d\n", strlen(decodedString)); //debug
	*outLen = strlen(decodedString);
	strcpy(decoded, decodedString);
	return decompSize;
}

char* computeSeqChk(char oper1, char oper2) { //will be used later for multi char ESC SEQ's. Currently unused
	char escChk[3];
	escChk[0] = oper1;
	escChk[1] = oper2;
	escChk[2] = '\0';

	return escChk;
}

int rleCompressFile(char fileToCompress[]) {
	/*
	 *	Function Name:			rleCompressFile
	 *	Function Author:		Garnet Koebel, Alex Wu
	 *	Function Type:			int
	 *	Function Inputs:		fileToCompress, a string containing the name of the target file
	 *	Function Outputs:		int, returns 0 on success
	 *	Error Handling:			yes, checks if file pointer null
	 *	Function Description:	This function compresses a file using RLE (Run Length Encoding)
	 *
	*/
	int outLen = 0;


	FILE* temp = fopen(fileToCompress, "r"); //open target file
	printf("Compressing %s\n", fileToCompress);
	char outName[sizeof(fileToCompress) + sizeof("-comp.rle") + 20]; //create array to store output filename
	outName[0] = '\0'; //make outName a string
	strcat(outName, fileToCompress);
	strcat(outName, "-comp.rle");
	FILE* outFile = fopen(outName, "w"); //open output file

	if (temp != NULL && outFile != NULL) { //make sure files opened correctly
		char* inChar = malloc(sizeof(char));
		char* compArr = NULL; //create an array of bytes that can fit the file contents
		//compArr[0] = '1';
		char outArr[300000];
	
		size_t size = 0;

		//Get the buffer size
		fseek(temp, 0, SEEK_END); // Go to end of file */
		size = ftell(temp); /* How many bytes did we pass ? */

		printf("Size of file: %d bytes\n", size);
		/* Set position of stream to the beginning */
		rewind(temp);

		/* Allocate the buffer (no need to initialize it with calloc) */
		compArr = (malloc((size + 1) * sizeof(*compArr))); /* size + 1 byte for the \0 */

		/* Read the file into the buffer */
		fread(compArr, size, 1, temp); /* Read 1 chunk of size bytes from fp into buffer */

		compArr[size] = '\0';            //NULL-terminate the buffer

		
		rleEncode(compArr, strlen(compArr), &outArr, &outLen, 'ESC'); //compress data
		printf("OUT ARR: %s\n", outArr);
		printf("OUT LEN: %d\n", outLen);
		fwrite(outArr, sizeof(char), strlen(outArr), outFile);


		fclose(temp);
		fclose(outFile);
		return 0;
	} else {
		printf("ERR_FILE_PTR_NULL\n");

		return 1;
	}
}

int rleDecompressFile(char fileToDecompress[]) {
	/*
	 *	Function Name:			rleDecompressFile
	 *	Function Author:		Garnet Koebel, Alex Wu
	 *	Function Type:			int
	 *	Function Inputs:		fileToDecompress, a string containing the name of the target file
	 *	Function Outputs:		int, returns 0 on success
	 *	Error Handling:			yes, checks if file pointer null
	 *	Function Description:	This function decompresses a file that was compressed using RLE
	 *							(Run Length Encoding)
	 *
	*/

	//append compressed to fileName for compressed file

	int outLen = 0;


	FILE* temp = fopen(fileToDecompress, "r"); //open target file
	printf("Decompressing %s\n", fileToDecompress);
	char outName[sizeof(fileToDecompress) + sizeof("-comp.rle") + 20]; //create array to store output filename
	outName[0] = '\0'; //make outName a string
	strcat(outName, fileToDecompress);
	strcat(outName, "-decomp.rle");
	FILE* outFile = fopen(outName, "w"); //open output file

	if (temp != NULL && outFile != NULL) { //make sure files opened correctly
		char* inChar = malloc(sizeof(char));
		char* compArr = NULL; //create an array of bytes that can fit the file contents
		//compArr[0] = '1';
		char outArr[300000];

		size_t size = 0;

		//Get the buffer size
		fseek(temp, 0, SEEK_END); // Go to end of file */
		size = ftell(temp); /* How many bytes did we pass ? */

		printf("Size of file: %d bytes\n", size);
		/* Set position of stream to the beginning */
		rewind(temp);

		/* Allocate the buffer (no need to initialize it with calloc) */
		compArr = (malloc((size + 1) * sizeof(*compArr))); /* size + 1 byte for the \0 */

		/* Read the file into the buffer */
		fread(compArr, size, 1, temp); /* Read 1 chunk of size bytes from fp into buffer */

		compArr[size] = '\0';            //NULL-terminate the buffer

		//printf("COMP ARR: %s\n", compArr);
		int decodeLen = rleDecode(compArr, strlen(compArr), &outArr, &outLen, 'ESC'); //compress data
		printf("OUT ARR: %s\n", outArr);
		printf("OUT LEN: %d\n", decodeLen);
		fwrite(outArr, sizeof(char), strlen(outArr), outFile);


		fclose(temp);
		fclose(outFile);
		return 0;
	}
	else {
		printf("ERR_FILE_PTR_NULL\n");

		return 1;
	}


}

int huffmanCompress(char filename[]) { //Super function for huffan compression
	/*
	 *	Function Name:				huffmanCompress
	 *	Function Author:			Alex Wu
	 *	Function Type:				int
	 *	Function Inputs:			filename, a string containing the name of the target file
	 *	Function Outputs:			int, returns 0 on success
	 *	Error Handling:
	 *	Function Description:		This function compresses a file using huffmanc coding library (huffman.c)
	 */

	FILE* input, * comp;

	input = fopen(filename, "rb");

	if (!input) {
		printf("Unable to open %s\n", filename);
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


	unsigned char out[100000];		//char array for compressed output

	unsigned int outsize;			//compressed output byte size

	outsize = Huffman_Compress(buffer, out, size);			// returns the huffman output byte size
	out[outsize] = '\0';									// NULL terminate array

	printf("IN MSG: %s\n", buffer);							// print the contents of the input file
	printf("IN LEN: %d\n\n", size);							// print input file size in bytes

	char filenameComp[30];									// char array for file name and extension for the compressed file

	printf("Type the desired name for the compressed file and extension to be saved: \n");
	scanf("%s", filenameComp);


	comp = fopen(filenameComp, "wb");			//opens the file to be compressed

	if (!comp) {								// error checking file
		printf("Unable to open %s\n", filenameComp);
		return;
	}


	fwrite(out, sizeof(char), outsize, comp);				// write the contents of the compressed data from array to file
	fclose(comp);
	printf("COMPRESSED MSG: %s\n", out);					// print the content of the compressed data
	printf("COMPRESSED LEN: %d\n\n", outsize);				// print the size of the compressed data in bytes

}

int huffmanDecompress(char filename[]) { //Super function for huffman decompression
	/*
	 *	Function Name:			huffmanDecompress
	 *	Function Author:		Alex Wu
	 *	Function Type:			int
	 *	Function Inputs:		filename, a string containing the name of the target file
	 *	Function Outputs:		int, returns 0 on success
	 *	Error Handling:
	 *	Function Description:	This function decompresses a file that was compressed using huffmanc coding library (huffman.c)
	 *
	*/

	FILE* input, * output;

	input = fopen(filename, "rb");

	if (!input) {
		printf("Unable to open %s\n", filename);
		return;
	}

	char* buffer = NULL;
	size_t size = 0;

	//Get the buffer size
	fseek(input, 0, SEEK_END);	// Go to end of file 
	size = ftell(input);		// Find the number of bytes passed

	printf("Size of file: %d bytes\n", size);

	rewind(input);				// Set position of stream to the beginning 

	/* Allocate the buffer (no need to initialize it with calloc) */
	buffer = (malloc((size + 1) * sizeof(*buffer)));	//size + 1 byte for the \0 

	/* Read the file into the buffer */
	fread(buffer, size, 1, input);			// Read 1 chunk of size bytes from fp into buffer 


	buffer[size] = '\0';            //NULL-terminate the buffer

	fclose(input);

	unsigned char decompressOut[100000];					// char array for decompressed output

	unsigned int outsize = size * 2;							// number of output byte size 

	Huffman_Uncompress(buffer, decompressOut, size, outsize);			// huffman decompression
	decompressOut[size] = '\0';										// NULL terminate the array

	char filenameDecomp[100];								// char array for file name and extension for decompressed file

	printf("Type the desired name of the decompressed file and extension to be saved: \n");
	scanf("%s", filenameDecomp);

	output = fopen(filenameDecomp, "wb");

	if (!output) {
		printf("Unable to open %s\n", filenameDecomp);
		return;
	}

	fwrite(decompressOut, sizeof(char), size, output);	// write the decompressed data to file
	fclose(output);
	printf("OUT MSG: %s\n", decompressOut);				// print the decompressed data
	printf("OUT LEN: %d\n", size);						// print the size of the decompressed data in bytes

}

int compressFile(char compType[]) { //Super function for compression
	/*
	 *	Function Name:			compressFile
	 *	Function Author:
	 *	Function Type:			int
	 *	Function Inputs:
	 *	Function Outputs:		returns zero on success, other output done by pointers
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

}

int compressData(void* data, char compType, void* compData) {
	/*
	 *	Function Name:			compressData
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	if (compType == 'R') {
		char* toComp = data;
		int compSize = 0;
		
		rleEncode(data, sizeof(data), &compData, &compSize, 'ESC');

	} else if (compType == 'H') {


	}

}

int decompressFile(char compType[]) { //Super function for compression
	/*
	 *	Function Name:			compressFile
	 *	Function Author:
	 *	Function Type:			int
	 *	Function Inputs:
	 *	Function Outputs:		returns zero on success, other output done by pointers
	 *	Error Handling:			
	 *	Function Description:
	 *
	*/

}

int decompressData(void* data, char compType, void* decompData) {
	/*
	 *	Function Name:			decompressData
	 *	Function Author:		Garnet Koebel
	 *	Function Type:
	 *	Function Inputs:
	 *	Function Outputs:
	 *	Error Handling:
	 *	Function Description:
	 *
	*/

	if (compType == 'R') {
		char* toComp = data;
		int compSize = 0;

		rleDecode(data, sizeof(data), &decompData, &compSize, 'ESC');

	}
	else if (compType == 'H') {


	}

}