#include "Scan.h"

#include "Global.h"
extern Global m_Global;

#include "Motor.h"
extern Motor m_Motor[8];

#include "Control.h"
extern Control m_Control;

#include "Camera.h"
extern Camera m_Camera;

void Scan::Setup(void)
{
}


int16_t Scan::Action(uint8_t count)
{
	int16_t error = 0;
	int32_t x_start, y_start, z_start;
	uint8_t i;
	uint8_t motor_x = 0, motor_y = 1, motor_z = 2;

//	Serial.print("SCAN=");
//	Serial.println(count);

	i = 0;
	while(count)
	{
		x_start = m_Global.bufer[i];
		i++;
		y_start = m_Global.bufer[i];
		i++;
		z_start = m_Global.bufer[i];
		i++;

//		Serial.print(count);
//		Serial.print(" ");
//		Serial.println(y_start);

		m_Motor[motor_x].SetMode(m_Motor[motor_x].NmToStep(x_start));
		m_Motor[motor_y].SetMode(m_Motor[motor_y].NmToStep(y_start));
		m_Motor[motor_z].SetMode(m_Motor[motor_z].NmToStep(z_start));

		error = m_Motor[motor_z].WaiteStop();
		if (error)
		{
			m_Control.StopMotor();
			Serial.print(count);
			Serial.println(" Scan Z error noWait");
			return error;
		}

		error = m_Motor[motor_x].WaiteStop();
		if (error)
		{
			m_Control.StopMotor();
			Serial.print(count);
			Serial.println(" Scan X error noWait");
			return error;
		}

		error = m_Motor[motor_y].WaiteStop();
		if (error)
		{
			m_Control.StopMotor();
			Serial.print(count);
			Serial.println(" Scan Y error noWait");
			return error;
		}

//		Serial.print("camera ");
//		Serial.println(count);

		m_Camera.Action();
		count--;
	}

	return error;
}
