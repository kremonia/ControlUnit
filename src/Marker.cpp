#include "Marker.h"
#include "Global.h"

void Marker::Setup(void)
{
	pinMode(Klap_Marker, OUTPUT);
	digitalWrite(Klap_Marker, LOW);
}

void  Marker::Action(void)
{
	digitalWrite(Klap_Marker, HIGH);
	delay(m_Global.micro_param.marker_impuls);
	digitalWrite(Klap_Marker, LOW);

//	Serial.print("Marker ");
//	Serial.println(Klap_Marker);
}

void  Marker::ActionHIGH(void)
{
	digitalWrite(Klap_Marker, HIGH);

//	Serial.print("Marker HIGH ");
//	Serial.println(Klap_Marker);
}

void  Marker::ActionLOW(void)
{
	digitalWrite(Klap_Marker, LOW);

//	Serial.print("Marker LOW ");
//	Serial.println(Klap_Marker);
}
