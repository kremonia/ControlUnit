#include "Control.h"

#include "TransferUsb.h"
extern TransferUsb m_TransferUsb;

#include "Motor.h"
extern Motor m_Motor[8];

#include "TransferRs485.h"
extern TransferRs485 m_TransferRs485;

#include "Objective.h"
extern Objective m_Objective;

#include "Joy.h"
extern Joy m_Joy;

#include "InputSwitch.h"
extern InputSwitch m_InputSwitch;

#include "Keybord.h"
extern Keybord m_Keybord;

#include "Vacuum.h"
extern Vacuum m_Vacuum;

#include "Marker.h"
extern Marker m_Marker;

#include "Camera.h"
extern Camera m_Camera;

#include "Scan.h"
extern Scan m_Scan;

#include "Focus.h"
extern Focus m_Focus;

void Control::Setup(void)
{
	uint8_t i, axis, dlinaEE;
	uint8_t* adrEE;
	uint8_t* adr;

	Serial.println("");
	Serial.println("Begin Setup");

	m_Global.micro_param.dev_id = eeprom_read_byte((uint8_t*)0);
//	m_Global.microparam.dev_id = 255;

	if (m_Global.micro_param.dev_id == 255)
	{// default setup
		m_Global.micro_param.dev_id = dev_id_Scan;

		Serial.println("Empty EEPROM");

		m_Global.micro_param.joy_enable = B1111;
		m_Global.micro_param.keyboard_enable = 1;
		m_Global.micro_param.joy_group = 0;

		m_Global.micro_param.joy_reverse = B00001000;


		// � ��������� 
		m_Global.micro_param.joy_speed_obj[0] = 100;
		m_Global.micro_param.joy_speed_obj[1] = 50;
		m_Global.micro_param.joy_speed_obj[2] = 25;
		m_Global.micro_param.joy_speed_obj[3] = 10;
		m_Global.micro_param.joy_speed_obj[4] = 5;
		m_Global.micro_param.joy_speed_obj[5] = 5;

		m_Global.micro_param.motor_enable = (B00001111);
		m_Global.micro_param.baza_enable = (B00000111);
		m_Global.micro_param.marker_impuls = 50;

		adrEE = 0;
		adr = &m_Global.micro_param.dev_id;
		dlinaEE = sizeof(micro_param_t);

		for (i = 0; i < dlinaEE; i++)
		{
			eeprom_update_byte(adrEE, *adr);
			adr++;
			adrEE++;
		}

		for (axis = 0; axis < 8; axis++)
		{
			m_Motor[axis].param.tip = 0;
			m_Motor[axis].param.current = 31;
			m_Motor[axis].param.ihold = 0;
			m_Motor[axis].param.timeout = 60000;
			m_Motor[axis].param.accelerate_max = 0xFFFF;
			m_Motor[axis].param.step_error = 16;
			m_Motor[axis].param.centr = 0;
			m_Motor[axis].param.load = 0;
			m_Motor[axis].param.oborot_value = 40000; //nm // 5000000; // 
			m_Motor[axis].param.vel_max = 1000000;
			m_Motor[axis].param.oborot_step = 1024;  //step 102400; // 
			m_Motor[axis].param.oborot_enc = 0;

			switch (axis)
			{
			case 0: // X
				m_Motor[axis].param.csPin = 37;
				break;
			case 1: // Y
				m_Motor[axis].param.csPin = 38;
				break;
			case 2: //Z
				m_Motor[axis].param.csPin = 53;			// GUF 53    //SVK 33
				m_Motor[axis].param.vel_max = 1000000; // 500000;
				m_Motor[axis].param.oborot_value = 500; //nm // 5000000; //
				break;
			case 3: // U
				m_Motor[axis].param.csPin = 36;
				m_Motor[axis].param.vel_max = 500000; 
				m_Motor[axis].param.oborot_value = 1125; 
				m_Motor[axis].param.oborot_step = 230400; //2304;  
				break;
			case 4: //X2
				m_Motor[axis].param.csPin = 31;
				m_Motor[axis].param.oborot_value = 20000; //nm // 5000000; // 
				m_Motor[axis].param.oborot_step = 512;  //step 102400; // 
				break;
			case 5: //Y2
				m_Motor[axis].param.csPin = 32;
				m_Motor[axis].param.oborot_value = 20000; //nm // 5000000; // 
				m_Motor[axis].param.oborot_step = 512;  //step 102400; // 
				break;
			case 6: //Z2
				m_Motor[axis].param.csPin = 33;
				m_Motor[axis].param.oborot_value = 20000; //nm // 2000000; // 
				m_Motor[axis].param.oborot_step = 512;  //step 102400; // 
				break;
			case 7: //U2
				m_Motor[axis].param.csPin = 35;
				m_Motor[axis].param.oborot_value = 36000; //Foton 360000
				m_Motor[axis].param.oborot_step = 804864;  //Foton 804864000
				break;
			default:
				break;
			}

			adrEE = (uint8_t*)(sizeof(micro_param_t) + sizeof(motor_param_t) * axis);
			adr = &m_Motor[axis].param.tip;
			dlinaEE = sizeof(motor_param_t);

			for (i = 0; i < dlinaEE; i++)
			{
				eeprom_update_byte(adrEE, *adr);
				adr++;
				adrEE++;
			}
		}

		// ooffset + stol
		dlinaEE = 72 + 16; // 
		for (i = 0; i < dlinaEE; i++)
		{
			eeprom_update_byte(adrEE, 0);
			adrEE++;
		}
	}

	m_Global.micro_param.corr_z[0][0] = 0;
	m_Global.micro_param.corr_z[0][1] = 0;
	m_Global.micro_param.corr_z[1][0] = 0;
	m_Global.micro_param.corr_z[1][1] = 0;
	m_Global.micro_param.corr_enable = 0;

	m_Global.micro_param.dev_id = eeprom_read_byte((uint8_t*)0);

	adrEE = 0;
	adr = &m_Global.micro_param.dev_id;
	dlinaEE = sizeof(micro_param_t);

	for (i = 0; i < dlinaEE; i++)
	{
		*adr = eeprom_read_byte(adrEE);
		adr++;
		adrEE++;
	}

	Serial.print("dev_id=");
	Serial.print(m_Global.micro_param.dev_id);
	Serial.print(" corr_enable=");
	Serial.println(m_Global.micro_param.corr_enable);


	m_Joy.joy_active = B111;

	for (axis = 0; axis < 8; axis++)
	{
		adrEE = (uint8_t*)(sizeof(micro_param_t) + sizeof(motor_param_t) * axis);
		adr = &m_Motor[axis].param.tip;
		dlinaEE = sizeof(motor_param_t);


		Serial.print("axis=");
		Serial.print(axis);
		Serial.print(" adrEE=");
		Serial.print((uint16_t)adrEE);
		Serial.print(" dlinaEE=");
		Serial.print (dlinaEE);

		for (i = 0; i < dlinaEE; i++)
		{
			*adr = eeprom_read_byte(adrEE);
			adr++;
			adrEE++;
		}

		Serial.print(" cs=");
		Serial.print(m_Motor[axis].param.csPin);
		Serial.print(" centr=");
		Serial.print(m_Motor[axis].param.centr);
		Serial.print(" load=");
		Serial.print(m_Motor[axis].param.load);
		Serial.print(" vel=");
		Serial.print(m_Motor[axis].param.vel_max);
		Serial.print(" value=");
		Serial.print(m_Motor[axis].param.oborot_value);
		Serial.print(" step=");
		Serial.print(m_Motor[axis].param.oborot_step);
		Serial.print(" error_step=");
		Serial.print(m_Motor[axis].param.step_error);
		Serial.print(" current=");
		Serial.print(m_Motor[axis].param.current);
		Serial.print(" hold=");
		Serial.print(m_Motor[axis].param.ihold);
		Serial.print(" enc=");
		Serial.println(m_Motor[axis].param.oborot_enc);
	}

	// offset
	adrEE = (uint8_t*)(sizeof(micro_param_t) + sizeof(motor_param_t) * 8);
	adr = (uint8_t*)&m_Global.obj_offset;
	dlinaEE = 6*12 + 4*4; // + coef_focus[2][2]

	Serial.print("offset eeprom read adrEE=");
	Serial.print((uint16_t)adrEE);
	Serial.print(" dlina=");
	Serial.println(dlinaEE);

	for (i = 0; i < dlinaEE; i++)
	{
		*adr = eeprom_read_byte(adrEE);
		adr++;
		adrEE++;
	}

	for (i = 0; i < 6; i++)
	{
		Serial.print("offset ");
		Serial.print(i);
		Serial.print(" X=");
		Serial.print(m_Global.obj_offset[i][0]);
		Serial.print(" Y=");
		Serial.print(m_Global.obj_offset[i][1]);
		Serial.print(" Z=");
		Serial.println(m_Global.obj_offset[i][2]);
	}

	m_DLPMega.Setup();
	m_InputSwitch.Setup();
	m_Keybord.Setup();
	m_TransferRs485.Setup();
	m_TransferUsb.Setup();
	m_Objective.Setup();
	m_Vacuum.Setup();
	m_Marker.Setup();
	m_Camera.Setup();
	m_Scan.Setup();
	m_Focus.Setup();

	for (uint8_t axis = 0; axis < 8; axis++)
	{
		if (m_Global.micro_param.motor_enable & (1<<axis))
		{
			m_Motor[axis].Setup();
			Serial.print("axis=");
			Serial.print(axis);
			Serial.print(" centr=");
			Serial.print(m_Motor[axis].param.centr);
			Serial.print(" load=");
			Serial.println(m_Motor[axis].param.load);

			if (axis == 3)
			{
				m_Motor[axis].motorTMC.set_stop_r_enable(0);
				m_Motor[axis].motorTMC.set_stop_l_enable(0);
			}
			
			//bad costul
			if ((m_Global.micro_param.dev_id == dev_id_Foton) && ((axis == 4) || (axis == 5)))
			{
				uint32_t data;
				m_Motor[axis].motorTMC.readREG(TMC_REG_GCONF, &data);
				data |= (1 << 4);
				m_Motor[axis].motorTMC.writeREG(TMC_REG_GCONF, data);

				m_Motor[axis].motorTMC.readREG(TMC_REG_SW_MODE, &data);
				data |= (1 << 4);

				m_Motor[axis].motorTMC.writeREG(TMC_REG_SW_MODE, data);

				m_Motor[axis].motorTMC.readREG(TMC_REG_GSTAT, &data);
			}
		}
	}

	m_Joy.Setup();
	m_Global.micro_status.motor_base = 0;

	Serial.print("Sdev_id=");
	Serial.println(m_Global.micro_param.dev_id);

	Serial.print("joy_enable=");
	Serial.println(m_Global.micro_param.joy_enable, HEX);

	Serial.print("motor_enable=");
	Serial.println(m_Global.micro_param.motor_enable, HEX);

	Serial.print("baza_enable=");
	Serial.println(m_Global.micro_param.baza_enable, HEX);

	Serial.print("corr_enable=");
	Serial.println(m_Global.micro_param.corr_enable, HEX);

	Serial.print("marker_impuls=");
	Serial.println(m_Global.micro_param.marker_impuls);

	Serial.println("End Setup");
}

