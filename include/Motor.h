#pragma once

#include "Global.h"

extern Global m_Global;

#include "TransferUsb.h"

extern TransferUsb m_TransferUsb;

#include "Control.h"

extern Control m_Control;

#include "Trinamic_TMC5130.h"
//#include "mcp23s08.h"

class Motor {
public:
    void Setup(void);

    void Loop(void);

    void SetMode(uint32_t position, uint8_t mode = 0, uint8_t vel_proc = 100);

    void StopMotor(void);

    int8_t WaiteStop();

    int8_t HomePosition(void);

    int8_t HomeSet(int32_t value);

    int32_t NmToStep(int32_t nm);

    int32_t StepToNm(int32_t step);

    motor_param_t param;
    motor_status_t status;
    Trinamic_TMC5130 motorTMC;
//	mcp23s08 motorDC;
};
