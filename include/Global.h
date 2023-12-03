#pragma once

#include "Arduino.h"
#include "MicroFunction.h"

// Multiplexor 
#define S0				 5
#define S1				 4
#define S2				 3
#define COM				 6
#define MULT_EN			 7

#define Klap_Vacuum			 12	

#define Reserved_in1		22
#define Reserved_in2		23
#define Reserved_in3		24
#define vacuum_in4			25

// Marker
#define Klap_Marker			 48

// Camera
//DDRK |= (1 << 5) | (1 << 6);

class Global
{
public:
	int errRS485 = 0;
	micro_status_t micro_status;
	micro_param_t micro_param;
	int32_t obj_offset[6][3] = { { 0 } };

	int32_t bufer[120 * 3];

	int err3RS485 = 0;
	uint8_t enc_corr = 1;
	uint8_t baze_print = 1;
};