void Control::Status(void)
{
	m_TransferUsb.bufferWrite.pak8.x55 = 0x55;
	m_TransferUsb.bufferWrite.pak8.dest = 1;
	m_TransferUsb.bufferWrite.pak8.ist = 0xAA;
	m_TransferUsb.bufferWrite.pak8.dlina = 10 + sizeof(m_Global.micro_status);
	m_TransferUsb.bufferWrite.pak8.key = micro_STATUS;
	m_TransferUsb.bufferWrite.pak8.kz_adr = 0;

	uint8_t* astatus;
	uint8_t* adata;

	m_Global.micro_status.paket_count++;

	astatus = (uint8_t*)&(m_Global.micro_status.commanda);
	adata = (uint8_t*)m_TransferUsb.bufferWrite.pak32.data;
	for (uint8_t i = 0; i < sizeof(m_Global.micro_status); i++)
	{
		*adata = *astatus;
		adata++;
		astatus++;
	}
}

void Control::StopMotor(void)
{
	for (size_t axis = 0; axis < 8; axis++)
	{
		m_Motor[axis].StopMotor();
	}
}

int8_t Control::Home(void)
{
	int8_t i, error, axis;

	for (i = 0; i < 8; i++)
	{
		axis = 7 - i;

		if (m_Global.micro_param.motor_enable & (1 << axis))
		{
			if (m_Global.micro_param.baza_enable & (1 << axis))
			{
				error = m_Motor[axis].HomePosition();
				if (error)
				{
					return error;;
				}
			}
			else
			{
				error = m_Motor[axis].HomeSet(0);

				if (error)
				{
					return error;;
				}
			}
	
			m_Global.micro_status.motor_base |= (1<<axis);

			Serial.print("motor_base=");
			Serial.println(m_Global.micro_status.motor_base, HEX);

			m_Global.micro_status.motor_nm[axis] = 0;
		}
	}

	uint8_t dlinaEE;
	uint8_t* adrEE;
	uint8_t* adr;

	adrEE = 0;
	adr = &m_Global.micro_param.dev_id;
	dlinaEE = sizeof(micro_param_t);

	for (i = 0; i < dlinaEE; i++)
	{
		*adr = eeprom_read_byte(adrEE);
		adr++;
		adrEE++;
	}

	Serial.print("End Home corr_enable=");
	Serial.println(m_Global.micro_param.corr_enable, HEX);

	return 0;
}

