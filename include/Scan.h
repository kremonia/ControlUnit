#pragma once
#include "stdint.h"
#include "MicroFunction.h"

class Scan
{
public:
	void Setup(void);

	int16_t Action(uint8_t count);
};
