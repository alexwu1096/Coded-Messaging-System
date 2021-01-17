/*
* File Name : Encryption Implementation
* File Author(s) : Mike Wu, Garnet Koebel
* Last File Update : 2020 - 12 - 03
* File Description : This file contains the definitions and implementations for encryption related functions.
*/


char* encryptDecrypt(char inpString[], char xorKey) {
    /*
         *	Function Name:          encryptDecrypt
         *	Function Author:		Mike Wu, Garnet Koebel
         *	Function Type:			char*
         *	Function Inputs:		char inpString[] - data to encrypt/decrypt, char xorKey - key to use for encryption/decryption
         *	Function Outputs:		char* - pointer to encrypted data
         *  Error Handling:			No
         *	Function Description:	This function takes in a character array and an encryption key. Performs an XOR operation on the character array
         *                          using the encryption key then returns a pointer to the encrypted data.
         *
        */


        // Define XOR key 
        // Any character value will work 
        //char xorKey = 'P';

        // calculate length of input string 
    int len = strlen(inpString);

    // perform XOR operation of key 
    // with every caracter in string 
    int i;
    for (i = 0; i < len; i++)
    {
        inpString[i] = inpString[i] ^ xorKey;
        //printf("%c", inpString[i]);
    }

    inpString[i] = '\0'; //null terminate string.

    return inpString;
}