void Control::Otvet(uint8_t commanda, int16_t error, int32_t rab32i)
{
	int timeout;

	m_TransferUsb.bufferWrite.pak32.kz_adr = error;
	m_Global.micro_status.commanda = 0;

	if (!error)
	{
		switch (commanda)
		{
		case micro_DeskDllObjectiveGet:
		case micro_DeskDllObjectiveSet:
		case micro_OBJ_NEXT:
		case micro_OBJ_PREV:
		case micro_DeskDllLightGet:
		case micro_LIGHT_GET:
		case micro_DeskDllBFDFGet:
			m_TransferUsb.bufferWrite.pak32.dlina = 10 + 4 * 1;
			m_TransferUsb.bufferWrite.pak32.data[0] = rab32i;
			break;
/*
		case micro_POL_ANGLE:
		case micro_POL_DELTA:
		case micro_POL_HOME:
		case micro_POL_STEP_ABS:
		case micro_POL_STEP_REL:
			// wait
			flag_work = 1;
//			Serial.println("Pol wait state");
			timeout = 300;
			while (true)
			{
				flag_status = 1;
				m_TransferRs485.info_new.dev_id = ID_POLAROID;
				m_TransferRs485.info_new.cmd = CMD_READ_STATE; //7
				m_TransferRs485.info_new.data32 = 3;
				delay(10);
				m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info_new);
				if (timeout <= 0)
				{
					Serial.println(" timeout");
					error = -1;
					break;
				}
				delay(10);

				m_TransferRs485.Loop();
				if (!flag_work)
				{
					break;
				}
				timeout--;
				delay(100);

				m_TransferRs485.Loop();
				if (!flag_work)
				{
					break;
				}
			}

			flag_status = 2;
			m_TransferRs485.info_new.dev_id = ID_POLAROID;
			m_TransferRs485.info_new.cmd = CMD_READ_ANGLE; //6
			m_TransferRs485.info_new.data32 = 3;
			m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info_new);

			flag_work = 1;
			timeout = 300;
			while (true)
			{
				if (timeout <= 0)
				{
					Serial.println(" timeout");
					error = -1;
					break;
				}
				m_TransferRs485.Loop();
				if (!flag_work)
				{
					m_TransferUsb.bufferWrite.pak32.dlina = 10 + 4 * 2;
					m_TransferUsb.bufferWrite.pak32.data[0] = m_Global.micro_status.polar_ugol;
					m_TransferUsb.bufferWrite.pak32.data[1] = m_Global.micro_status.polar_enc;
					break;
				}
				timeout--;
				delay(10);
			}
			break;
*/
		default:
			break;
		}
	}
	else
	{
		Serial.print("commanda=");
		Serial.print(commanda);
		Serial.print(" error=");
		Serial.println(error);
	}

	m_TransferUsb.bufferWrite.pak8.kz_adr = error;
	m_TransferUsb.Send();
	m_Global.micro_status.commanda = 0;
}

