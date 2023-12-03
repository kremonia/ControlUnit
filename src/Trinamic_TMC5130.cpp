#include "Trinamic_TMC5130.h"
#define TMC_REGS             30

const uint8_t addr_tmc_regs[TMC_REGS] PROGMEM = {
	TMC_REG_XTARGET,
	TMC_REG_GCONF,
	TMC_REG_IHOLD_IRUN,
	TMC_REG_TPOWERDOWN,
	TMC_REG_TPWMTHRS,
	TMC_REG_TCOOLTHRS,
	TMC_REG_THIGH,
	TMC_REG_RAMPMODE,
	TMC_REG_XACTUAL,
	TMC_REG_VACTUAL,
	TMC_REG_VSTART,
	TMC_REG_A1,
	TMC_REG_V1,
	TMC_REG_AMAX,
	TMC_REG_VMAX,
	TMC_REG_DMAX,
	TMC_REG_D1,
	TMC_REG_VSTOP,
	TMC_REG_TZEROWAIT,
	TMC_REG_XTARGET,
	TMC_REG_VDCMIN,
	TMC_REG_SW_MODE,
	TMC_REG_ENCMODE,
	TMC_REG_X_ENC,
	TMC_REG_ENC_CONST,
	TMC_REG_CHOPCONF,
	TMC_REG_COOLCONF,
	TMC_REG_DCCTRL,
	TMC_REG_PWMCONF,
	TMC_REG_ENCM_CTRL
};

// diffrent mode 
const uint32_t stealthChop[TMC_REGS] PROGMEM = {
	0,          //TMC_REG_XTARGET
	4,          // TMC_REG_GCONF
	334095,     //657930,334095,     //TMC_REG_IHOLD_IRUN
	10,         // //TMC_REG_TPOWERDOWN
	10,         //TMC_REG_TPWMTHRS
	20,         //TMC_REG_TCOOLTHRS
	4,          //TMC_REG_THIGH
	2,          //TMC_REG_RAMPMODE
	0,          //TMC_REG_XACTUAL
	0,          //TMC_REG_VACTUAL
	5,          //TMC_REG_VSTART
	100,       //TMC_REG_A1
	1000,      //TMC_REG_V1
	50000,      //TMC_REG_AMAX
	0,          //TMC_REG_VMAX
	50000,      //7000, //TMC_REG_DMAX
	30000,       //1400 TMC_REG_D1
	10,         //TMC_REG_VSTOP
	0,          //TMC_REG_TZEROWAIT
	0,          //TMC_REG_XTARGET
	0,          //TMC_REG_VDCMIN
	15,         //TMC_REG_SW_MODE
	1599,       //TMC_REG_ENCMODE
	0,          //TMC_REG_X_ENC
	10000,      //1600(3200),400000 200000  TMC_REG_ENC_CONST
	197077,     //229620, 98548, TMC_REG_CHOPCONF
	0,          //TMC_REG_COOLCONF
	0,          //TMC_REG_DCCTRL
	1376775,    //1900815,//262407,//TMC_REG_PWMCONF
	0,          //TMC_REG_ENCM_CTRL
};

const uint32_t coolConf[TMC_REGS] PROGMEM = {
	0,          //TMC_REG_XTARGET
	0,			// TMC_REG_GCONF
	7680,       //657930,334095,     //TMC_REG_IHOLD_IRUN
	0,          // //TMC_REG_TPOWERDOWN
	100000,     //TMC_REG_TPWMTHRS
	1000000,    //TMC_REG_TCOOLTHRS
	3,          //TMC_REG_THIGH
	2,          //TMC_REG_RAMPMODE
	0,          //TMC_REG_XACTUAL
	0,          //TMC_REG_VACTUAL
	5,          //TMC_REG_VSTART
	100,        //TMC_REG_A1
	1000,       //TMC_REG_V1
	30000,      //TMC_REG_AMAX
	0,          //TMC_REG_VMAX
	50000,      //7000, //TMC_REG_DMAX
	30000,      //1400 TMC_REG_D1
	10,         //TMC_REG_VSTOP
	0,          //TMC_REG_TZEROWAIT
	0,          //TMC_REG_XTARGET
	0,          //TMC_REG_VDCMIN
	15,         //TMC_REG_SW_MODE
	1599,       //TMC_REG_ENCMODE
	0,          //TMC_REG_X_ENC
	0x1412c0,      // TMC_REG_ENC_CONST	 
				   // The encoder constant register shall be programmed to 51.2 in decimal mode. 
				   // Therefore, set ENC_CONST = 51 ? 2e16 + 0.2 ? 10000
				   // Factor = FSC*USC / encoder resolution: 
				   // fsc=400 usc=256 encoder resolution = 5000 ??????? ?? ??????
	198293,     //239252,//196949,//229620,     ////98548, //TMC_REG_CHOPCONF
	20717635,   //1671203,          //TMC_REG_COOLCONF
	0,          //TMC_REG_DCCTRL
	1900812,    //262407,     //TMC_REG_PWMCONF
	0,          //TMC_REG_ENCM_CTRL
};


