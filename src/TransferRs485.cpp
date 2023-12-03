#include "TransferRs485.h"

#include "Global.h"
extern Global m_Global;

#include "Control.h"
extern Control m_Control;

#include "Motor.h"
extern Motor m_Motor[8];

#include "Marker.h"
extern Marker m_Marker;

#include "Joy.h"
extern Joy m_Joy;

//#define TIME_REQ_STATUS_OBJS	50   //ms

void TransferRs485::init()
{
	pinMode(RS485_PIN_RXTX, OUTPUT);
	digitalWrite(RS485_PIN_RXTX, LOW);
	Serial2.begin(RS485_SPEED);

	this->is_transmited = 0;
	this->is_received = 0;
	this->is_start_receive = 0;
	this->inputLengthRS = 0;
}

TransferRs485::TransferRs485()
{
}

TransferRs485::~TransferRs485()
{
}

void TransferRs485::Setup()
{
		init();
		setData(RS_DEV_ID, ID_CONTROL_BLOCK);
}

uint16_t TransferRs485::CrcXmodemUpdata(uint16_t crc, uint8_t data)
{
	crc = crc ^ ((uint16_t)data << 8);
	for (int i = 0; i < 8; i++) {
		if (crc & 0x8000) {
			crc = (crc << 1) ^ 0x1021;
		}
		else {
			crc <<= 1;
		}
	}
	return crc;
}

uint16_t TransferRs485::calc_crc(uint8_t* data, uint16_t size)
{
	uint16_t crc = 0xFFFF;
	for (uint16_t i = 0; i < size; ++i) {
		crc = CrcXmodemUpdata(crc, data[i]);
	}
	return crc;
}



bool TransferRs485::readPak(void)
{
	if (Serial2.available())
	{
		int inbyte;
		uint8_t i = 0;

		while (true)
		{
			if (!Serial2.available())
			{ // ���� ����� ����� ������
				delay(2);
				if (!Serial2.available())
				{
					return false;
				}
			}
			inbyte = Serial2.read();
			inputRS[i] = (uint8_t)inbyte;

			if (!i)
			{
				if (inbyte != START_PACKAGE)
				{
					m_Global.errRS485++;
					if (m_Global.errRS485 == 1)
					{
						Serial.print(inbyte);
						Serial.print(" inbyte != START_PACKAGE all error count=");
						Serial.println(m_Global.errRS485);
					}
					continue;
				}
			}

			i++;
			if (i == RS485_PACKAGE_SIZE)
			{ 
				if ((inputRS[0] == START_PACKAGE) && (inputRS[RS485_PACKAGE_SIZE - 1] == END_PACKAGE))
				{
					uint16_t crc = calc_crc(inputRS, RS485_PACKAGE_SIZE - 3);

					if (((uint8_t)(crc >> 8) != inputRS[7]) || ((uint8_t)crc != inputRS[8]))
					{
						i = 0;
						m_Global.errRS485++;
						if (m_Global.errRS485 == 1)
						{
							Serial.print("bad ks RS485 all error count=");
							Serial.println(m_Global.errRS485);
						}
						continue;
					}

					infoLast.dev_id = inputRS[1];
					infoLast.status = inputRS[2];
					infoLast.cmd = inputRS[3];
					infoLast.key = inputRS[4];
					infoLast.data0 = inputRS[5];
					infoLast.data= inputRS[6];

					return true;
				}
				else
				{
					i = 0;
					m_Global.errRS485++;
					if (m_Global.errRS485 == 1)
					{
						Serial.print(inputRS[RS485_PACKAGE_SIZE - 1]);
						Serial.print(" inputRS[RS485_PACKAGE_SIZE - 1] != END_PACKAGE all error count=");
						Serial.println(m_Global.errRS485);
					}
					continue;
				}
			}
		}
	}
	return false;
}