void Control::Loop(void)
{
	if (!m_Global.micro_status.commanda_new)
	{
		return;
	}

	uint16_t commanda;
	uint8_t rab8u;
	uint8_t i;
	uint8_t* adr;
	uint8_t* adata;
	int8_t axis, param8, dlinaEE;
	uint16_t axis_maska;

	int16_t error = 0;
	int32_t rab32i = 0, step = 0;

	commanda = m_Global.micro_status.commanda_new;
	m_Global.micro_status.commanda_new = 0;

	m_Global.micro_status.is_Stop = 0;
	m_TransferUsb.bufferWrite.pak32.x55 = 0x55;
	m_TransferUsb.bufferWrite.pak32.dest = 1;
	m_TransferUsb.bufferWrite.pak32.ist = 0xAA;
	m_TransferUsb.bufferWrite.pak32.dlina = 10;
	m_TransferUsb.bufferWrite.pak32.key = commanda;
	m_TransferUsb.bufferWrite.pak32.kz_adr = 0;

	switch (commanda)
	{
	case micro_STATUS:
		Status();
		m_TransferUsb.Send();
		return;
	case micro_STOP:
	case micro_STOP_MOTOR:
		m_Global.micro_status.commanda = 0;
		Serial.println("STOP");
		m_Global.micro_status.is_Stop = 1;
		m_TransferUsb.Send();
		return;
	}

	if (m_Global.micro_status.commanda)
	{
		return;
	}

	if (commanda & micro_noWaite) // проверка на noWaite
	{
		m_Global.micro_status.commanda = commanda;
		m_TransferUsb.Send();
	}

	switch (commanda)
	{
	case micro_MOVE + micro_noWaite:
		m_Global.enc_corr = 0;
		axis = m_TransferUsb.bufferRead.pak8.kz_adr;
		m_Joy.m_JoyPC = (1 << axis);
		rab32i = m_TransferUsb.bufferRead.pak32.data[0];
		//Serial.print("MOVE ");
		//Serial.print(axis);
		//Serial.print("  ");
		//Serial.println(m_TransferUsb.bufferRead.pak32.kz_adr);

		if (rab32i > 0)
		{
			m_Motor[axis].SetMode(1, VEL_POS_MODE, rab32i);
		}
		else  if (rab32i < 0)
		{
			m_Motor[axis].SetMode(0, VEL_NEG_MODE, -rab32i);
		}
		else
		{
			m_Joy.m_JoyPC = 0;
			m_Motor[axis].StopMotor();
		}
		m_Global.enc_corr = 1;
		break;
	case micro_CORR:
//		Serial.print("CORR ");
//		Serial.println(m_TransferUsb.bufferRead.pak32.kz_adr, HEX);
		m_Global.micro_param.corr_enable = m_TransferUsb.bufferRead.pak32.kz_adr;
		break;
	case micro_CENTR:
		Serial.println("CENTR ");
		
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			m_Vacuum.Action(1);
			delay(500);

			for (axis = 0; axis < 3; axis++)
			{
				rab32i = m_Motor[axis].param.centr;
				if ((axis >= 0) && (axis <= 2) && (m_Global.micro_param.corr_enable & 1))
				{
					rab32i += m_Global.obj_offset[m_Global.micro_status.obj][axis];
				}
				step = m_Motor[axis].NmToStep(rab32i - 5000);
				m_Motor[axis].SetMode(step);
			}

			delay(50);

			for (axis = 0; axis < 3; axis++)
			{
				error = m_Motor[axis].WaiteStop();
				if (error)
				{
					break;
				}
			}

			for (axis = 0; axis < 3; axis++)
			{
				rab32i = m_Motor[axis].param.centr;
				if ((axis >= 0) && (axis <= 2) && (m_Global.micro_param.corr_enable & 1))
				{
					rab32i += m_Global.obj_offset[m_Global.micro_status.obj][axis];
				}
				step = m_Motor[axis].NmToStep(rab32i);
				m_Motor[axis].SetMode(step, 0, 25);
				Serial.println(rab32i);
			}
		}
		break;
	case micro_LOAD:
//		Serial.println("LOAD ");
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			for (axis = 0; axis < 3; axis++)
			{
				rab32i = m_Motor[axis].param.load;
				step = m_Motor[axis].NmToStep(rab32i);
				m_Motor[axis].SetMode(step);
			}

			delay(50);
			for (axis = 0; axis < 3; axis++)
			{
				error = m_Motor[axis].WaiteStop();
				if (error)
				{
					break;
				}
			}

			m_Vacuum.Action(0);
		}
		break;
	case micro_FOCUS:
	case micro_FOCUS + micro_noWaite:
