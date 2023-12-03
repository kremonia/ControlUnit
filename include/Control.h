#pragma once
#include "Arduino.h"

#include "Global.h"
extern Global m_Global;

class Control
{
public:
	uint8_t flag_status = 0;
	uint8_t flag_work = 0;
	uint8_t flag_obj = 0;

	void Setup(void);
	void  StopMotor(void);
	void Otvet(uint8_t commanda, int16_t error, int32_t rab32i);
//	int32_t CorrXYZ(uint8_t axis);
	void Loop(void);
	void Status(void);
	int8_t Home(void);
};