void TransferRs485::Obj_mcu()
{
	if ((infoLast.status == RS_WRITE))
	{
		if ((infoLast.cmd == CMD_CHANGE_LIGHTER_U) && (infoLast.key == 29))
		{
			if (infoLast.data == 1)
			{
//				Serial.println("light_plus");
			}
			else
			{
//				Serial.println("light_minus");
			}
		}
		else if ((infoLast.cmd == 0) && (infoLast.key == 63))
		{
			if (infoLast.data == 103)
			{
//				Serial.println("povorot_L");
				m_Motor[3].SetMode(1, VEL_POS_MODE);
			}
			else if (infoLast.data == 3)
			{
				m_Motor[3].StopMotor();
			}
			else if (infoLast.data == 102)
			{
//				Serial.println("povorot_R");
				m_Motor[3].SetMode(0, VEL_NEG_MODE);
			}
			else if (infoLast.data == 2)
			{
				m_Motor[3].StopMotor();
			}
		}
	}
	else
	{
	}
}

void  TransferRs485::Control_panel(void)
{
//	Serial.print("Control_panel data=");
//	Serial.println(infoLast.data);

	if (infoLast.status == RS_WRITE)
	{
		if (infoLast.cmd == 0xFF)
		{
			m_Control.flag_work = 0;
			if (infoLast.key == 0x39)
			{
				if (m_adr == 10)
				{
					if (infoLast.data == 1)
					{
						m_Global.micro_status.io &= ~(1 << bit_DF);
//						Serial.println("BF");
					}
					else if (infoLast.data == 2)
					{
						m_Global.micro_status.io |= (1 << bit_DF);
//						Serial.println("DF");
					}
				}
				else if (m_adr == 0)
				{
					if (infoLast.data == 1)
					{
						m_Global.micro_status.io &= ~(1 << bit_Mirror);
//						Serial.println("Mirror 1");
					}
					else if (infoLast.data == 2)
					{
						m_Global.micro_status.io |= (1 << bit_Mirror);
//						Serial.println("Mirror 2");
					}
				}
			}
			else if (infoLast.key == 1)
			{
				if (m_Global.micro_status.obj != (infoLast.data - 1) && (infoLast.data > 0) && (infoLast.data  <= 6))
				{
					if (m_Global.micro_status.obj < 6)
					{
						int32_t corr, step[3];

						if ((m_Global.micro_param.corr_enable & 1) && m_Global.micro_status.motor_base 
							&& (infoLast.data>0) && (infoLast.data < 6) && (!m_Joy.m_JoyWork))
						{
							m_Global.enc_corr = 0;
							for (uint8_t i = 0; i < 3; i++)
							{
								corr = m_Global.obj_offset[infoLast.data - 1][i] - m_Global.obj_offset[m_Global.micro_status.obj][i];
								corr = m_Motor[i].NmToStep(corr);
								step[i] = m_Motor[i].status.step_act;
								step[i] += corr;

								m_Motor[i].SetMode(step[i]);
							}

							m_Global.enc_corr = 1;
						}
					}

					m_Global.micro_status.obj = infoLast.data - 1;
					if (m_Global.micro_status.obj == 5)
					{
						if (!(m_Global.micro_status.io & (1 << bit_Mirror)))
						{
							delay(5000);
//							Serial.println("cmd Mirror On");
							info.dev_id = ID_OBJ_MCU;
							info.status = RS_WRITE;
							info.cmd = 0xD;
							info.key = 0x1D;
							info.data = 0;
							info.data0 = 0;
							writePackage((uint8_t*)&info);
							delay(1000);
						}
					}
				}
				m_Control.flag_obj = 0;
				Serial.print("Obj=");
				Serial.println(m_Global.micro_status.obj);
			}
		}
	}
	else if (infoLast.key == 0x3A)
	{
		if (infoLast.data == 1)
		{
			m_Global.micro_status.io &= ~((1 << bit_filtr1) | (1 << bit_filtr2));
			m_Global.micro_status.io |= (1 << bit_filtr1);
//			Serial.println("filtr1");
		}
		else if (infoLast.data == 2)
		{
			m_Global.micro_status.io &= ~((1 << bit_filtr1) | (1 << bit_filtr2));
			m_Global.micro_status.io |= (1 << bit_filtr2);
//			Serial.println("filtr2");
		}
		else if (infoLast.data == 3)
		{
			m_Global.micro_status.io &= ~((1 << bit_filtr1) | (1 << bit_filtr2));
//			Serial.println("filtr0");
		}
	}
	else if (infoLast.key == 0x3B)
	{
		if (infoLast.data == 2)
		{
			m_Global.micro_status.io &= ~((1 << bit_portUp1) | (1 << bit_portUp2));
			m_Global.micro_status.io |= (1 << bit_portUp1);
//			Serial.println("portUp1");
		}
		else if (infoLast.data == 3)
		{
			m_Global.micro_status.io &= ~((1 << bit_portUp1) | (1 << bit_portUp2));
			m_Global.micro_status.io |= (1 << bit_portUp2);
//			Serial.println("portUp2");
		}
		else if (infoLast.data == 1)
		{
			m_Global.micro_status.io &= ~((1 << bit_portUp1) | (1 << bit_portUp2));
//			Serial.println("portUp0");
		}
	}
	else if (infoLast.key == 0x3B)
	{
		if (infoLast.data == 1)
		{
			m_Global.micro_status.io &= ~(1 << bit_portDown);
//			Serial.println("portDown0");
		}
		else if (infoLast.data == 2)
		{
			m_Global.micro_status.io |= (1 << bit_portDown);
//			Serial.println("portDown1");
		}
	}
	else
	{
		if (m_Control.flag_work)
		{
//			Serial.print("Read adr=");
//			Serial.print(infoLast.key, HEX);
//			Serial.print(" data=");
//			Serial.println(infoLast.data, HEX);
			m_Control.flag_work = 0;
		}
		else
		{
			Serial.println("bad otvet data");
		}
	}
}

