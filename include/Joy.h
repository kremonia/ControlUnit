#pragma once
#include "Global.h"
extern Global m_Global;

#include "Motor.h"
extern Motor m_Motor[8];

#define LowPassJoySpeed 32
#define UPDTE_JOY_MS        10	// ms
#define STALL_RANGE         15 // 10  // 

class Joy
{
public:
	void Setup(void);
	void Loop(void);

	uint16_t m_JoySdvig[3] = { 128,128,128 };
	uint8_t  m_JoyWork = {0};
	uint8_t  m_JoyPC = { 0 };
	uint8_t speedKoeffJoy[128];

	uint32_t lastTimeJoy =  0;
	uint8_t joy_active = 0;
	uint8_t z_active = 0;

	uint16_t joy_position[3] = { 0,0,0 };
	uint8_t AverBuffSpeed[3][LowPassJoySpeed];
	uint16_t posAverBuffSpeed[3] = { 0,0,0 };
};
