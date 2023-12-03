#include "CTest.h"
#include "Arduino.h"

#include "Global.h"
extern Global m_Global;

#include "InputSwitch.h"
extern InputSwitch m_InputSwitch;

#include "Motor.h"
extern Motor m_Motor[8];

void CTest::TestStep(uint8_t axis, int32_t value)
{
	int32_t step;

	m_Motor[axis].HomeSet(1000000);

	step = m_Motor[axis].NmToStep(value);
	Serial.print("axis=");
	Serial.print(axis);
	Serial.print(" value=");
	Serial.print(value);
	Serial.print(" step=");
	Serial.println(step);

	m_Motor[axis].SetMode(step, 0, 100);
	m_Motor[axis].param.timeout = 60000;
	m_Motor[axis].WaiteStop();
	delay(120000);

	step = -step;
	Serial.print("axis=");
	Serial.print(axis);
	Serial.print(" value=");
	Serial.print(value);
	Serial.print(" step=");
	Serial.println(step);

	m_Motor[axis].SetMode(step, 0, 100);
	m_Motor[axis].WaiteStop();
	delay(120000);
	Serial.println("--------------------------------------------------------------");
	while (true)
	{
		;
	}
}



void CTest::Test45(void)
{
	Serial.println("Test45");
	Serial.println("--------------------------------------------------------------");

	m_Motor[0].HomePosition();
	m_Motor[1].HomePosition();
	delay(10000);

	Serial.println("move X 100000000");
	m_Motor[0].SetMode(m_Motor[0].NmToStep(100000000), 0, 100);
	m_Motor[1].SetMode(m_Motor[1].NmToStep(100000000), 0, 100);

	delay(10000);
	Serial.println("move X 50000000");
	m_Motor[0].SetMode(m_Motor[0].NmToStep(50000000), 0, 100);
	m_Motor[1].SetMode(m_Motor[1].NmToStep(50000000), 0, 100);

	delay(10000);
	Serial.println("move Y 40000000");
	m_Motor[0].SetMode(m_Motor[0].NmToStep(10000000), 0, 100);
	m_Motor[1].SetMode(m_Motor[1].NmToStep(90000000), 0, 100);
	delay(10000);
	Serial.println("--------------------------------------------------------------");
}

void CTest::TestMotor(int axis, int delay_ms)
{
	uint16_t cnt=0;
	int32_t rab32i=0;

	Serial.println("TestMotor");
	Serial.println("--------------------------------------------------------------");
	Serial.print("Test axis=");
	Serial.println(axis);
	Serial.print(" csPin=");
	Serial.print(m_Motor[axis].param.csPin);
	Serial.print(" vel=");
	Serial.print(m_Motor[axis].param.vel_max);

	m_Motor[axis].HomePosition();

	while (true)
	{
		cnt++;
		Serial.println(cnt);
		delay(delay_ms);
		m_Motor[axis].SetMode(rab32i,0,100);
		m_Motor[axis].Loop();
		Serial.println(m_Motor[axis].status.step_act);
		rab32i += 1000;
	}
}

void CTest::Loop(void)
{
	uint16_t sec; // 100 msec

	uint8_t axis;

	axis = 0;
/*
	axis = 3;
	m_Motor[axis].param.vel_max = 300000;
	Serial.println("--------------------------------------------------------------");
	Serial.print("Test axis=");
	Serial.println(axis);
	Serial.print(" csPin=");
	Serial.print(m_Motor[axis].param.csPin);
	Serial.print(" vel=");
	Serial.print(m_Motor[axis].param.vel_max);
*/
	while (true)
	{
		m_Motor[axis].StopMotor();

		m_TransferUsb.Loop();
		m_Control.Loop();
		m_InputSwitch.Loop();
		if (!(m_Global.micro_status.io & (1 << bit_in4_vacuum)))
		{
			while (true)
			{
				if (axis < 7)
				{
					axis++;
				}
				else
				{
					axis = 0;
				}

				if (m_Global.micro_param.motor_enable & (1 << axis))
				{
					break;
				}
			}

			sec = 100;
			m_Motor[axis].param.timeout = 60000;
			while (sec)
			{
				m_Motor[axis].Loop();
				m_InputSwitch.Loop();
				if (m_Global.micro_status.io & (1 << bit_in4_vacuum))
				{
					break;
				}
				sec--;
				delay(100);
			}

			if (!sec)
			{
				break;
			}
		}

		m_Motor[axis].param.vel_max = 1000000;

		Serial.println("");
		Serial.println("--------------------------------------------------------------");
		Serial.print("Test axis=");
		Serial.println(axis);
		Serial.print(" csPin=");
		Serial.print(m_Motor[axis].param.csPin);
		Serial.print(" vel=");
		Serial.print(m_Motor[axis].param.vel_max);

		Serial.print(" NEG Move ");
		m_Motor[axis].StopMotor();
		m_Motor[axis].SetMode(0, VEL_NEG_MODE);

		sec = 100;
		while (sec)
		{
			m_Motor[axis].Loop();
			m_InputSwitch.Loop();
			if (!(m_Global.micro_status.io & (1 << bit_in4_vacuum)))
			{
				break;
			}

			delay(100);
			if (!(m_Motor[axis].status.vel_act))
			{
				break;
			}

			sec--;
		}

		m_Motor[axis].StopMotor();
		if (!sec)
		{
			Serial.println(" bad Neg Stop");
			delay(10000);
		}
		else
		{
			Serial.println(" good Neg");
		}

		Serial.print(" POS Move ");

		m_Motor[axis].SetMode(1, VEL_POS_MODE);

		sec = 100;
		while (sec)
		{
			m_Motor[axis].Loop();
			m_InputSwitch.Loop();
			if (!(m_Global.micro_status.io & (1 << bit_in4_vacuum)))
			{
				break;
			}

			delay(100);
			if (!(m_Motor[axis].status.vel_act))
			{
				break;
			}

			sec--;
		}

		m_Motor[axis].StopMotor();
		if (!sec)
		{
			Serial.println(" bad Pos Stop");
			delay(10000);
		}
		else
		{
			Serial.println(" good Pos");
		}
	}
}
