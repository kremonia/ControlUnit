#include "Camera.h"
#include "Global.h"

void Camera::Setup(void)
{
//	PORTK |= (1 << 5) | (1 << 6);
//	DDRK |= (1 << 5) | (1 << 6);

	PORTK |= (1 << 6);
	DDRK |= (1 << 6);
}

void  Camera::Action(void)
{
	PORTK &= ~(1 << 6);
	delayMicroseconds(15000);
	PORTK |= (1 << 6);
/*
	PORTK &= ~((1 << 5) | (1 << 6));
	delayMicroseconds(100);
	PORTK |= (1 << 5) | (1 << 6);
*/
}
