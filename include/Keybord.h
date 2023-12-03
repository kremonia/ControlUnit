#pragma once
#include "Keypad.h"
#include "Global.h"

#include "DLPMega.h"
extern DLPMega m_DLPMega;

class Keybord 
{
public:
	Keypad* keypad;

	void Setup(void);
	void Loop(void);
	void GetStatusKey(void);

	uint32_t lastTimeKey;
};
