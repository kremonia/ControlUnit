#pragma once
#include "stdint.h"
#include "MicroFunction.h"

class Focus
{
public:
	void Setup(void);

	int16_t Action(int32_t dest, int32_t shag=2000, uint8_t cnt=20, uint16_t cadr_delay =100, uint8_t axis = 2);
};
