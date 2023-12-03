//#include <avr/eeprom.h>
#include "Arduino.h"
#include "SPI.h"

#include "Global.h"
Global m_Global;

#include "Trinamic_TMC5130_registers.h"
#include "Trinamic_TMC5130.h"
Trinamic_TMC5130 m_Trinamic_TMC5130;

#include "DLPMega.h"
DLPMega m_DLPMega;

#include "Keypad.h"
#include "Key.h"
#include "Keybord.h"
Keybord m_Keybord;

#include "Joy.h"
Joy m_Joy;

#include "CaCRC.h"
CaCRC m_aCRC;

#include "TransferUsb.h"
TransferUsb m_TransferUsb;

#include "TransferRs485.h"
TransferRs485 m_TransferRs485;

#include "Control.h"
Control m_Control;

#include "Objective.h"
Objective m_Objective;

#include "Vacuum.h"
Vacuum m_Vacuum;

#include "InputSwitch.h"
InputSwitch m_InputSwitch;

#include "Motor.h"
Motor m_Motor[8];

#include "Marker.h"
Marker m_Marker;

#include "Camera.h"
Camera m_Camera;

#include "Scan.h"
Scan m_Scan;

#include "Focus.h"
Focus m_Focus;

#include "CTest.h"
CTest m_Test;

void setup()
{
	Serial.begin(115200);
	Serial.println("begin");

	delay(10000);
	m_TransferRs485.infoLast.data = 1;

	m_Control.Setup();

	m_Global.enc_corr = 0;

	m_InputSwitch.Loop();
	if (m_Global.micro_status.io & (1 << bit_in4_vacuum))
	{
		Serial.println("begin Test");
		m_Test.Loop();
	}
	else
	{
		m_Global.enc_corr = 0;
		m_Control.Home();

		m_Objective.ObjectiveNext();
		m_Objective.ObjectivePrev();

		Serial.println("End Setup");
		m_Global.enc_corr = 1;
	}
}

uint32_t count = 0, time_old=0;
uint8_t rab8u;
uint32_t valtemp = 0;
int32_t baze_print = 0;
int32_t corr_z_old = 0;
int32_t corr_z = 0;

void loop()
{
	uint8_t axis;

	m_InputSwitch.Loop();
	m_Vacuum.Loop();
	m_Objective.Loop();
	m_Keybord.Loop();
	m_Joy.Loop();
	m_TransferRs485.Loop();
	m_TransferUsb.Loop();

	int32_t rab32i, delta, step_enc, err;
	union64 rab64u;

	if (m_Global.enc_corr)
	{
		m_Motor[0].Loop();
		if (!m_Motor[0].status.vel_act)
		{
			step_enc = m_Motor[0].status.enc_act / 8;
			err = step_enc - m_Motor[0].status.step_act;

			if (abs(err) > 8)
			{
				m_Motor[0].motorTMC.set_RAMPMODE(0);
				m_Motor[0].motorTMC.set_XACTUAL(step_enc);

				Serial.print("0err=");
				Serial.print(err);
				Serial.print("0step=");
				Serial.print(m_Motor[0].status.step_act);
				Serial.print(" enc0=");
				Serial.println(step_enc);

				m_Motor[0].WaiteStop();
			}
		}

		m_Motor[1].Loop();
		if (!m_Motor[1].status.vel_act)
		{
			step_enc = m_Motor[1].status.enc_act / 8;
			err = step_enc - m_Motor[1].status.step_act;

			if (abs(err) > 8)
			{
				m_Motor[1].motorTMC.set_RAMPMODE(0);
				m_Motor[1].motorTMC.set_XACTUAL(step_enc);

				Serial.print("1err=");
				Serial.print(err);
				Serial.print("1step=");
				Serial.print(m_Motor[1].status.step_act);
				Serial.print(" enc1=");
				Serial.println(step_enc);

				m_Motor[1].WaiteStop();
			}
		}

	}

	rab64u.i64 = 0;
	corr_z = 0;
	for (axis = 0; axis < 8; axis++)
	{
		if (m_Global.micro_param.motor_enable & (1 << axis))
		{
			m_Motor[axis].Loop();
			rab32i = m_Motor[axis].StepToNm(m_Motor[axis].status.step_act);

			if (m_Global.micro_param.corr_enable & 1)
			{
				if (axis <= 2)
				{
					rab32i -= m_Global.obj_offset[m_Global.micro_status.obj][axis];
				}
				m_Global.micro_status.motor_nm[axis] = rab32i;
			}
		}
	}


	m_Control.Loop();

	if ((!m_Global.micro_status.motor_base) && !baze_print)
	{
		baze_print = 1;
		Serial.println("motor_base=0");
	}

	if (m_Global.errRS485)
	{
		m_Global.micro_status.rs485_error += m_Global.errRS485;
		m_Global.err3RS485 = m_Global.errRS485;
		m_Global.errRS485 = 0;
		Serial.print("err3RS485 all error count=");
		Serial.println(m_Global.err3RS485);
	}
/*
	if (!count)
	{
		Serial.print("cicle us=");
		Serial.println((millis() - time_old)/10);
		time_old = millis();
		count = 10000;

		Serial.print("0step=");
		Serial.print(m_Motor[0].status.step_act);
		Serial.print(" enc0=");
		Serial.println(m_Motor[0].status.enc_act);

		Serial.print("1step=");
		Serial.print(m_Motor[1].status.step_act);
		Serial.print(" enc1=");
		Serial.println(m_Motor[1].status.enc_act);
	}
	count--;
*/
}