void Trinamic_TMC5130::initSPI(void)
{
	// initialize SPI
	SPI.end();
	SPI.setDataMode(TMC_SPI_DATA_MODE);
	SPI.setBitOrder(TMC_SPI_BIT_ORDER);
	SPI.setClockDivider(TMC_SPI_CLOCK_DIVIDER);
	SPI.begin();
}

// initialize the driver with its CS/SS pin
void Trinamic_TMC5130::init(uint8_t param_cs)
{
	csPin = param_cs;
	status = 0;
	pinMode(csPin, OUTPUT);
	digitalWrite(csPin, HIGH);
    digitalWrite(csPin, LOW);

//	TmcInitRegisters(Mode::STEALTH_CHOP);
	TmcInitRegisters(Mode::COOL_STEP);
}

void Trinamic_TMC5130::TmcInitRegisters(byte mode)
{
	uint32_t value;
	uint8_t address;

	for (int i = 0; i < TMC_REGS; i++)
	{
		address = pgm_read_byte(addr_tmc_regs + i);
		if (mode == Mode::COOL_STEP)
		{
			value = pgm_read_dword(coolConf + i);
		}
		else if (mode == Mode::STEALTH_CHOP)
		{
			value = pgm_read_dword(stealthChop + i);
		}
		writeREG(address, value);
	}

	writeREG(TMC_REG_MSLUT0, 0xAAAAB554);
	writeREG(TMC_REG_MSLUT1, 0x4A9554AA);
	writeREG(TMC_REG_MSLUT2, 0x24492929);
	writeREG(TMC_REG_MSLUT3, 0x10104222);
	writeREG(TMC_REG_MSLUT4, 0xFBFFFFFF);
	writeREG(TMC_REG_MSLUT5, 0xB5BB777D);
	writeREG(TMC_REG_MSLUT6, 0x49295556);
	writeREG(TMC_REG_MSLUT7, 0x00404222);
	writeREG(TMC_REG_MSLUTSEL, 0xFFFF8056);
	writeREG(TMC_REG_MSLUTSTART, 0x00F70000);
	set_sgt(0);

	set_V1(0);
	set_D1(0xFFFF);
	set_DMAX(0xFFFF);
	set_VSTART(0);
	set_VSTOP(1);
	set_seimin(1);
	set_sedn(0);
	set_seup(0);
	set_TPOWERDOWN(0);
	set_TPWMTHRS(300);
	set_TCOOLTHRS(295);

	set_en_pwm_mode(1);

	// 	chopper mode    0 Standard mode(spreadCycle)
	// 					1 Constant off time with fast decay time.
	// 					Fast decay time is also terminated when the
	// 					negative nominal current is reached.Fast decay is
	// 					after on time.
	set_chm(0);
	set_sfilt(0); // 
	set_X_ENC(0);
	set_IHOLD_IRUN(5, 31, 0);
	set_VMAX(0);
	set_THIGH(50);
}

// read status
uint8_t Trinamic_TMC5130::readSTAT()
{
  digitalWrite(csPin, LOW);

  // read address
  status = SPI.transfer(0x00);
  // flush 4 bytes
  for(int i=0; i<4; i++){
    SPI.transfer(0x00);
  }
  digitalWrite(csPin, HIGH);

  return status;
}