//		Serial.println("micro_FOCUS");
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			rab32i = m_TransferUsb.bufferRead.pak32.data[0];
			error = m_Focus.Action( rab32i, m_TransferUsb.bufferRead.pak32.data[1], m_TransferUsb.bufferRead.pak32.data[2], m_TransferUsb.bufferRead.pak32.data[3], m_TransferUsb.bufferRead.pak8.kz_adr);
		}
		break;
	case micro_ABS:
	case micro_ABS + micro_noWaite:
//		Serial.println("ABS ");
		m_Global.enc_corr = 0;
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			axis_maska = m_TransferUsb.bufferRead.pak8.kz_adr;
			i = 0;
			for (axis = 0; axis < 8; axis++)
			{
				if (axis_maska & (1 << axis))
				{
					rab32i = m_TransferUsb.bufferRead.pak32.data[i];
					if ((axis >= 0) && (axis <= 2) && (m_Global.micro_param.corr_enable & 1))
					{
						rab32i += m_Global.obj_offset[m_Global.micro_status.obj][axis];
					}

					if ((rab32i > m_Motor[axis].param.nm_max) && (m_Motor[axis].param.nm_max > 0))
					{
						rab32i = m_Motor[axis].param.nm_max;
					}
					else if ((rab32i < 1000000) && (axis != 3))
					{
						rab32i = 1000000;
					}

					m_TransferUsb.bufferRead.pak32.data[i] = rab32i;

					if ((axis == 0) || (axis == 1))
					{
						rab32i -= 5000;
					}
					else
					{
						rab32i -= 500;
					}

					step = m_Motor[axis].NmToStep(rab32i);

//					Serial.print(" axis=");
//					Serial.print(axis);
//					Serial.print(" nm=");
//					Serial.print(rab32i);
//					Serial.print(" d_step=");
//					Serial.println(step - m_Motor[axis].status.step_act);

					m_Motor[axis].SetMode(step);
					i++;
				}
			}

			if (!(commanda & micro_noWaite))
			{
				delay(100);
				for (axis = 0; axis < 8; axis++)
				{
					if (axis_maska & (1 << axis))
					{
						error = m_Motor[axis].WaiteStop();
						if (error)
						{
							break;
						}
					}
				}
			}

			i = 0;
			for (axis = 0; axis < 8; axis++)
			{
				if (axis_maska & (1 << axis))
				{
					rab32i = m_TransferUsb.bufferRead.pak32.data[i];
					step = m_Motor[axis].NmToStep(rab32i);
					m_Motor[axis].SetMode(step);

//					Serial.print(" axis=");
//					Serial.print(axis);
//					Serial.print(" nm=");
//					Serial.print(rab32i);
//					Serial.print(" step=");
//					Serial.println(step);

					i++;
				}
			}

			if (!(commanda & micro_noWaite))
			{
				delay(100);
				for (axis = 0; axis < 8; axis++)
				{
					if (axis_maska & (1 << axis))
					{
						error = m_Motor[axis].WaiteStop();
						if (error)
						{
							break;
						}
					}
				}
			}
//			Serial.println("end ABS");
		}
		m_Global.enc_corr = 1;
		break;
	case micro_REL:
	case micro_REL + micro_noWaite:
