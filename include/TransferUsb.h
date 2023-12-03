#pragma once
#include "DLPMega.h"
extern DLPMega m_DLPMega;

class TransferUsb
{
public:
	word input_index=0;
	paket bufferRead;
	paket bufferWrite;

	void Setup(void);
	void Loop(void);
	void Send(void);
	int ControlKSUM(void);
};
