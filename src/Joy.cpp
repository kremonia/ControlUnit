#include "Joy.h"

#include "Global.h"
extern Global m_Global;

#include "Motor.h"
extern Motor m_Motor[8];

#include "TransferRs485.h"
extern TransferRs485 m_TransferRs485;

uint8_t analog_pins[3] = { A8,A9,A10 };//{A8,A9,A10,A9}; // a8,a9,a10,a11

void Joy::Setup()
{
	unsigned long rabul, rabul2;

	for (uint8_t i = 0; i <= 127; i++)
	{
		rabul = 160;
		rabul *= i;
		rabul *= i;
		rabul *= i;

		rabul2 = 9597;
		rabul2 *= i;
		rabul2 *= i;
		rabul += rabul2;

		rabul2 = 404400;
		rabul2 *= i;
		rabul += rabul2;

		rabul += 986700;
		rabul /= 3000000;

		speedKoeffJoy[i] =  (uint8_t)rabul;
	}

	delay(500);
	unsigned long count = 0;
	unsigned long Summa[3] = { 0 };

	unsigned long start = millis();
	while ((millis() - start) < 300)
	{
		for (uint8_t i = 0; i < 3; i++)
		{
			Summa[i] += analogRead(analog_pins[i]) >> 2;
		}
		count++;
	}

	for (int i = 0; i < 3; i++)
	{
		if (count)
		{
			m_JoySdvig[i] = Summa[i] / count;
		}
		else
		{
			m_JoySdvig[i] = 0;
		}
	}
}