//		Serial.println("REL ");
		m_Global.enc_corr = 0;
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			axis_maska = m_TransferUsb.bufferRead.pak8.kz_adr;
			i = 0;
			for (axis = 0; axis < 8; axis++)
			{
				if (axis_maska & (1 << axis))
				{
					rab32i = m_Motor[axis].status.step_act;
					rab32i = m_Motor[axis].StepToNm(rab32i);
					rab32i += m_TransferUsb.bufferRead.pak32.data[i];

					if ((rab32i > m_Motor[axis].param.nm_max) && (m_Motor[axis].param.nm_max > 0))
					{
						rab32i = m_Motor[axis].param.nm_max;
					}
					else if ((rab32i < 1000000) && (axis != 3))
					{
						rab32i = 1000000;
					}
					m_TransferUsb.bufferRead.pak32.data[i] = rab32i;

					if ((axis == 0) || (axis == 1))
					{
						step = m_Motor[axis].NmToStep(rab32i - 5000);
					}
					else
					{
						step = m_Motor[axis].NmToStep(rab32i - 500);
					}

					step = m_Motor[axis].NmToStep(rab32i);
					m_Motor[axis].SetMode(step);
					i++;
				}
			}

			delay(100);
			if (!(commanda & micro_noWaite))
			{
				for (axis = 0; axis < 8; axis++)
				{
					if (axis_maska & (1 << axis))
					{
						error = m_Motor[axis].WaiteStop();
						if (error)
						{
							break;
						}
					}
				}
			}

			i = 0;
			for (axis = 0; axis < 8; axis++)
			{
				if (axis_maska & (1 << axis))
				{
					rab32i = m_TransferUsb.bufferRead.pak32.data[i];
					step = m_Motor[axis].NmToStep(rab32i);
					m_Motor[axis].SetMode(step);
					i++;
				}
			}

			if (!(commanda & micro_noWaite))
			{
				delay(100);
				for (axis = 0; axis < 8; axis++)
				{
					if (axis_maska & (1 << axis))
					{
						error = m_Motor[axis].WaiteStop();
						if (error)
						{
							break;
						}
					}
				}
			}
		}
		m_Global.enc_corr = 1;
		break;
	case micro_SCAN:
	case micro_SCAN + micro_noWaite:
		//			Serial.println("micro_SCAN");
		if ((!m_Global.micro_status.motor_base) || (m_Global.micro_status.obj == 255) || (m_TransferUsb.bufferRead.pak8.kz_adr > 120))
		{
			Serial.println("error no base");
			error = 2;
		}
		else
		{
			error = m_Scan.Action(m_TransferUsb.bufferRead.pak8.kz_adr);
		}
		break;
	case micro_Vacuum:
		m_Vacuum.Action(m_TransferUsb.bufferRead.pak8.kz_adr);
		break;
	case micro_DeskSvet:
		Serial.println("DeskSvet");
		m_TransferRs485.info.dev_id = ID_OBJ_MCU;
		m_TransferRs485.info.status = RS_WRITE;
		m_TransferRs485.info.cmd = CMD_MOVE_MIRROR;
		m_TransferRs485.info.key = 5;
		m_TransferRs485.info.data = 1;
		m_TransferRs485.info.data0 = 0;
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
		delay(1000);
		break;
	case micro_DeskIk:
		Serial.println("DeskIk");
		m_TransferRs485.info.dev_id = ID_OBJ_MCU;
		m_TransferRs485.info.status = RS_WRITE;
		m_TransferRs485.info.cmd = CMD_MOVE_MIRROR;
		m_TransferRs485.info.key = 5;
		m_TransferRs485.info.data = 2;
		m_TransferRs485.info.data0 = 0;
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
		delay(1000);
		break;
	case micro_DeskUp:
		//			Serial.println("DeskUp");
		m_Marker.ActionLOW();

		m_TransferRs485.info.dev_id = ID_CONTROL_PANEL;
		m_TransferRs485.info.status = RS_WRITE;
		m_TransferRs485.info.cmd = 0;
		m_TransferRs485.info.key = 0x0E;
		m_TransferRs485.info.data = 2;
		m_TransferRs485.info.data0 = 0;
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
		delay(1000);
		break;
	case micro_DeskDown:
		//			Serial.println("DeskDown");
		m_Marker.ActionHIGH();

		m_TransferRs485.info.dev_id = ID_CONTROL_PANEL;
		m_TransferRs485.info.status = RS_WRITE;
		m_TransferRs485.info.cmd = 0;
		m_TransferRs485.info.key = 14;
		m_TransferRs485.info.data = 1;
		m_TransferRs485.info.data0 = 0;
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
		delay(1000);
		break;
	case micro_PowerOn:
		//			Serial.println("PowerOn");
		break;
	case micro_PowerOff:
		//			Serial.println("PowerOff");
		break;
	case micro_ECHO:
		Serial.println("ECHO");
		break;
	case micro_MARKER:
		//			Serial.println("MARKER");
		m_Marker.Action();
		break;
	case micro_DeskDllSetup:
		//			Serial.println("Setup");
		Setup();
		Home();
		m_Objective.ObjectiveNext();
		m_Objective.ObjectivePrev();
		break;
	case micro_DeskDllHome:
	case micro_HOME:
		//			Serial.println("Home");
		error = Home();
		//			Serial.print("Home kz=");
		//			Serial.println(error);
		break;
	case micro_OBJ_GET:
	case micro_DeskDllObjectiveGet:
//		Serial.println("OBJ_GET");
		m_Objective.ObjectiveGet();
		rab32i = m_Global.micro_status.obj;
		break;
	case micro_DeskDllObjectiveSet:
//		Serial.println("OBJ_SET");
		m_Objective.ObjectiveSet(&m_TransferUsb.bufferRead.pak8.data[0]);
		rab32i = m_Global.micro_status.obj;
		break;
	case micro_OBJ_NEXT:
//		Serial.println("OBJ_NEXT");
		m_Objective.ObjectiveNext();
		rab32i = m_Global.micro_status.obj;
		break;
	case micro_OBJ_PREV:
