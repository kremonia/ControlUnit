#pragma once
#include "Arduino.h"

class CTest
{
public:
	void Loop(void);
	void TestMotor(int axis, int delay_ms);
	void Test45(void);
	void TestStep(uint8_t axis, int32_t value);
};
