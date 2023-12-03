#include "Objective.h"
#include "Arduino.h"

#include "Global.h"
extern Global m_Global;

#include "TransferRs485.h"
extern TransferRs485 m_TransferRs485;

#include "Control.h"
extern Control m_Control;

void Objective::Setup(void)
{
}

void Objective::DelayGUF(void)
{
	Serial.println("GUF delay 5000");
	uint8_t cnt = 10;
	while (cnt)
	{
		cnt--;
		delay(100);
		m_TransferRs485.Loop();
	}
}

void Objective::ObjectiveNext(void)
{
	package_info info;
	uint8_t taut = 0;

	info.x23 = 0x23;
	info.dev_id = 1;
	info.status = 2;
	info.cmd = 1;
	info.key = 2;
	info.data = 1;
	info.data0 = 0;

	m_Control.flag_obj = 1;
	m_TransferRs485.writePackage((uint8_t*)&info);
	while (m_Control.flag_obj)
	{
		taut++;
		delay(100);
		if (!taut)
		{
			break;
		}
		m_TransferRs485.Loop();
	}

	if ((m_Global.micro_param.dev_id == dev_id_GUF) && (m_Global.micro_status.obj == 6))
	{
		DelayGUF();
	}
	return;
}

void Objective::ObjectivePrev(void)
{
	package_info info;
	uint8_t taut = 0;

	info.x23 = 0x23;
	info.dev_id = 1;
	info.status = 2;
	info.cmd = 1;
	info.key = 2;
	info.data = 2;
	info.data0 = 0;

	m_Control.flag_obj = 1;
	m_TransferRs485.writePackage((uint8_t*)&info);

	while (m_Control.flag_obj)
	{
		delay(100);
		taut++;
		if (!taut)
		{
			break;
		}
		m_TransferRs485.Loop();
	}

	if ((m_Global.micro_param.dev_id == dev_id_GUF) && (m_Global.micro_status.obj == 5))
	{
		DelayGUF();
	}
	return;
}

void Objective::ObjectiveGet()
{
	package_info info;
	info.dev_id = 1;
	info.status = RS_READ;
	info.cmd = CMD_EMPTY;
	info.key = RS_CURRENT_OBJ;
	info.data = 0;
	info.data0 = 0;

	m_Control.flag_work = 1;
	m_TransferRs485.writePackage((uint8_t*)&info);
	while (m_Control.flag_work)
	{
		delay(100);
		m_TransferRs485.Loop();
	}
//	Serial.print(" Obj old=");
//	Serial.println(m_Global.microstatus.obj);

	delay(100);
	return;
}

void Objective::ObjectiveSet(uint8_t* param)
{
//	Serial.print("ObjSet=");
//	Serial.print(*param);
	ObjectiveGet();

	while (*param != m_Global.micro_status.obj)
	{
		if (*param > m_Global.micro_status.obj)
		{
			ObjectiveNext();
		}
		else if (*param < m_Global.micro_status.obj)
		{
			ObjectivePrev();
		}

		if (m_Global.micro_status.obj == 6)
		{
			delay(4000);
		}
	}

//	Serial.print(" Obj new=");
//	Serial.println(m_Global.microstatus.obj);
	*param = m_Global.micro_status.obj;
	return;
}


void Objective::Loop(void)
{
	return;
}