//		Serial.println("OBJ_PREV");
		m_Objective.ObjectivePrev();
		rab32i = m_Global.micro_status.obj;
		break;
	case micro_EE_READ:
		adr = (uint8_t*)m_TransferUsb.bufferRead.pak8.kz_adr;
		dlinaEE = -m_TransferUsb.bufferRead.pak8.dlina_inf;
		Serial.print("EE_READ ");
		Serial.print((uint16_t)adr);
		Serial.print(" ");
		Serial.println(dlinaEE);

		adata = (uint8_t*)&m_TransferUsb.bufferWrite.pak8.data;
		m_TransferUsb.bufferWrite.pak8.dlina_inf = dlinaEE;
		for (i = 0; i < dlinaEE; i++)
		{
			param8 = eeprom_read_byte(adr);
			//				Serial.print(" ");
			//				Serial.print(param8);
			*adata = param8;
			adata++;
			adr++;
		}
		//			Serial.println("");
		m_TransferUsb.bufferWrite.pak8.dlina = 10 + dlinaEE;
		break;
	case micro_EE_WRITE:
		adr = (uint8_t*)m_TransferUsb.bufferRead.pak8.kz_adr;
		dlinaEE = m_TransferUsb.bufferRead.pak8.dlina_inf;
		Serial.print("EE_WRITE ");
		Serial.print((uint16_t)adr);
		Serial.print(" ");
		Serial.print(dlinaEE);

		adata = (uint8_t*)&m_TransferUsb.bufferRead.pak8.data[0];
		for (i = 0; i < dlinaEE; i++)
		{
			param8 = *adata;
			
			Serial.print(" ");
			Serial.print(param8);
			
			eeprom_update_byte(adr, param8);
			adata++;
			adr++;
		}
		Serial.println("");
		break;
	case micro_WRITE:
		//			Serial.print("micro_WRITE ");
		//			Serial.print(m_TransferUsb.bufferRead.pak32.kz_adr);
		//			Serial.print(" ");
		//			Serial.println(m_TransferUsb.bufferRead.pak32.dlina_inf);
		if ((m_TransferUsb.bufferRead.pak32.kz_adr * 4 + m_TransferUsb.bufferRead.pak32.dlina_inf) > sizeof(m_Global.bufer))
		{
			error = 2;
		}
		else
		{
			for (i = 0; i < m_TransferUsb.bufferRead.pak32.dlina_inf / 4; i++)
			{
				//					Serial.println(m_TransferUsb.bufferRead.pak32.data[i]);
				m_Global.bufer[m_TransferUsb.bufferRead.pak32.kz_adr + i] = m_TransferUsb.bufferRead.pak32.data[i];
			}
		}
		break;
	case micro_READ:
		//			Serial.println("micro_READ");
		//			Serial.println(m_TransferUsb.bufferRead.pak32.kz_adr);
		if (m_TransferUsb.bufferRead.pak32.dlina_inf > 52)
		{
			error = 2;
		}
		else
		{
			for (i = 0; i < m_TransferUsb.bufferRead.pak32.dlina_inf / 4; i++)
			{
				m_TransferUsb.bufferWrite.pak32.data[i] = m_Global.bufer[m_TransferUsb.bufferRead.pak32.kz_adr + i];
			}
		}

		m_TransferUsb.bufferWrite.pak32.dlina_inf = m_TransferUsb.bufferRead.pak32.dlina_inf;
		m_TransferUsb.bufferWrite.pak32.dlina = 10 + m_TransferUsb.bufferWrite.pak32.dlina_inf;
		break;
	case micro_LIGHT_PLUS:
		break;
	case micro_LIGHT_MINUS:
		break;
	case micro_LIGHT_STATUS:
		break;
	case micro_LIGHT_GET:
	case micro_DeskDllLightGet:
		break;
	case micro_LIGHT_SET:
	case micro_DeskDllLightSet:
		break;
	case micro_DeskDllApertureGet:
		break;
	case micro_DeskDllApertureSet:
		break;
/*
	case micro_POL_HOME:
	case micro_POL_ANGLE:
	case micro_POL_DELTA:
	case micro_POL_SET_SPEED:
	case micro_POL_STOP:
	case micro_POL_READ_ANGLE:
	case micro_POL_READ_STATE:
	case micro_POL_MOVE:
	case micro_POL_POS_RESET:
	case micro_POL_STEP_ABS:
	case micro_POL_STEP_REL:
		//			Serial.print("Pol commanda=");
		//			Serial.println(commanda);

		m_TransferRs485.info_new.dev_id = ID_POLAROID;
		m_TransferRs485.info_new.data32 = m_TransferUsb.bufferRead.pak32.data[0];
		switch (commanda)
		{
		case micro_POL_ANGLE:
			m_TransferRs485.info_new.cmd = 1;
			break;
		case micro_POL_DELTA:
			m_TransferRs485.info_new.cmd = 2;
			break;
		case micro_POL_SET_SPEED:
			m_TransferRs485.info_new.cmd = 3;
			break;
		case micro_POL_HOME:
			m_TransferRs485.info_new.cmd = 4;
			break;
		case micro_POL_STOP:
			m_TransferRs485.info_new.cmd = 5;
			break;
		case micro_POL_READ_ANGLE:
			m_TransferRs485.info_new.cmd = 6;
			flag_status = 2;
			break;
		case micro_POL_READ_STATE:
			m_TransferRs485.info_new.cmd = 7;
			flag_status = 1;
			break;
		case micro_POL_MOVE:
			m_TransferRs485.info_new.cmd = 8;
			break;
		case micro_POL_POS_RESET:
			m_TransferRs485.info_new.cmd = 9;
			m_Global.micro_status.polar_ugol = 0;
			m_Global.micro_status.polar_enc = 0;
			break;
		case micro_POL_STEP_ABS:
			m_TransferRs485.info_new.cmd = 10;
			break;
		case micro_POL_STEP_REL:
			m_TransferRs485.info_new.data32 += m_Global.micro_status.polar_ugol;
			m_TransferRs485.info_new.cmd = 11;
			break;
		default:
			break;
		}
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info_new);
		break;
*/
	case micro_DeskDllBFDFGet:
		//			Serial.println("BFDFGet=");
		m_TransferRs485.dataRead(57, &rab32i);
