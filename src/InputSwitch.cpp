
#include "InputSwitch.h"

#include "Global.h"
extern Global m_Global;

void InputSwitch::Setup(void)
{
	pinMode(Reserved_in1, INPUT);
	digitalWrite(Reserved_in1, HIGH);
}

void InputSwitch::Loop(void)
{
	if (!digitalRead(Reserved_in1))
	{
		m_Global.micro_status.io |= (1 << bit_in1);
	}
	else
	{
		m_Global.micro_status.io &= ~(1 << bit_in1);
	}

	if (!digitalRead(Reserved_in2))
	{
		m_Global.micro_status.io |= (1 << bit_in2);
	}
	else
	{
		m_Global.micro_status.io &= ~(1 << bit_in2);
	}

	if (!digitalRead(Reserved_in3))
	{
		m_Global.micro_status.io |= (1 << bit_in3);
	}
	else
	{
		m_Global.micro_status.io &= ~(1 << bit_in3);
	}

	if (!digitalRead(vacuum_in4))
	{
		m_Global.micro_status.io |= (1 << bit_in4_vacuum);
	}
	else
	{
		m_Global.micro_status.io &= ~(1 << bit_in4_vacuum);
	}
}
