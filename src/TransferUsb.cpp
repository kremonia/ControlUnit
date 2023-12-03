#include "TransferUsb.h"

#include "Global.h"
extern Global m_Global;

#include "Control.h"
extern Control m_Control;

#include "CaCRC.h"
extern CaCRC m_aCRC;

void TransferUsb::Setup()
{
}

void TransferUsb::Send(void)
{
	uint16_t ksum;
	uint8_t i;

	ksum = m_aCRC.CRC16(bufferWrite.pak0.buf, bufferWrite.pak32.dlina - 2);
	bufferWrite.pak0.buf[bufferWrite.pak32.dlina - 2] = ksum & 0xFF;
	bufferWrite.pak0.buf[bufferWrite.pak32.dlina - 1] = ksum>>8;

	DDRF = 0xFF;  // on write
	for (i = 0; i < bufferWrite.pak32.dlina; i++) {
		m_DLPMega.writeByte(bufferWrite.pak0.buf[i]);
	}
	DDRF = 0x0;  // on read
}

int TransferUsb::ControlKSUM()
{
	uint16_t ksum, ksum_pak;
	
	ksum_pak = bufferRead.pak0.buf[bufferRead.pak32.dlina - 1];
	ksum_pak = ksum_pak << 8;
	ksum_pak |= bufferRead.pak0.buf[bufferRead.pak32.dlina - 2];

	ksum = m_aCRC.CRC16(bufferRead.pak0.buf, bufferRead.pak32.dlina - 2);

	if (ksum != ksum_pak)
	{
		Serial.println("bad ks");
		return 1;
	}
	if (!bufferRead.pak32.key)
	{
		return 2;
	}

	m_Global.micro_status.commanda_new = bufferRead.pak32.key;

	return 0;
}

void TransferUsb::Loop()
{
	byte inChar = 0;

	while (m_DLPMega.readByte(inChar))
	{

		input_index++;
		if (input_index > 64)
		{
			input_index = 0;
			Serial.println(" USB big input_index");
			break;
		}
		bufferRead.pak0.buf[input_index - 1] = inChar;

		if (input_index == 1)
		{
			if (inChar != 0x55)
			{
				Serial.print(inChar, HEX);
				Serial.println(" USB !=0x55");
				input_index = 0;
				break;
			}
		}
		else
		{
			if (input_index == bufferRead.pak32.dlina)
			{
//				Serial.println("");
				ControlKSUM();
				input_index = 0;
				break;
			}
		}
	}
}