//		Serial.println(rab32i);
		break;
	case micro_DeskDllBFDFSet:
		//			Serial.print("BFDFSet=");
//		Serial.println(m_TransferUsb.bufferRead.pak32.data[0]);

		m_TransferRs485.dataRead(57, &rab32i);

		rab8u = 4;
		while (rab32i != m_TransferUsb.bufferRead.pak32.data[0])
		{
			m_TransferRs485.info.dev_id = ID_OBJ_MCU;
			m_TransferRs485.info.status = RS_WRITE;
			m_TransferRs485.info.cmd = CMD_SWITCH_LIGHTER;
			m_TransferRs485.info.key = 0x1D; //29;
			m_TransferRs485.info.data = 0;
			m_TransferRs485.info.data0 = 0;
			m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
			delay(1000);

			m_TransferRs485.dataRead(57, &rab32i);
			//				Serial.print("BFDF=");
			//				Serial.println(rab32i);

			if (!rab8u)
			{
				error = 1;
				break;
			}
			rab8u--;
		}
		break;
	case micro_DeskDllPortUpGet:
		//			Serial.println("PortUpGet=");
		m_TransferRs485.dataRead(59, &rab32i);
		//			Serial.println(rab32i);
		break;
	case micro_DeskDllPortDownGet:
		//			Serial.println("PortDownGet=");
		m_TransferRs485.dataRead(60, &rab32i);
		//			Serial.println(rab32i);
		break;
	case micro_DeskDllFiltrGet:
		//			Serial.println("FiltrGet=");
		m_TransferRs485.dataRead(58, &rab32i);
		//			Serial.println(rab32i);
		break;
	case micro_DeskDllPortUpSet:
		//			Serial.print("PortUpSet=");
		//			Serial.println(m_TransferUsb.bufferRead.pak32.data[0]);

		m_TransferRs485.dataRead(59, &rab32i);

		rab8u = 4;
		while (rab32i != m_TransferUsb.bufferRead.pak32.data[0])
		{
			m_TransferRs485.info.dev_id = ID_OBJ_MCU;
			m_TransferRs485.info.status = RS_WRITE;
			m_TransferRs485.info.cmd = CMD_SWITCH_UPPORT;
			m_TransferRs485.info.key = 59;
			m_TransferRs485.info.data = 0;
			m_TransferRs485.info.data0 = 0;
			m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
			delay(1000);

			m_TransferRs485.dataRead(59, &rab32i);
			//				Serial.print("PortUp=");
			//				Serial.println(rab32i);

			if (!rab8u)
			{
				error = 1;
				break;
			}
			rab8u--;
		}
		break;
	case micro_DeskDllPortDownSet:
		//			Serial.print("PortDownSet=");
		//			Serial.println(m_TransferUsb.bufferRead.pak32.data[0]);

		m_TransferRs485.dataRead(60, &rab32i);

		rab8u = 4;
		while (rab32i != m_TransferUsb.bufferRead.pak32.data[0])
		{
			m_TransferRs485.info.dev_id = ID_OBJ_MCU;
			m_TransferRs485.info.status = RS_WRITE;
			m_TransferRs485.info.cmd = CMD_SWITCH_LOWPORT;
			m_TransferRs485.info.key = 60;
			m_TransferRs485.info.data = 0;
			m_TransferRs485.info.data0 = 0;
			m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
			delay(1000);

			m_TransferRs485.dataRead(60, &rab32i);
			//				Serial.print("PortDown=");
			//				Serial.println(rab32i);

			if (!rab8u)
			{
				error = 1;
				break;
			}
			rab8u--;
		}
		break;
	case micro_DeskDllFiltrSet:
		//			Serial.print("FiltrSet=");
		//			Serial.println(m_TransferUsb.bufferRead.pak32.data[0]);

		m_TransferRs485.dataRead(58, &rab32i);

		rab8u = 4;
		while (rab32i != m_TransferUsb.bufferRead.pak32.data[0])
		{
			m_TransferRs485.info.dev_id = ID_OBJ_MCU;
			m_TransferRs485.info.status = RS_WRITE;
			m_TransferRs485.info.cmd = CMD_SWITCH_OPFILTER;
			m_TransferRs485.info.key = 58;
			m_TransferRs485.info.data = 0;
			m_TransferRs485.info.data0 = 0;
			m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);
			delay(1000);

			m_TransferRs485.dataRead(58, &rab32i);
			//				Serial.print("Filtr=");
			//				Serial.println(rab32i);

			if (!rab8u)
			{
				error = 1;
				break;
			}
			rab8u--;
		}
		break;
	default:
		error = 2;
		Serial.print("bad commanda ");
		Serial.println(commanda);
		break;
	}

	if (!m_Global.micro_status.commanda) // проверка на noWaite
	{
		Otvet(commanda, error, rab32i);
	}
	else
	{
		m_Global.micro_status.commanda = 0;
	}
}