void TransferRs485::Control_blok(void)
{
	if (infoLast.status == RS_WRITE)
	{
		if (!infoLast.cmd)
		{
			info.dev_id = ID_CONTROL_PANEL;
			info.status = RS_WRITE;
			info.cmd = 0;
			info.key = infoLast.key;
			info.data = 0;
			info.data0 = 0;

			if (infoLast.key == 0x0E)
			{
				if (infoLast.data == 1)
				{
					info.data = 2;
//					Serial.println("stol_Up");
//					m_Motor[2].SetMode(1, VEL_POS_MODE); //Foton_old
					m_Marker.ActionLOW();
				}
				else if (infoLast.data == 2)
				{
					info.data = 1;
//					Serial.println("stol_Down");
//					m_Motor[2].SetMode(0, VEL_NEG_MODE); //Foton_old
					m_Marker.ActionHIGH();
				}
				else
				{
					;
				}
				writePackage((uint8_t*)&info);
				delay(1000);
			}
			else if (infoLast.key == 0x0F)
			{
				if (infoLast.data == 1)
				{
					info.data = 1;
//					Serial.println("stol_Load");
					uint8_t axis;
					for (axis = 0; axis < 3; axis++)
					{
//							rab32i = m_Motor[axis].NmToStep(m_TransferUsb.bufferRead.pak32.data[i]);
//							m_Motor[axis].SetMode(rab32i);
					}

					delay(50);

					for (axis = 0; axis < 3; axis++)
					{
						m_Motor[axis].WaiteStop();
					}
				}
				else if (infoLast.data == 2)
				{
					info.data = 2;
//					Serial.println("stol_Work");
				}
				else
				{
					;
				}
				writePackage((uint8_t*)&info);
				delay(1000);
			}
		}
		else if (infoLast.data == 106)
		{
//			Serial.println("naklon_L");
			m_Motor[7].SetMode(1, VEL_POS_MODE);
		}
		else if (infoLast.data == 107)
		{
//			Serial.println("naklon_R");
			m_Motor[7].SetMode(0, VEL_NEG_MODE);
		}
		else if ((infoLast.data == 6) || (infoLast.data == 7))
		{
			m_Motor[7].StopMotor();
		}
	}
	else if (infoLast.status == 5)
	{
		if (m_Control.flag_status == 1)
		{
//			Serial.print("Pol Status=");
//			Serial.println(infoLast.cmd);
			if (infoLast.cmd)
			{
				m_Control.flag_work = 1;
			}
			else
			{
				m_Control.flag_work = 0;
			}
			m_Control.flag_status = 0;
		}
/*
		else if (m_Control.flag_status == 2)
		{
			m_Global.micro_status.polar_ugol = infoLast.data;
			m_Global.micro_status.polar_ugol = m_Global.micro_status.polar_ugol << 8;
			m_Global.micro_status.polar_ugol += infoLast.key;
			m_Global.micro_status.polar_ugol = m_Global.micro_status.polar_ugol << 8;
			m_Global.micro_status.polar_ugol += infoLast.cmd;
			m_Control.flag_status++;
//			Serial.print("Pol Angle=");
//			Serial.println((int32_t)m_Global.microstatus.polar_ugol);
		}
		else if (m_Control.flag_status == 3)
		{
			m_Global.micro_status.polar_enc = infoLast.data;
			m_Global.micro_status.polar_enc = m_Global.micro_status.polar_enc << 8;
			m_Global.micro_status.polar_enc += infoLast.key;
			m_Global.micro_status.polar_enc = m_Global.micro_status.polar_enc << 8;
			m_Global.micro_status.polar_enc += infoLast.cmd;
			m_Control.flag_status = 0;
			m_Control.flag_work = 0;
//			Serial.print("Pol Enc=");
//			Serial.println((int32_t)m_Global.microstatus.polar_enc);
		}
*/
		else
		{
			m_Control.flag_work = 0;
			Serial.print("Pol error flag_status=0");
		}
	}
}

