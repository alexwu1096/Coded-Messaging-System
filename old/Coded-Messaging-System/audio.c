#include "audio.h"


void sendAudioMsg() {
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char save[2];
	char replay;
	char c;																// used to flush extra input
	FILE* f;

	InitializeRecording();
	// start recording
	RecordBuffer(iBigBuf, lBigBufSize);
	CloseRecording();

	//temp = iBigBuf;
	//printf("data is : %s\n", iBigBuf);
	// save audio recording  
	printf("Would you like to save your audio recording? (y/n): ");
	scanf("%s", &save);
	char saveC = save[0];
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((saveC == 'y') || (saveC == 'Y')) {
		/* Open input file */
		fopen_s(&f, "recording.dat", "wb");
		if (!f) {
			printf("unable to open %s\n", "recording.dat");
			return 0;
		}
		printf("Writing to sound file ...\n");
		fwrite(iBigBuf, sizeof(short), lBigBufSize, f);
		fclose(f);
	}
	
}

void recvAudioMsg() {
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;

	InitializePlayback();
	printf("\nPlaying recording from buffer\n");
	PlayBuffer(iBigBuf, lBigBufSize);
	ClosePlayback();
}

void playRecordedMsg() {
	extern short iBigBuf[];												// buffer
	extern long  lBigBufSize;											// total number of samples
	short* iBigBufNew = (short*)malloc(lBigBufSize * sizeof(short));		// buffer used for reading recorded sound from file

	char save;
	char replay;
	char c;																// used to flush extra input
	FILE* f;

	// replay audio recording from file -- read and store in buffer, then use playback() to play it
	printf("Would you like to replay the saved audio recording from the file? (y/n): ");
	scanf_s("%c", &replay, 1);
	while ((c = getchar()) != '\n' && c != EOF) {}								// Flush other input
	if ((replay == 'y') || (replay == 'Y')) {
		/* Open input file */
		fopen_s(&f, "C:\\myfiles\\recording.dat", "rb");
		if (!f) {
			printf("unable to open %s\n", "C:\\myfiles\\recording.dat");
			return 0;
		}
		printf("Reading from sound file ...\n");
		fread(iBigBufNew, sizeof(short), lBigBufSize, f);				// Record to new buffer iBigBufNew
		fclose(f);
	}
	InitializePlayback();
	printf("\nPlaying recording from saved file ...\n");
	PlayBuffer(iBigBufNew, lBigBufSize);
	ClosePlayback();

}