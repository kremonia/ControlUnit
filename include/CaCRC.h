#pragma once
#include "stdint.h"

class CaCRC
{
public:
	uint16_t CRC16(uint8_t* buf, uint16_t dlina);
};
