#include "Keybord.h"
#include <avr/eeprom.h>

#include "Global.h"
extern Global m_Global;
#include "Control.h"
extern Control m_Control;
#include "Marker.h"
extern Marker m_Marker;
#include "Motor.h"
extern Motor m_Motor[8];
#include "Vacuum.h"
extern Vacuum m_Vacuum;

#define KEYS  12
#define UPDTE_KEYS_MS       50	// ms

char hexaKeys[4][3] = {
//	{ 1,2,3 },
//	{ 4,5,6 },
//	{ 7,8,9 },
//	{ 10,0,11 }
	{ 2,1,3 },
	{ 5,4,6 },
	{ 8,7,9 },
	{ 0,10,11 }
};

byte KeyPad_Row_Pins[4] = { 39,40,41,42 };//{50, 49, 48, 47}; //connect to the row pinouts of the keypad
byte KeyPad_Col_Pins[3] = { 43,44,45 };//{46, 45, 44}; //connect to the column pinouts of the keypad

void Keybord::Setup()
{
	keypad = new Keypad(makeKeymap(hexaKeys), KeyPad_Row_Pins, KeyPad_Col_Pins, 4, 3);
}

void Keybord::Loop()
{
	GetStatusKey();
}

void Keybord::GetStatusKey()
{
	if (millis() - lastTimeKey < UPDTE_KEYS_MS)
	{
		return;
	}
	
	m_Global.micro_status.key = 0;

	keypad->getKeys();
	for (int i = 0; i < KEYS; i++)
	{
		if (keypad->isPressed(i))
		{
			bitSet(m_Global.micro_status.key, i);
			Serial.println("key pressed");
		}
	}

	lastTimeKey = millis();
}