void Joy::Loop()
{
	if ((millis() - lastTimeJoy) < UPDTE_JOY_MS)
	{
		return;
	}
	lastTimeJoy = millis();

	for (int i = 0; i < 3; i++)
	{
		joy_position[i] = analogRead(analog_pins[i]) >> 2;
	}

	int16_t direction = 0;
	uint16_t curr_value = 0;
	uint8_t axis0, axis, joy_axis, dir_abs;

	axis0 = 0;

	if (!(m_Global.micro_param.keyboard_enable))
	{
		if (!digitalRead(Reserved_in1))
		{
			m_Global.micro_status.io |= (1 << bit_group);
			m_Global.micro_param.joy_group |= 1;
			joy_active = m_Global.micro_param.joy_enable & 0xF0;
			axis0 = 4;
		}
		else
		{
			m_Global.micro_status.io &= ~(1 << bit_group);
			m_Global.micro_param.joy_group &= ~1;
			joy_active = m_Global.micro_param.joy_enable & 0x0F;
		}
	}
	else
	{
		if (m_Global.micro_status.key & (1 << 10))
		{
			joy_active = m_Global.micro_param.joy_enable;
			joy_active &= B0011;
			joy_active |= B0100;
		}
		else if (m_Global.micro_status.key & (1 << 11))
		{
			joy_active = m_Global.micro_param.joy_enable;
			joy_active &= B0011;
			joy_active |= B1000;
		}
	}

	for (axis = axis0; axis < (axis0 + 4); axis++)
	{
		if (joy_active & (1<<axis))
		{
			joy_axis = axis - axis0;
			if (joy_axis == 3)
			{
				joy_axis = 2;
			}

			curr_value = joy_position[joy_axis];
			direction = curr_value - m_JoySdvig[joy_axis];

			if (direction > 127)
			{
				direction = 127;
			}
			else if (direction < -127)
			{
				direction = -127;
			}

			dir_abs = abs(direction);
			if (dir_abs > STALL_RANGE)
			{
				if (axis == 2)
				{
					z_active = 1;
				}

				if (direction > STALL_RANGE)
				{
					direction -= STALL_RANGE;
				}
				else if (direction < -STALL_RANGE)
				{
					direction += STALL_RANGE;
				}

				if (m_Global.micro_param.joy_reverse & (1 << axis))
				{
					direction = -direction;
				}
				dir_abs = abs(direction);

				m_JoyWork |= (1<<axis);

				AverBuffSpeed[joy_axis][posAverBuffSpeed[joy_axis]] = speedKoeffJoy[abs(direction)];

				posAverBuffSpeed[joy_axis]++;
				if (posAverBuffSpeed[joy_axis] > LowPassJoySpeed)
				{
					posAverBuffSpeed[joy_axis] = 0;
				}

				uint16_t vel = 0;
				for (int x = 0; x < LowPassJoySpeed; x++)
				{
					vel += AverBuffSpeed[joy_axis][x];
				}
				vel /= LowPassJoySpeed*2;

				uint8_t vel_proc=100;
				if (m_Global.micro_status.obj != 255)
				{
					vel_proc = m_Global.micro_param.joy_speed_obj[m_Global.micro_status.obj];
				}

				vel *= vel_proc;
				vel /= 100;

				vel_proc = vel;
				if (!vel_proc)
				{
					vel_proc++;
				}

//				Serial.print("joy=");
//				Serial.print(axis);
//				Serial.print(" vel=");
//				Serial.println(vel);

				m_Global.micro_status.is_Stop = 0;

				int32_t rab32i;
				m_Motor[axis].Loop();
				rab32i = m_Motor[axis].StepToNm(m_Motor[axis].status.step_act);

				if (direction > 0)
				{
					if (m_Motor[axis].param.nm_max > 0)
					{
						if (rab32i < m_Motor[axis].param.nm_max)
						{
							m_Motor[axis].SetMode(1, VEL_POS_MODE, vel_proc);
						}
						else
						{
							m_Motor[axis].StopMotor();
						}
					}
					else
					{
						m_Motor[axis].SetMode(1, VEL_POS_MODE, vel_proc);
					}
				}
				else
				{
					if ((rab32i > 1000000) && (m_Motor[axis].param.nm_max > 0))
					{
						if (axis != 3)
						{
							m_Motor[axis].SetMode(0, VEL_NEG_MODE, vel_proc);
						}
						else
						{
							m_Motor[axis].StopMotor();
						}
					}
					else
					{
						m_Motor[axis].SetMode(1, VEL_NEG_MODE, vel_proc);
					}
				}
			}
			else
			{
				if (m_JoyWork & (1<<axis))
				{
					m_JoyWork &= ~(1 << axis);
					m_Motor[axis].StopMotor();
					posAverBuffSpeed[joy_axis] = 0;
					for (uint8_t x = 0; x < LowPassJoySpeed; x++)
					{
						AverBuffSpeed[joy_axis][x] = 0;
					}
/*
					int32_t rab32i;
					m_Motor[axis].Loop();
					rab32i = m_Motor[axis].StepToNm(m_Motor[axis].status.step_act);
					if (axis == 2)
					{
						z_active = 0;
						rab32i -= m_Control.CorrXYZ(axis);
						m_Control.z_dest=rab32i;

						Serial.print("z_dest=");
						Serial.print(rab32i);
						union64 rab64;
						rab64.i64 = m_Control.delta[0];
						Serial.print(" deltaX=");
						Serial.println(rab64.i32[0]);
						rab64.i64 *= m_Global.coef_x[0];
						Serial.print(" corrX0=");
						Serial.println(rab64.i32[1]);

						rab64.i64 = m_Control.delta[1];
						Serial.print(" deltaY=");
						Serial.println(rab64.i32[0]);
						rab64.i64 *= m_Global.coef_y[0];
						Serial.print(" corrY0=");
						Serial.print(rab64.i32[1]);
						Serial.print(" corr=");
						Serial.println(m_Control.CorrXYZ(axis));
					}
					else if (!z_active)
					{
						m_Control.delta[axis] = rab32i - m_Motor[axis].param.centr;

						Serial.print("z_corr axis=");
						Serial.print(axis);
						Serial.print(" z_dest=");
						Serial.println(m_Control.z_dest);

						union64 rab64;
						rab64.i64 = m_Control.delta[0];
						Serial.print(" deltaX=");
						Serial.print(rab64.i32[0]);
						rab64.i64 *= m_Global.coef_x[0];
						Serial.print(" corrX0=");
						Serial.println(rab64.i32[1]);

						rab64.i64 = m_Control.delta[1];
						Serial.print(" deltaY=");
						Serial.println(rab64.i32[0]);
						rab64.i64 *= m_Global.coef_y[0];
						Serial.print(" corrY0=");
						Serial.println(rab64.i32[1]);

						rab32i = m_Control.CorrXYZ(2);
						Serial.print(" corr=");
						Serial.println(rab32i);

						rab32i += m_Control.z_dest;
						rab32i = m_Motor[2].NmToStep(rab32i);
						m_Motor[2].SetMode(rab32i);
					}
*/
				}
			}
		}
	}
}
