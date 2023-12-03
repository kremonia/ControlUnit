#include "Motor.h"

#include "TransferRs485.h"

extern TransferRs485 m_TransferRs485;


/// Converts nm to steps of the motor
/// \param nm length in nm
/// \return amount of steps
int32_t Motor::NmToStep(int32_t nm) {
    int64_t rabi64;

    rabi64 = nm;
    rabi64 *= param.oborot_step;
    rabi64 += param.oborot_value / 2;
    rabi64 /= param.oborot_value;
    return rabi64;
}

/// Converts steps of the motor to nm
/// \param step amount of steps
/// \return length in nm
int32_t Motor::StepToNm(int32_t step) {
    int64_t rabi64{step};

//    rabi64 = step;
    rabi64 *= param.oborot_value;
    rabi64 += param.oborot_step / 2;
    rabi64 /= param.oborot_step;
    return rabi64;
}

void Motor::Setup(void) {
    if (!param.tip) {
        Serial.print("init motorTMC=");
        Serial.println(param.csPin);
        motorTMC.initSPI();
        motorTMC.init(param.csPin);
        motorTMC.set_AMAX(0xFFFF);
        motorTMC.set_IHOLD_IRUN(param.ihold, param.current, 0);
        motorTMC.set_ENC_CONST(0x1412c0);
        motorTMC.set_enc_sel_decimal(1);
    } else if (param.tip == 1) {
/*
		Serial.print("init motorDC=");
		Serial.println(param.csPin);
		motorDC.initSPI();
		motorDC.init(param.csPin, 0x20);
		Loop();

		// ���������� ������������
		m_TransferRs485.info.dev_id = ID_CONTROL_PANEL;
		m_TransferRs485.info.status = RS_WRITE;
		m_TransferRs485.info.cmd = 0;
		m_TransferRs485.info.key = 0x0E;
		m_TransferRs485.info.data0 = 0;

		if (status.sensor & (1 << PINS_DC_SENS_UP))
		{
			m_TransferRs485.info.data = 2;
		}
		else if (status.sensor & (1 << PINS_DC_SENS_DOWN))
		{
			m_TransferRs485.info.data = 1;
		}
		m_TransferRs485.writePackage((uint8_t*)&m_TransferRs485.info);

		delay(100);
*/
    } else if (param.tip == 2) {

    }

    motorTMC.initSPI();
}

void Motor::Loop(void) {
    uint8_t rab8u = 0;

    if (!param.tip) {
        uint32_t step;

        rab8u = motorTMC.readREG(TMC_REG_X_ENC, (uint32_t * ) & (status.enc_act));
        rab8u = motorTMC.readREG(TMC_REG_XACTUAL, (uint32_t * ) & step);
        status.status = rab8u;
        /*
        if (param.oborot_enc)
        {
            rab8u = motorTMC.readREG(TMC_REG_X_ENC, (uint32_t*)&(status.enc_act));
            enc_step = status.enc_act / 8;
            err = enc_step - status.step_act;
            if (abs(err) > 1)
            {
                motorTMC.set_XACTUAL(enc_step);
            }
        }
*/
        status.vel_act = motorTMC.get_VACTUAL();
        if (!(rab8u & (1 << SpiStatus::STANDSTILL))) {
            status.vel_act++;
        }
        status.step_act = step;
    } else {
/*
		motorDC.initSPI();
		rab8u = motorDC.digital_read(PINS_DC_SENS_UP);
		if (rab8u) {
			status.sensor &= ~(1 << PINS_DC_SENS_UP);
		}
		else {
			status.sensor |= (1 << PINS_DC_SENS_UP);
			if (status.mode == VEL_POS_MODE)
			{
				StopMotor();
			}
		}

		rab8u = motorDC.digital_read(PINS_DC_SENS_DOWN);
		if (rab8u) {
			status.sensor &= ~(1 << PINS_DC_SENS_DOWN);
		}
		else {
			status.sensor |= (1 << PINS_DC_SENS_DOWN);
			if (status.mode == VEL_NEG_MODE)
			{
				StopMotor();
			}
		}

		rab8u = motorDC.digital_read(PINS_DC_SENS_MIDDLE);
		if (rab8u) {
			status.sensor &= ~(1 << PINS_DC_SENS_MIDDLE);
		}
		else {
			status.sensor |= (1 << PINS_DC_SENS_MIDDLE);
		}
*/
        motorTMC.initSPI();
    }
}

