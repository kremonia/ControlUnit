#include "DLPMega.h"

void DLPMega::Setup(void)
{
	pinMode(DLP_WR, OUTPUT);
	pinMode(DLP_RD, OUTPUT);
	pinMode(DLP_RXF, INPUT);
	pinMode(DLP_TXE, INPUT);

	bitClear(PORTK, 3); //3
	delayMicroseconds(2);
	bitSet(PORTK, 3);// RD

	DDRF = 0xFF;
	writeByte(0xAA);
	DDRF = 0;
}

//========================================================================
bool DLPMega::writeByte(char val)
{
	PORTF = val;

	bitSet(PORTK, 4);// WR
	bitClear(PORTK, 4);// WR

	return true;
}

//========================================================================
bool DLPMega::readByte(byte& val)
{
	if (!bitRead(PINL,3))
	{
		bitClear(PORTK,3); //3
		delayMicroseconds(2);
		// ������ ���� ��� ���� ��� ����� ���������� �������	TODO:
		volatile byte a = PINF;
		val = a;
		val =PINF;
		bitSet(PORTK, 3); //3
		return true;
	}
	return false;
}

bool DLPMega::sendData(const byte* nData, word wLength)
{
	uint8_t i;
	DDRF = 0xFF;
	for (i = 0; i < wLength; i++)
	{
		writeByte(nData[i]);
	}
	DDRF = 0;

  return true;
}
