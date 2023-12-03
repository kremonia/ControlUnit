#pragma once
//#define Klap_Marker			 9	//Marker
// �� ��������� ������ �� 4 ������

#include "Global.h"
extern Global m_Global;

class Marker
{
public:
	void Setup(void);
	void Action(void);
	void ActionHIGH(void);
	void ActionLOW(void);
};