void Motor::SetMode(uint32_t position, uint8_t mode, uint8_t vel_proc) {
    uint64_t rab64u;
    int32_t vmax;

    status.mode = mode;
    status.vel_proc = vel_proc;
    status.step_dest = position;

    rab64u = param.vel_max;
    rab64u *= vel_proc;
    rab64u /= 100;
    vmax = rab64u;

    if (!param.tip) {
        if (!mode) {
            motorTMC.set_XTARGET(position);
        }
        motorTMC.set_RAMPMODE(mode);
        motorTMC.set_VMAX(vmax);
        delay(2);
    } else if (param.tip == 1) {
/*
		motorDC.initSPI();
		delay(1);
		if (status.mode == VEL_POS_MODE)
		{
			motorDC.digital_write(DC_PINA, 1);
			motorDC.digital_write(DC_PINB, 0);
		}
		else if (status.mode == VEL_NEG_MODE)
		{
			motorDC.digital_write(DC_PINA, 0);
			motorDC.digital_write(DC_PINB, 1);
		}
		motorTMC.initSPI();
*/
    }
}

void Motor::StopMotor(void) {
    status.vel_act = 0;

    if (!param.tip) {
        motorTMC.initSPI();
        delay(1);
        motorTMC.set_VMAX(0);
        motorTMC.set_RAMPMODE(POSITION_MODE);
    } else if (param.tip == 1) {
/*
		motorDC.initSPI();
		delay(1);
		motorDC.digital_write(DC_PINA, 0);
		motorDC.digital_write(DC_PINB, 0);
*/
    }

    motorTMC.initSPI();
}

int8_t Motor::WaiteStop() {
    uint32_t timeout;
    int32_t step_error = 0;

    timeout = param.timeout;
    timeout += millis();
    while (timeout >= millis()) {
        m_TransferUsb.Loop();
        m_Control.Loop();

        if (m_Global.micro_status.is_Stop) {
            Serial.println("microstatus.is_Stop");
            StopMotor();
            m_Global.micro_status.is_Stop = 0;
            return 4;
        }

        Loop();
        step_error = status.step_dest - status.step_act;
        if ((abs(step_error) <= param.step_error) || !status.vel_act) {
            status.vel_act = 0;
            return 0;
        }
    }

    Serial.print("timeout WaiteStop csPin=");
    Serial.print(param.csPin);
    Serial.print(" step_act=");
    Serial.print(status.step_act);
    Serial.print(" step_dest=");
    Serial.print(status.step_dest);
    Serial.print(" step_error=");
    Serial.print(step_error);
    Serial.print(" vel_act=");
    Serial.println(status.vel_act);
    status.vel_act = 0;
    return 4;
}

int8_t Motor::HomeSet(int32_t value) {
    StopMotor();

    if (!param.tip) {
        motorTMC.initSPI();
        motorTMC.set_VMAX(0);
        motorTMC.set_RAMPMODE(POSITION_MODE);
        motorTMC.set_XACTUAL(value);
        motorTMC.set_XTARGET(value);
        motorTMC.set_X_ENC(value);
    }

    status.vel_act = 0;
    status.step_dest = 0;
    status.step_act = 0;
    status.enc_act = 0;

    return 0;
}

int8_t Motor::HomePosition() {
    uint16_t timeout;

    StopMotor();

    Serial.print("Start Home ");
    Serial.println(param.csPin);

    HomeSet(0);

    SetMode(0, VEL_NEG_MODE, 100);
    timeout = 600;
    while (timeout) {
        delay(100);
        Loop();
        if (!status.vel_act) {
            break;
        }
        timeout--;
    }

    if (!timeout) {
        Serial.print("timeout Stop csPin=");
        Serial.println(param.csPin);
        status.vel_act = 0;
        return 1;
    }

    HomeSet(0);

    Serial.print("otstup csPin=");
    Serial.println(param.csPin);
    SetMode(NmToStep(500000));

    if (WaiteStop()) {
        Serial.print("timeout Set csPin=");
        Serial.println(param.csPin);
        status.vel_act = 0;
        return 1;
    }

    SetMode(0, VEL_NEG_MODE, 50);
    timeout = 100;
    while (timeout) {
        delay(100);
        Loop();
        if (!status.vel_act) {
            break;
        }
        timeout--;
    }

    if (!timeout) {
        Serial.print("timeout2 Stop csPin=");
        Serial.println(param.csPin);
        status.vel_act = 0;
        return 1;
    }

    HomeSet(0);

    Serial.print("otstup2 csPin=");
    Serial.println(param.csPin);

    SetMode(NmToStep(500000));
    if (WaiteStop()) {
        status.vel_act = 0;
        Serial.print("timeout otstup2 csPin=");
        Serial.println(param.csPin);
        return 1;
    }

    Serial.print("Good Home axis=");
    Serial.println(param.csPin);
    return 0;
}
