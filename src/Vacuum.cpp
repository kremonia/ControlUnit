#include "Vacuum.h"

#include "Global.h"
extern Global m_Global;

void Vacuum::Setup(void)
{
	// ������
	pinMode(Klap_Vacuum, OUTPUT);
//	digitalWrite(Klap_Vacuum, LOW);
	digitalWrite(Klap_Vacuum, HIGH);
}

void Vacuum::Action(uint8_t flag)
{
	flag_vacuum = flag;
	Loop();
}

void Vacuum::Loop(void)
{
	if ((m_Global.micro_status.io & (1 << bit_in4_vacuum)) || (flag_vacuum))
	{
		digitalWrite(Klap_Vacuum, HIGH);
	}
	else
	{
		digitalWrite(Klap_Vacuum, LOW);
	}
}

/*void Vacuum::Loop(void) //integral
{
	byte value;

	digitalWrite(S0, 1);
	digitalWrite(S1, 1);
	digitalWrite(S2, 1);

	value = digitalRead(COM);
	if (!value)
	{
		m_Global.microstatus.io &= ~(1 << bit_vacuum);
		digitalWrite(Klap_Vacuum, HIGH);
	}
	else
	{
		m_Global.microstatus.io |= 1 << bit_vacuum;
		digitalWrite(Klap_Vacuum, LOW);
	}
}*/
