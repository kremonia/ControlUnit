#pragma once
#include "Arduino.h"
#define Objective_LEFT	 24
#define Objective_RIGHT	 22

#define OBJECTIVE_NUMBER 6
#define DELAY_IMPULSE_SWITCH_OBJECTIVE_MS        200	// ms

class Objective
{
public:
	void Setup(void);
	void Loop(void);

	void ObjectiveNext(void);
	void ObjectivePrev(void);
	void ObjectiveGet(void);
	void ObjectiveSet(uint8_t* param);
	void DelayGUF(void);
};