// read a register
uint8_t Trinamic_TMC5130::readREG(uint8_t address, uint32_t *data)
{
	uint8_t res = 0;

	digitalWrite(csPin, LOW);

	SPI.transfer(address & ~TMC_WRITE);
	for (int i = 0; i < 4; i++)
	{
		SPI.transfer(0x00);
	}
	
	digitalWrite(csPin, HIGH);

	digitalWrite(csPin, LOW);

	res = SPI.transfer(address & ~TMC_WRITE);

	*data  = SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);

	digitalWrite(csPin, HIGH);
	//Serial.println("read");
	return res;
}


uint8_t Trinamic_TMC5130::readSignedREG( uint8_t address,int32_t *data )
{
	//init_SPI();
	digitalWrite(csPin, LOW);

	// read address
	status = SPI.transfer(address & ~TMC_WRITE);

	// flush 4 bytes
	for(int i=0;i<4;i++){
		SPI.transfer(0x00);
	}

	digitalWrite(csPin, HIGH);
	// restart transmission
	digitalWrite(csPin, LOW);

	// read address
	status = SPI.transfer(address & ~TMC_WRITE);

	// retrieve data
	*data  = SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);
	*data <<=8;
	*data |= SPI.transfer(0x00);

	digitalWrite(csPin, HIGH);
	//Serial.println("read");

	return status;
}

// write to a register
uint8_t Trinamic_TMC5130::writeREG(uint8_t address, uint32_t data)
{
  digitalWrite(csPin, LOW);

  // write address
  status = SPI.transfer(address|TMC_WRITE);

  // write data
  SPI.transfer((data>>24UL)&0xFF);
  SPI.transfer((data>>16UL)&0xFF);
  SPI.transfer((data>> 8UL)&0xFF);
  SPI.transfer((data>> 0UL)&0xFF);

  digitalWrite(csPin, HIGH);

  //Serial.println("write");

  return status;
}

// alter a register using a bitmask
uint8_t Trinamic_TMC5130::alterREG(uint8_t address, uint32_t data, uint32_t mask)
{
  uint32_t oldData, newData;

  readREG( address, &oldData );

  newData = ( oldData&~mask ) | ( data&mask );

  writeREG( address, newData );

  return status;
}

// set single bits in the GCONF register
uint8_t Trinamic_TMC5130::setGCONF(uint8_t position, uint8_t value)
{
  alterREG(TMC_REG_GCONF, uint32_t(value)<<position, 0x1UL<<position);

  return status;
}

// set single bits or values in the chopconf register (constraining masks are applied if necessary)
uint8_t Trinamic_TMC5130::set_CHOPCONF(uint8_t position, uint8_t value)
{
  alterREG(TMC_REG_CHOPCONF, uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position);

  return status;
}

  //////////
 // GCONF
//////////

