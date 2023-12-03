#pragma once
#include "Global.h"
extern Global m_Global;

class Vacuum
{
public:
	void Setup(void);
	void Loop(void);
	void Action(uint8_t flag);

	uint8_t flag_vacuum=0;
};
