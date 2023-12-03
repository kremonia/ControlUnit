#include "Focus.h"

#include "Global.h"
extern Global m_Global;

#include "Motor.h"
extern Motor m_Motor[8];

#include "Control.h"
extern Control m_Control;

#include "Camera.h"
extern Camera m_Camera;

void Focus::Setup(void)
{
}


int16_t Focus::Action(int32_t dest, int32_t shag, uint8_t cnt, uint16_t cadr_delay, uint8_t axis)
{
	int32_t  error_max, step_error;
	int16_t i;
	uint32_t time_start;

	Serial.print("Focus cnt=");
	Serial.print(cnt);
	Serial.print(" shag=");
	Serial.print(shag);
	shag= m_Motor[axis].NmToStep(shag);
	Serial.print(" step=");
	Serial.println(shag);

	error_max = m_Motor[axis].param.step_error;

	dest = m_Motor[axis].NmToStep(dest);
	m_Motor[axis].SetMode(dest - 1024);
	m_Motor[axis].WaiteStop();

	for (i = 0; i < cnt; i++)
	{
		m_Motor[axis].SetMode(dest);

		time_start= millis();
		while (true)
		{
			m_Motor[axis].Loop();
			step_error = dest - m_Motor[axis].status.step_act;
			if (abs(step_error) <= error_max)
			{

//				Serial.print(i);
//				Serial.print(" error=");
//				Serial.print(step_error);
//				Serial.print(" step=");
//				Serial.println(m_Motor[axis].status.step_act);

				m_Camera.Action();
				while (true)
				{
					if ((millis() - time_start) > cadr_delay)
					{
						break;
					}
				}
				break;
			}

			if ((millis() - time_start) >= 2000)  
			{
				Serial.print("Focus timeout ");
				Serial.println(i);
				return 5;
			}
		}
		dest += shag;
	}

	return 0;
}