uint8_t Trinamic_TMC5130::set_I_scale_analog(uint8_t value)
{
  setGCONF(TMC_GCONF_I_SCALE_ANALOG, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_internal_rsense(uint8_t value)
{
  setGCONF(TMC_GCONF_INTERNAL_RSENSE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_en_pwm_mode(uint8_t value)
{
  setGCONF(TMC_GCONF_EN_PWM_MODE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_enc_commutation(uint8_t value)
{
  setGCONF(TMC_GCONF_ENC_COMMUTATION, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_shaft(uint8_t value)
{
  setGCONF(TMC_GCONF_SHAFT, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag0_error(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG0_ERROR, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag0_otpw(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG0_OTPW, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag0_stall(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG0_STALL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag1_stall(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG1_STALL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag1_index(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG1_INDEX, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag1_onstate(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG1_ONSTATE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag1_steRS_skipped(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG1_STERS_SKIPPED, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag0_int_pushpull(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG0_INT_PUSHPULL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_diag1_int_pushpull(uint8_t value)
{
  setGCONF(TMC_GCONF_DIAG1_INT_PUSHPULL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_small_hysteresis(uint8_t value)
{
  setGCONF(TMC_GCONF_SMALL_HYSTERESIS, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_stop_enable(uint8_t value)
{
  setGCONF(TMC_GCONF_STOP_ENABLE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_direct_mode(uint8_t value)
{
  setGCONF(TMC_GCONF_DIRECT_MODE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_X_COMPARE(uint32_t value)
{
	uint32_t data;

	data = value & TMC_X_COMPARE_MASK;

	writeREG(TMC_REG_X_COMPARE, data);

	return status;
}

/*
uint8_t Trinamic_TMC2130::set_test_mode(uint8_t value)
{
  set_GCONF(TMC_GCONF_TEST_MODE, value);

  return _status;
}
*/

//////////
// IHOLD_IRUN
//////////

uint8_t Trinamic_TMC5130::set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay)
{
  uint32_t data;
  // adding ihold
  data = (( uint32_t(ihold)&TMC_IHOLD_MASK )<<TMC_IHOLD );
  // adding irun
  data |= (( uint32_t(irun)&TMC_IRUN_MASK )<<TMC_IRUN );
  // adding iholddelay
  data |= (( uint32_t(iholddelay)&TMC_IHOLDDELAY_MASK )<<TMC_IHOLDDELAY );
 
  // writing data
  writeREG(TMC_REG_IHOLD_IRUN, data);

  return status;
}

//////////
// TPOWERDOWN
//////////

uint8_t Trinamic_TMC5130::set_TPOWERDOWN(uint8_t value)
{
  uint32_t data;

  data = value & TMC_TPOWERDOWN_MASK;

  writeREG(TMC_REG_TPOWERDOWN, data);

  return status;
}

//////////
// TSTEP
//////////

uint32_t Trinamic_TMC5130::get_TSTEP()
{
  uint32_t data;

  readREG(TMC_REG_TSTEP, &data);

  data &= TMC_TSTEP_MASK;

  return data;
}

//////////
// TPWMTHRS
//////////

uint8_t Trinamic_TMC5130::set_TPWMTHRS(uint32_t value)
{
  uint32_t data;

  data = value & TMC_TPWMTHRS_MASK;

  writeREG(TMC_REG_TPWMTHRS, data);

  return status;
}

//////////
// TCOOLTHRS
//////////

uint8_t Trinamic_TMC5130::set_TCOOLTHRS(uint32_t value)
{
  uint32_t data;

  data = value & TMC_TCOOLTHRS_MASK;

  writeREG(TMC_REG_TCOOLTHRS, data);

  return status;
}

//////////
// THIGH
//////////

uint8_t Trinamic_TMC5130::set_THIGH(uint32_t value)
{
  uint32_t data;

  data = value & TMC_THIGH_MASK;

  writeREG(TMC_REG_THIGH, data);

  return status;
}


uint8_t Trinamic_TMC5130::set_RAMPMODE( uint8_t value )
{
	uint32_t data;
	data = value & TMC_RAMPMODE_MASK;
	writeREG(TMC_REG_RAMPMODE,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_XACTUAL( uint32_t value )
{
	uint32_t data;
	data = value & TMC_XACTUAL_MASK;
	writeREG(TMC_REG_XACTUAL,data);
	return status;
}


uint32_t Trinamic_TMC5130::get_XACTUAL()
{
	uint32_t data; // oh baby return int get uinsigned 

	//readSignedREG(TMC_REG_XACTUAL, &data);
	readREG(TMC_REG_XACTUAL,&data);

	//data &= TMC_VACTUAL_MASK;

	return data;
}

int32_t Trinamic_TMC5130::get_VACTUAL()
{
	int32_t data; 

	//readSignedREG(TMC_REG_VACTUAL, &datas);
	readSignedREG(TMC_REG_VACTUAL,&data);

	if(data<0){
		return data;
	}
	// sign velocity 
	if(data>8388607){ // this magic number is 2^23-1
		data = data - 16777215; // this magic number is 2^24
	}
	//data &= TMC_VACTUAL_MASK;

	return data;
}

// 
// uint8_t Trinamic_TMC5130::set_VACTUAL( uint32_t value )
// {
// 	uint32_t data;
// 	data = value &TMC_VACTUAL_MASK;
// 	writeREG(TMC_REG_VACTUAL,data);
// 	return _status;
// }


uint8_t Trinamic_TMC5130::set_VSTART( uint32_t value )
{
	uint32_t data;
	data = value &TMC_VSTART_MASK;
	writeREG(TMC_REG_VSTART,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_A1( uint16_t value )
{
	uint32_t data;
	data = value &TMC_A1_MASK;
	writeREG(TMC_REG_A1,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_V1( uint32_t value )
{
	uint32_t data;
	data = value &TMC_V1_MASK;
	writeREG(TMC_REG_V1,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_AMAX( uint16_t value )
{
	uint32_t data;
	data = value &TMC_AMAX_MASK;
	writeREG(TMC_REG_AMAX,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_VMAX( uint32_t value )
{
	uint32_t data;
	data = value &TMC_VMAX_MASK;
	writeREG(TMC_REG_VMAX,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_DMAX( uint16_t value )
{
	uint32_t data;
	data = value &TMC_DMAX_MASK;
	writeREG(TMC_REG_DMAX,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_D1( uint16_t value )
{
	uint32_t data;
	data = value &TMC_D1_MASK;
	writeREG(TMC_REG_D1,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_VSTOP( uint32_t value )
{
	uint32_t data;
	data = value &TMC_VSTOP_MASK;
	writeREG(TMC_REG_VSTOP,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_TZEROWAIT( uint16_t value )
{
	uint32_t data;
	data = value &TMC_TZEROWAIT_MASK;
	writeREG(TMC_REG_TZEROWAIT,data);
	return status;
}


uint8_t Trinamic_TMC5130::set_XTARGET( int32_t value )
{
	uint32_t data;
	data = value & TMC_XTARGET_MASK;
	writeREG(TMC_REG_XTARGET, data);
	return status;
}

// unsigned make
uint32_t Trinamic_TMC5130::get_XTARGET()
{
	uint32_t data; // oh baby return int get uinsigned 

	readREG(TMC_REG_XDIRECT, &data);

	data &= TMC_XDIRECT_MASK;

	return data;
}

//////////
// XDIRECT
//////////
// only tmc2130
uint8_t Trinamic_TMC5130::set_XDIRECT(int16_t coil_a, int16_t coil_b)
{
  uint32_t data;

  data = 0x0;
  data |= ( coil_b & TMC_XDIRECT_COIL_B_MASK );
  data = data << TMC_XDIRECT_COIL_B;
  data |= ( coil_a & TMC_XDIRECT_COIL_A_MASK );
  data &= TMC_XDIRECT_MASK;

  writeREG(TMC_REG_XDIRECT, data);

  return status;
}
// only tmc2130
uint8_t Trinamic_TMC5130::set_XDIRECT(uint32_t value)
{
  uint32_t data;

  data = value;// & TMC_XDIRECT_MASK;

  writeREG(TMC_REG_XDIRECT, data);

  return status;
}
// only tmc2130
int32_t Trinamic_TMC5130::get_XDIRECT()
{
  uint32_t data; // oh baby return int get uinsigned 

  readREG(TMC_REG_XDIRECT, &data);

  data &= TMC_XDIRECT_MASK;

  return data;
}

//////////
// VDCMIN
//////////

uint8_t Trinamic_TMC5130::set_VDCMIN(int32_t value)
{
  int32_t data;

  data = value & TMC_VDCMIN_MASK;

  writeREG(TMC_REG_VDCMIN, data);

  return status;
}


uint8_t Trinamic_TMC5130::set_SW_MODE( uint8_t position, uint8_t value )
{
	alterREG(TMC_REG_SW_MODE,uint32_t(value)<<position, 0x1UL<<position);

	return status;
	
}


uint8_t Trinamic_TMC5130::set_en_softstop( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_EN_SOFTSTOP, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_sg_stop( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_SG_STOP, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_en_latch_encoder( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_EN_LATCH_ENCODER, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_latch_r_inactive( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_LATCH_R_INACTIVE, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_latch_r_active( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_LATCH_R_ACTIVE, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_latch_l_inactive( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_LATCH_L_INACTIVE, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_latch_l_active( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_LATCH_L_ACTIVE, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_swap_lr( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_SWAP_LR, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_pol_stop_r( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_POL_STOP_R, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_pol_stop_l( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_POL_STOP_L, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_stop_r_enable( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_STOP_R_ENABLE, value);

	return status;
}


uint8_t Trinamic_TMC5130::set_stop_l_enable( uint8_t value )
{
	set_SW_MODE(TMC_SW_MODE_STOP_L_ENABLE, value);

	return status;
}


uint8_t Trinamic_TMC5130::get_RAMP_STAT( uint8_t position )
{
	uint32_t data;

	readREG(TMC_REG_RAMP_STAT, &data);

	return bitRead(data,position);
}


uint8_t Trinamic_TMC5130::get_status_stop_l()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_STATUS_STOP_L);
}


uint8_t Trinamic_TMC5130::get_status_stop_r()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_STATUS_STOP_R);
}


uint8_t Trinamic_TMC5130::get_status_latch_l()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_STATUS_LATCH_L);
}


uint8_t Trinamic_TMC5130::get_status_latch_r()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_STATUS_LATCH_R);
}


uint8_t Trinamic_TMC5130::get_event_stop_l()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_EVENT_STOP_L);
}


uint8_t Trinamic_TMC5130::get_event_stop_r()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_EVENT_STOP_R);
}


uint8_t Trinamic_TMC5130::get_event_pos_reached()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_EVENT_POS_REACHED);
}


uint8_t Trinamic_TMC5130::get_velocity_reached()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_VELOCITY_REACHED);
}


uint8_t Trinamic_TMC5130::get_position_reached()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_POSITION_REACHED);
}


uint8_t Trinamic_TMC5130::get_vzero()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_VZERO);
}


uint8_t Trinamic_TMC5130::get_t_zerowait_active()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_T_ZEROWAIT_ACTIVE);
}


uint8_t Trinamic_TMC5130::get_second_move()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_SECOND_MOVE);
}


uint8_t Trinamic_TMC5130::get_status_sg()
{
	return get_RAMP_STAT(TMC_RAMP_STAT_STATUS_SG);
}

uint32_t Trinamic_TMC5130::get_XLATCH()
{
	uint32_t data;

	readREG(TMC_REG_XLATCH, &data);

	data &= TMC_XLATCH_MASK;

	return data;
}


uint8_t Trinamic_TMC5130::set_ENCMODE( uint8_t position,uint8_t value )
{
	alterREG(TMC_REG_ENCMODE, uint32_t(value)<<position, 0x1UL<<position);

	return status;
}


uint8_t Trinamic_TMC5130::set_enc_sel_decimal( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_ENC_SEL_DECIMAL,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_latch_x_act( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_LATCH_X_ACT,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_clr_enc_x( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_CLR_ENC_X,value);
	return status;
}

uint8_t Trinamic_TMC5130::set_neg_edge( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_NEG_EDGE,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_pos_edge( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_POS_EDGE,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_clr_once( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_CLR_ONCE,value);
	return status;
}

uint8_t Trinamic_TMC5130::set_clr_cont( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_CLR_CONT,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_ignore_AB( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_IGNORE_AB,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_pol_N( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_POL_N,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_pol_B( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_POL_B,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_pol_A( uint8_t value )
{
	set_ENCMODE(TMC_ENCMODE_POL_A,value);
	return status;
}


uint8_t Trinamic_TMC5130::set_X_ENC( uint32_t value )
{
	uint32_t data;

	data = value & TMC_X_ENC_MASK;

	writeREG(TMC_REG_X_ENC, data);

	return status;
}


uint8_t Trinamic_TMC5130::get_X_ENC()
{
	uint32_t data;

	readREG(TMC_REG_X_ENC, &data);

	data &= TMC_X_ENC_MASK;

	return data;
}


uint8_t Trinamic_TMC5130::set_ENC_CONST( uint32_t value )
{
	uint32_t data;

	data = value & TMC_ENC_CONST_MASK;

	writeREG(TMC_REG_ENC_CONST, data);

	return status;
}


uint8_t Trinamic_TMC5130::get_ENC_STATUS()
{
	uint32_t data;

	readREG(TMC_REG_ENC_STATUS, &data);

	data &= TMC_ENC_STATUS_MASK;

	return uint8_t(data);
}


uint8_t Trinamic_TMC5130::get_ENC_LATCH()
{
	uint32_t data;

	readREG(TMC_REG_ENC_LATCH, &data);

	data &= TMC_ENC_LATCH_MASK;

	return uint8_t(data);
}

//////////
// MSLUT
//////////

uint8_t Trinamic_TMC5130::set_MSLUT0(uint32_t value)
{
  writeREG(TMC_REG_MSLUT0, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT1(uint32_t value)
{
  writeREG(TMC_REG_MSLUT1, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT2(uint32_t value)
{
  writeREG(TMC_REG_MSLUT2, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT3(uint32_t value)
{
  writeREG(TMC_REG_MSLUT3, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT4(uint32_t value)
{
  writeREG(TMC_REG_MSLUT4, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT5(uint32_t value)
{
  writeREG(TMC_REG_MSLUT5, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT6(uint32_t value)
{
  writeREG(TMC_REG_MSLUT6, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_MSLUT7(uint32_t value)
{
  writeREG(TMC_REG_MSLUT7, value);

  return status;
}

//////////
// MSLUTSEL
//////////

uint8_t Trinamic_TMC5130::set_MSLUTSEL(uint32_t value)
{
  writeREG(TMC_REG_MSLUTSEL, value);

  return status;
}

//////////
// MSLUTSTART
//////////

uint8_t Trinamic_TMC5130::set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90)
{
  uint32_t data;
  data = ( uint32_t(start_sin90) & TMC_MSLUTSTART_START_SIN90_MASK );
  data = data<<TMC_MSLUTSTART_START_SIN90;
  data |= ( uint32_t(start_sin) & TMC_MSLUTSTART_START_SIN_MASK );
  data &= TMC_MSLUTSTART_MASK;

  writeREG(TMC_REG_MSLUTSTART, data);

  return status;
}

//////////
// MSCNT
//////////

uint16_t Trinamic_TMC5130::get_MSCNT()
{
  uint32_t data;

  readREG(TMC_REG_MSCNT, &data);

  data &= TMC_MSCNT_MASK;

  data = uint16_t(data);

  return data;
}

//////////
// MSCURACT
//////////

int32_t Trinamic_TMC5130::get_MSCURACT()
{
  uint32_t data;

  readREG(TMC_REG_MSCURACT, &data);

  data &= TMC_MSCURACT_MASK;

  return data;
}

//////////
// CHOPCONF
//////////

uint8_t Trinamic_TMC5130::set_diss2g(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_DISS2G, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_dedge(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_DEDGE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_intpol(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_INTPOL, value);

  return status;
}

// setting the microstep resolution
uint8_t Trinamic_TMC5130::set_mres(uint16_t value)
{
  uint8_t data = 0;

  switch(value){
    case 1:
      data = 8;
    break;
    case 2:
      data = 7;
    break;
    case 4:
      data = 6;
    break;
    case 8:
      data = 5;
    break;
    case 16:
      data = 4;
    break;
    case 32:
      data = 3;
    break;
    case 64:
      data = 2;
    break;
    case 128:
      data = 1;
    break;
	default:
	  data = 0;
	break;
  }

  set_CHOPCONF(TMC_CHOPCONF_MRES, data);

  return status;
}

uint8_t Trinamic_TMC5130::set_sync(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_SYNC, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_vhighchm(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VHIGHCHM, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_vhighfs(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VHIGHFS, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_vsense(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_VSENSE, value);

  return status;
}
// ([0-3]) set comparator blank time to 16, 24, 36 or 54 clocks, 1 or 2 is recommended
uint8_t Trinamic_TMC5130::set_tbl(uint8_t value)
{
  set_CHOPCONF(TMC_CHOPCONF_TBL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_chm(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_CHM, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_rndtf(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_RNDTF, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_disfdcc(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_DISFDCC, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_fd(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_FD, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_hend(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_HEND, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_hstrt(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_HSTRT, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_toff(uint8_t value)
{

  set_CHOPCONF(TMC_CHOPCONF_TOFF, value);

  return status;
}

  //////////
 // COOLCONF
//////////

// alter coolconf
uint8_t Trinamic_TMC5130::alter_COOLCONF(uint32_t data, uint32_t mask)
{
  _coolconf = ( _coolconf & ~mask ) | ( data & mask );

  writeREG( TMC_REG_COOLCONF, _coolconf );

  return status;
}

// set coolconf
uint8_t Trinamic_TMC5130::set_COOLCONF(uint8_t position, uint8_t value)
{

  alter_COOLCONF( uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position);

  return status;
}

uint8_t Trinamic_TMC5130::set_sfilt(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SFILT, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_sgt(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SGT, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_seimin(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEIMIN, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_sedn(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEDN, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_semax(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEMAX, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_seup(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEUP, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_semin(uint8_t value)
{
  set_CHOPCONF(TMC_COOLCONF_SEMIN, value);

  return status;
}

  //////////
 // DCCTRL
//////////

uint8_t Trinamic_TMC5130::set_DCCTRL(uint16_t dc_time, uint16_t dc_sg)
{
  uint32_t data;
  data = ( uint32_t(dc_sg) & TMC_DCCTRL_DC_SG_MASK );
  data = data<<TMC_DCCTRL_DC_SG;
  data |= ( uint32_t(dc_time) & TMC_DCCTRL_DC_TIME_MASK );
  data &= TMC_DCCTRL_MASK;

  writeREG(TMC_REG_DCCTRL, data);

  return status;
}


uint32_t Trinamic_TMC5130::get_DRV_STATUS()
{
	uint32_t data;

	readREG(TMC_REG_DRV_STATUS, &data);

	return data;
}


uint16_t Trinamic_TMC5130::get_sg_result()
{
	uint32_t data = get_DRV_STATUS();
	data &= TMC_DRV_STATUS_SG_RESULT_MASK;
	return uint16_t(data);
}


uint8_t Trinamic_TMC5130::get_fsactive()
{
	uint32_t data = get_DRV_STATUS();
	
	return bitRead(data,TMC_DRV_STATUS_FSACTIVE);
}


uint8_t Trinamic_TMC5130::get_csActual()
{
	uint32_t data = get_DRV_STATUS();

	data &=TMC_DRV_STATUS_CS_ACTUAL_MASK;
	
	uint8_t res = data >> TMC_DRV_STATUS_CS_ACTUAL;
	return res;
}


uint8_t Trinamic_TMC5130::get_stallGuard()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_STALL_GUARD);
}


uint8_t Trinamic_TMC5130::get_ot()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_OT);
}


uint8_t Trinamic_TMC5130::get_otpw()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_OTPW);
}


uint8_t Trinamic_TMC5130::get_s2ga()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_S2GA);
}


uint8_t Trinamic_TMC5130::get_s2gb()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_S2GB);
}


uint8_t Trinamic_TMC5130::get_ola()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_OLA);
}


uint8_t Trinamic_TMC5130::get_olb()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_OLB);
}


uint8_t Trinamic_TMC5130::get_stst()
{
	uint32_t data = get_DRV_STATUS();

	return bitRead(data,TMC_DRV_STATUS_STST);
}

//////////
 // PWMCONF
//////////

// alter pwmconf
uint8_t Trinamic_TMC5130::alter_PWMCONF(uint32_t data, uint32_t mask)
{
  _pwmconf = ( _pwmconf & ~mask ) | ( data & mask );

  writeREG( TMC_REG_PWMCONF, _pwmconf );

  return status;
}

// set pwmconf
uint8_t Trinamic_TMC5130::set_PWMCONF(uint8_t position, uint8_t value)
{

  alter_PWMCONF( uint32_t(value)<<position, TMC_CHOPCONF_MASKS[position]<<position );

  return status;
}

uint8_t Trinamic_TMC5130::set_freewheel(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_FREEWHEEL, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_pwm_symmetric(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_SYMMETRIC, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_pwm_autoscale(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_AUTOSCALE, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_pwm_freq(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_FREQ, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_PWM_GRAD(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_GRAD, value);

  return status;
}

uint8_t Trinamic_TMC5130::set_PWM_AMPL(uint8_t value)
{
  set_PWMCONF(TMC_PWMCONF_PWM_AMPL, value);

  return status;
}

  //////////
 // ENCM_CTRL
//////////

uint8_t Trinamic_TMC5130::set_ENCM_CTRL(uint8_t value)
{
  uint8_t data;

  data = value & TMC_ENCM_CTRL_MASK;

  writeREG(TMC_REG_ENCM_CTRL, data);

  return status;
}


uint8_t Trinamic_TMC5130::getStatus()
{
	return status;
}

//////////
 // STATUS
  
// check the reset status
boolean Trinamic_TMC5130::isReset()
{
  return status&TMC_SPISTATUS_RESET_MASK ? true : false;
}

// check the error status
boolean Trinamic_TMC5130::isError()
{
  return status&TMC_SPISTATUS_ERROR_MASK ? true : false;
}

// check the stallguard status
boolean Trinamic_TMC5130::isStallguard()
{
  return status&TMC_SPISTATUS_STALLGUARD_MASK ? true : false;
}

// check the standstill status
boolean Trinamic_TMC5130::isStandstill()
{
  return status&TMC_SPISTATUS_STANDSTILL_MASK ? true : false;
}
