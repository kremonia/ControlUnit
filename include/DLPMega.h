#pragma once
#include "WString.h"
#include "Arduino.h"
#include "Global.h"

#define MAX_LENGTH 256
#define PACKAGE_SIZE 6 //10

// USB
#define DLP_WR               A12//35//28
#define DLP_RD               A11//34//29
#define DLP_TXE              47//33//27
#define DLP_RXF              46//32//26


class DLPMega
{
public:
	void Setup(void);

	bool readByte(byte& val);
	bool writeByte(char val);
	bool sendData(const byte* nData, word wLength);
};