void TransferRs485::Polaroid(void)
{

}

void TransferRs485::Loop()
{
	if (!infoLast.status)
	{
		if (readPak())
		{
			if (infoLast.dev_id == ID_OBJ_MCU)
			{
				Obj_mcu();
			}
			else if (infoLast.dev_id == ID_CONTROL_PANEL)
			{
				Control_panel();
			}
			else if (infoLast.dev_id == ID_CONTROL_BLOCK)
			{
				Control_blok();
			}
			else if (infoLast.dev_id == ID_POLAROID)
			{
				Polaroid();
			}
		}
		infoLast.status = 0;
	}
}

void TransferRs485::setData(uint16_t index, uint16_t value)
{
	if (index > RS_GROUPS * RS_REGISTERS)
		return;

	rs_data[index] = value;
}


uint16_t TransferRs485::getData(uint16_t index)
{
	if (index > RS_GROUPS * RS_REGISTERS)
		return 0;

	return rs_data[index];
}


void TransferRs485::writePackage(uint8_t* info)
{
	info[0] = 0x23;
	uint16_t crc = calc_crc(info, RS485_PACKAGE_SIZE - 3);

	info[7] = (crc >> 8) & 0xFF;
	info[8] = (crc & 0xFF);

	digitalWrite(RS485_PIN_RXTX, HIGH);

//	Serial.println("Write485  ");

	for (int i = 0; i < RS485_PACKAGE_SIZE - 1; i++) {
		Serial2.write(info[i]);
		delayMicroseconds(100);

//		Serial.print(" ");
//		Serial.print(info[i], HEX);
	}
	Serial2.write(0x0D);

//	delayMicroseconds(RS_TX_DELAY);
	delayMicroseconds(100);
	digitalWrite(RS485_PIN_RXTX, LOW);

//	Serial.println(" 0x0D end paket");

//	for (int i = 0; i < 10; i++) {
//		delay(2);
//		readPak();
//	}
}

int TransferRs485::dataRead(uint8_t adr, int32_t* p_data, uint16_t timeout)
{
	int  error;
	package_info info;

	info.dev_id = 1;
	info.status = 1;
	info.cmd = 0xFF;
	info.key = adr;
	info.data = 0;
	info.data0 = 0;

	m_adr = adr;
	m_Control.flag_work = 1;
	writePackage((uint8_t*)&info);

	error = 1;
	while (timeout > 0)
	{
		timeout--;
		delay(100);
		Loop();
		if (!m_Control.flag_work)
		{
			*p_data = infoLast.data + info.data0*256;
//			Serial.print("read adr ");
//			Serial.print(adr);
//			Serial.print("=");
//			Serial.println(*p_data);
			error=0;
			break;
		}
	}
	m_Control.flag_work = 0;
	return error;
}

int TransferRs485::dataWrite(uint8_t adr, int32_t* p_data)
{
	package_info info;

	info.dev_id = 1;
	info.status = 2;
	info.cmd = 0xFF;
	info.key = adr;

	info.data = *p_data;

	writePackage((uint8_t*)&info);
	return 0;
}
