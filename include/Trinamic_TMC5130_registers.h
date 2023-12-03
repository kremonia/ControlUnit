#pragma once

// SPI
#define TMC_SPI_CLOCK_DIVIDER SPI_CLOCK_DIV4
#define TMC_SPI_DATA_MODE SPI_MODE3
#define TMC_SPI_BIT_ORDER MSBFIRST

// RW
#define TMC_READ                         (0x00)
#define TMC_WRITE                        (0x80)

// SPISTATUS MASKS
#define TMC_SPISTATUS_RESET_MASK         (0x01)
#define TMC_SPISTATUS_ERROR_MASK         (0x02)
#define TMC_SPISTATUS_STALLGUARD_MASK    (0x04)
#define TMC_SPISTATUS_STANDSTILL_MASK    (0x08)

// REGISTERS
#define TMC_REG_GCONF                    (0x00) // RW //    17 // Global configuration flags
#define TMC_REG_GSTAT                    (0x01) // RC //     3 // Global status flags
#define TMC_REG_IOIN                     (0x04) // R  //   8+8 // Reads the state of all input pins available
#define TMC_REG_IHOLD_IRUN               (0x10) // W  // 5+5+4 // Driver current control
#define TMC_REG_TPOWERDOWN               (0x11) // W  //     8 // sets delay time after stand still (stst) to motor current power down (0-4 seconds) 0_((2^8)-1) * 2^18 tclk
#define TMC_REG_TSTEP                    (0x12) // R  //    20 // Actual measured time between two 1/256 microsteps derived from the step input frequency in units of 1/fCLK. Measured value is (2^20)-1 in case of overflow or stand still
#define TMC_REG_TPWMTHRS                 (0x13) // W  //    20 // Upper velocity threshold for stealthChop voltage PWM mode
#define TMC_REG_TCOOLTHRS                (0x14) // W  //    20 // Lower threshold velocity for switching on smart energy coolStep and stallGuard feature (unsigned)
#define TMC_REG_THIGH                    (0x15) // W  //    20 // Velocity dependend switching into different chopper mode and fullstepping to maximize torque (unsigned)

#define TMC_REG_X_COMPARE                (0x05) // W //     32 // Position comparison register for motion controller position strobe.The Position pulse is available on output SWP_DIAG1.
//ramp generator registers
#define TMC_REG_RAMPMODE				 (0x20)	//Driving mode (Velocity, Positioning, Hold)
// q & qdot (position, velocity)
#define TMC_REG_XACTUAL					 (0x21)	//Actual motor position
#define TMC_REG_VACTUAL 				 (0x22)	//Actual  motor  velocity  from  ramp  generator
// Ramp curves
#define TMC_REG_VSTART					 (0x23)	//Motor start velocity
#define TMC_REG_A1						 (0x24)	//First  acceleration  between  VSTART  and  V1
#define TMC_REG_V1						 (0x25)	//First  acceleration  /  deceleration  phase  target velocity
#define TMC_REG_AMAX					 (0x26)	//Second  acceleration  between  V1  and  VMAX
#define TMC_REG_VMAX 					 (0x27)	//This is the target velocity in velocity mode. It can be changed any time during a motion.
#define TMC_REG_DMAX					 (0x28)	//Deceleration between VMAX and V1
#define TMC_REG_D1						 (0x2A) 	//Deceleration  between  V1  and  VSTOP
//Attention:  Do  not  set  0  in  positioning  mode, even if V1=0!
#define TMC_REG_VSTOP					 (0x2B)	//Motor stop velocity (unsigned)
//Attention: Set VSTOP > VSTART!
//Attention:  Do  not  set  0  in  positioning  mode, minimum 10 recommend!
#define TMC_REG_TZEROWAIT				 (0x2C)	//Defines  the  waiting  time  after  ramping  down
//to  zero  velocity  before  next  movement  or
//direction  inversion  can  start.  Time  range  is about 0 to 2 seconds.
#define TMC_REG_XTARGET					 (0x2D)	//Target position for ramp mode

#define TMC_REG_XDIRECT                  (0x2D) // RW //    32 // specifies motor coil currents and polarity directly programmed via SPI. Use signed, two's complement numbers. In this mode, the current is scaled by IHOLD. Velocity based current regulation of voltage PWM is not available in this mode. +- 255 for both coils
#define TMC_REG_VDCMIN                   (0x33) // W  //    23 // automatic commutation dcStep becomes enabled by the external signal DCEN. VDCMIN is used as the minimum step velocity when the motor is heavily loaded. Hint: Also set DCCTRL parameters in order to operate dcStep
#define TMC_REG_SW_MODE					 (0x34) // RW //    11 // switch mode configuration 
#define TMC_REG_RAMP_STAT				 (0x35) // R  //    14 // ramp status and switch event status
#define TMC_REG_XLATCH					 (0x36) // R  //    32 // ramp generator latch position
#define TMC_REG_ENCMODE					 (0x38) // RW //    11 // encoder configuration and user of N channel
#define TMC_REG_X_ENC					 (0x39) // RW //    11 // actual encoder position
#define TMC_REG_ENC_CONST				 (0x3A) // W //     32 // accumulation constant
#define TMC_REG_ENC_STATUS				 (0x3B) // R+C//    1  // encoder n event detected
#define TMC_REG_ENC_LATCH				 (0x3C) // R //     32 // encoder position X_ENC latched on N event
#define TMC_REG_MSLUT0                   (0x60) // W  //    32 // Each bit gives the difference between entry x and entry x+1 when combined with the corresponding MSLUTSEL W bits. Differential coding for the first quarter of a wave. Start values for CUR_A and CUR_B are stored for MSCNT position 0 in START_SIN and START_SIN90.
#define TMC_REG_MSLUT1                   (0x61) // W  //    32 //
#define TMC_REG_MSLUT2                   (0x62) // W  //    32 //
#define TMC_REG_MSLUT3                   (0x63) // W  //    32 //
#define TMC_REG_MSLUT4                   (0x64) // W  //    32 //
#define TMC_REG_MSLUT5                   (0x65) // W  //    32 //
#define TMC_REG_MSLUT6                   (0x66) // W  //    32 //
#define TMC_REG_MSLUT7                   (0x67) // W  //    32 //
#define TMC_REG_MSLUTSEL                 (0x68) // W  //    32 // defines four segments within each quarter MSLUT wave. Four 2 bit entries determine the meaning of a 0 and a 1 bit in the corresponding segment of MSLUT
#define TMC_REG_MSLUTSTART               (0x69) // W  //   8+8 //
#define TMC_REG_MSCNT                    (0x6A) // R  //    10 //
#define TMC_REG_MSCURACT                 (0x6B) // R  //   9+9 //
#define TMC_REG_CHOPCONF                 (0x6C) // RW //    32 //
#define TMC_REG_COOLCONF                 (0x6D) // W  //    25 //
#define TMC_REG_DCCTRL                   (0x6E) // W  //    24 //
#define TMC_REG_DRV_STATUS               (0x6F) // R  //    22 //
#define TMC_REG_PWMCONF                  (0x70) // W  //     8 //
#define TMC_REG_PWM_SCALE                (0x71) // R  //     8 //
#define TMC_REG_ENCM_CTRL                (0x72) // W  //     2 //
#define TMC_REG_LOST_STEPS               (0x73) // R  //    20 //

// GCONF OFFSETS
// for shifting incoming values to the right register position
#define TMC_GCONF_I_SCALE_ANALOG               (0) // 0: Internal, 1: AIN
#define TMC_GCONF_INTERNAL_RSENSE              (1) // 0: Normal, 1: Internal
#define TMC_GCONF_EN_PWM_MODE                  (2) // 0: Disable, 1: Enable
#define TMC_GCONF_ENC_COMMUTATION              (3) // 0: Disable, 1: Enable
#define TMC_GCONF_SHAFT                        (4) // 0: Normal, 1: Invert
#define TMC_GCONF_DIAG0_ERROR                  (5) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_OTPW                   (6) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_STALL                  (7) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_STALL                  (8) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_INDEX                  (9) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_ONSTATE                (10) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG1_STERS_SKIPPED          (11) // 0: Disable, 1: Enable
#define TMC_GCONF_DIAG0_INT_PUSHPULL           (12) // 0: Open Collector, 1: Push Pull
#define TMC_GCONF_DIAG1_INT_PUSHPULL           (13) // 0: Open Collector, 1: Push Pull
#define TMC_GCONF_SMALL_HYSTERESIS             (14) // 0: 1/16, 1: 1/32
#define TMC_GCONF_STOP_ENABLE                  (15) // 0: Normal, 1: Emergency Stop
#define TMC_GCONF_DIRECT_MODE                  (16) // 0: Normal, 1: XDIRECT
#define TMC_GCONF_TEST_MODE                    (17) // 0: Normal, 1: Enable, Don't use!

// GCONF MASKS
// not required, all 1 bit

//RAMP_STAT OFFSETS
#define TMC_RAMP_STAT_STATUS_STOP_L                   0 
#define TMC_RAMP_STAT_STATUS_STOP_R		              1 
#define TMC_RAMP_STAT_STATUS_LATCH_L                  2 
#define TMC_RAMP_STAT_STATUS_LATCH_R	              3 
#define TMC_RAMP_STAT_EVENT_STOP_L                    4 
#define TMC_RAMP_STAT_EVENT_STOP_R                    5 
#define TMC_RAMP_STAT_EVENT_STOP_SG                   6 
#define TMC_RAMP_STAT_EVENT_POS_REACHED               7 
#define TMC_RAMP_STAT_VELOCITY_REACHED                8 
#define TMC_RAMP_STAT_POSITION_REACHED                9 
#define TMC_RAMP_STAT_VZERO			                  10 
#define TMC_RAMP_STAT_T_ZEROWAIT_ACTIVE		          11 
#define TMC_RAMP_STAT_SECOND_MOVE		              12 
#define TMC_RAMP_STAT_STATUS_SG			              13 

// DRV_STATUS MASKS
#define TMC_DRV_STATUS_CS_ACTUAL_MASK				  (0b111110000000000000000UL)
#define TMC_DRV_STATUS_SG_RESULT_MASK				  (0b1111111111UL)

//DRV_STATUS OFFSETS
#define TMC_DRV_STATUS_SG_RESULT	                  0 
#define TMC_DRV_STATUS_FSACTIVE			              15 
#define TMC_DRV_STATUS_CS_ACTUAL	                  16 
#define TMC_DRV_STATUS_STALL_GUARD					  24 
#define TMC_DRV_STATUS_OT			                  25 
#define TMC_DRV_STATUS_OTPW							  26
#define TMC_DRV_STATUS_S2GA							  27 
#define TMC_DRV_STATUS_S2GB			                  28
#define TMC_DRV_STATUS_OLA			                  29
#define TMC_DRV_STATUS_OLB			                  30
#define TMC_DRV_STATUS_STST			                  31 


// IHOLD_IRUN OFFSETS
// for shifting incoming values to the right register position
#define TMC_IHOLD                        (0)
#define TMC_IRUN                         (8)
#define TMC_IHOLDDELAY                   (16)

// IHOLD IRUN MASKS
#define TMC_IHOLD_MASK                   (0b11111UL)
#define TMC_IRUN_MASK                    (0b11111UL)
#define TMC_IHOLDDELAY_MASK              (0b1111UL)

// TPOWERDOWN
// no offsets required
#define TMC_TPOWERDOWN_MASK              (0b11111111UL)

// X_COMPARE
// no offsets required
#define TMC_X_COMPARE_MASK               (0xFFFFFFFFUL)

// TSTEP
// no offsets required
#define TMC_TSTEP_MASK                   (0b11111111111111111111UL)

// TPWMTHRS
// no offsets required
#define TMC_TPWMTHRS_MASK                (0b11111111111111111111UL)

// TCOOLTHRS
#define TMC_TCOOLTHRS_MASK               (0b11111111111111111111UL)

// THIGH
// no offsets required
#define TMC_THIGH_MASK                   (0b11111111111111111111UL)

#define TMC_RAMPMODE_MASK				 (0b11UL)
#define TMC_XACTUAL_MASK				 (0xFFFFFFFFUL)
#define TMC_VACTUAL_MASK				 (0xFFFFFFFFUL)
#define TMC_VSTART_MASK					 (0b111111111111111111UL)
#define TMC_A1_MASK						 (0b1111111111111111UL)
#define TMC_V1_MASK						 (0b11111111111111111111UL)
#define TMC_AMAX_MASK					 (0xFFFFUL)
#define TMC_VMAX_MASK					 (0b11111111111111111111111UL)
#define TMC_DMAX_MASK					 (0xFFFFUL)
#define TMC_D1_MASK						 (0xFFFFUL)
#define TMC_VSTOP_MASK					 (0b111111111111111111UL)
#define TMC_TZEROWAIT_MASK				 (0xFFFFUL)
#define TMC_XTARGET_MASK				 (0xFFFFFFFFUL)
#define TMC_XLATCH_MASK					 (0xFFFFFFFFUL)
#define TMC_X_ENC_MASK					 (0xFFFFFFFFUL)
#define TMC_ENC_CONST_MASK				 (0xFFFFFFFFUL)
#define TMC_ENC_STATUS_MASK				 (0b1UL)
#define TMC_ENC_LATCH_MASK	     		 (0xFFFFFFFFUL)
// XDIRECT OFFSETS
// for shifting incoming values to the right register position
#define TMC_XDIRECT_COIL_A               (0)
#define TMC_XDIRECT_COIL_B               (16)

// XDIRECT MASKS
// mask the bits from the values we want to set
#define TMC_XDIRECT_MASK                 (0xFFFFFFFFUL)
#define TMC_XDIRECT_COIL_A_MASK          (0xFFFFUL)
#define TMC_XDIRECT_COIL_B_MASK          (0xFFFFUL)
// no offsets required
// needs no mask

// VDCMIN
// no offsets required
#define TMC_VDCMIN_MASK                  (0b11111111111111111111111UL)

// MSLUT
// no offsets required
// needs no mask

// MSLUTSEL
// no offsets required
// needs no mask

// MSLUTSTART OFFSETS
#define TMC_MSLUTSTART_START_SIN         (0)
#define TMC_MSLUTSTART_START_SIN90       (8)

// MSLUTSTART MASKS
#define TMC_MSLUTSTART_MASK              (0xFFFFUL)
#define TMC_MSLUTSTART_START_SIN_MASK    (0xFF)
#define TMC_MSLUTSTART_START_SIN90_MASK  (0xFF)


// MSCNT
// no offsets required
#define TMC_MSCNT_MASK                   (0b1111111111)

// MSCURACT
// no offsets required
#define TMC_MSCURACT_MASK                (0b111111111111111111UL)

//SW_MODE MASK
//mask the bits from the values we want to set /
// not required, all 1 bit

// // SW_MODE OFFSETS
// // for shifting incoming values to the right register position
#define TMC_SW_MODE_EN_SOFTSTOP				(11)
#define TMC_SW_MODE_SG_STOP					(10)
#define TMC_SW_MODE_EN_LATCH_ENCODER		(9)
#define TMC_SW_MODE_LATCH_R_INACTIVE		(8)
#define TMC_SW_MODE_LATCH_R_ACTIVE			(7)
#define TMC_SW_MODE_LATCH_L_INACTIVE		(6)
#define TMC_SW_MODE_LATCH_L_ACTIVE			(5)
#define TMC_SW_MODE_SWAP_LR					(4)
#define TMC_SW_MODE_POL_STOP_R				(3)
#define TMC_SW_MODE_POL_STOP_L				(2)
#define TMC_SW_MODE_STOP_R_ENABLE			(1)
#define TMC_SW_MODE_STOP_L_ENABLE			(0)

// ENCMODE OFFSETS
// // for shifting incoming values to the right register position
#define TMC_ENCMODE_ENC_SEL_DECIMAL			(10)
#define TMC_ENCMODE_LATCH_X_ACT				(9)
#define TMC_ENCMODE_CLR_ENC_X				(8)
#define TMC_ENCMODE_NEG_EDGE				(7)
#define TMC_ENCMODE_POS_EDGE				(6)
#define TMC_ENCMODE_CLR_ONCE				(5)
#define TMC_ENCMODE_CLR_CONT				(4)
#define TMC_ENCMODE_IGNORE_AB				(3)
#define TMC_ENCMODE_POL_N					(2)
#define TMC_ENCMODE_POL_B					(1)
#define TMC_ENCMODE_POL_A					(0)

// CHOPCONF MASKS
// mask the bits from the values we want to set

const uint32_t TMC_CHOPCONF_MASKS[] = {
  0b1111UL, // 0 TOFF
  0b111UL,  // 1
  0b11UL,   // 2
  0b1UL,    // 3
  0b111UL,  // 4 HYSTERESIS_START
  0b11UL,   // 5
  0b1UL,    // 6
  0b1111UL, // 7 HYSTERESIS_LOW
  0b111UL,  // 8
  0b11UL,   // 9
  0b1UL,    // 10
  0b1UL,    // 11 FAST_DECAY_TIMING
  0b1UL,    // 12 FAST_DECAY_MODE
  0b1UL,    // 13 RANDOM_TOFF
  0b1UL,    // 14 CHOPPER_MODE
  0b11UL,   // 15 TBL
  0b1UL,    // 16
  0b1UL,    // 17 SENSE_CURRENT_SCALING
  0b1UL,    // 18 HIGH_VELOCITY_STEPS
  0b1UL,    // 19 HIGH_VELOCITY_CHOPPER
  0b1111UL, // 20 SYNC_PWM
  0b111UL,  // 21
  0b11UL,   // 22
  0b1UL,    // 23
  0b1111UL, // 24 MRES
  0b111UL,  // 25
  0b11UL,   // 26
  0b1UL,    // 27
  0b1UL,    // 28 INTERPOLATE
  0b1UL,    // 29 DOUBLE_EDGE_PULSES
  0b1UL,    // 30 SHORT_PROTECTION
  0b1UL     // 31
};

// CHOPCONF OFFSETS
// for shifting incoming values to the right register position
/// ivan - checked
#define TMC_CHOPCONF_DISS2G                       (30)
#define TMC_CHOPCONF_DEDGE                        (29)
#define TMC_CHOPCONF_INTPOL                       (28)
#define TMC_CHOPCONF_MRES                         (24)
#define TMC_CHOPCONF_SYNC                         (20)
#define TMC_CHOPCONF_VHIGHCHM                     (19)
#define TMC_CHOPCONF_VHIGHFS                      (18)
#define TMC_CHOPCONF_VSENSE                       (17)
#define TMC_CHOPCONF_TBL                          (15)
#define TMC_CHOPCONF_CHM                          (14)
#define TMC_CHOPCONF_RNDTF                        (13)
#define TMC_CHOPCONF_DISFDCC                      (12)
#define TMC_CHOPCONF_FD                           (11)
#define TMC_CHOPCONF_HEND                         (7)
#define TMC_CHOPCONF_HSTRT                        (4)
#define TMC_CHOPCONF_TOFF                         (0)

// COOLCONF BIT OFFSETS
// for shifting incoming values to the right register position
/// ivan - checked
#define TMC_COOLCONF_SFILT                        (24)
#define TMC_COOLCONF_SGT                          (16)
#define TMC_COOLCONF_SEIMIN                       (15)
#define TMC_COOLCONF_SEDN                         (13)
#define TMC_COOLCONF_SEMAX                        (8)
#define TMC_COOLCONF_SEUP                         (5)
#define TMC_COOLCONF_SEMIN                        (0)

// COOLCONF MASKS
// mask the bits from the values we want to set
/// ivan - checked
const int TMC_COOLCONF_MASKS[] = {
  0b1111UL, // 0 TMC_COOLCONF_SEMIN
  0b111UL, // 1
  0b11UL, // 2
  0b1UL, // 3
  0b0UL, // 4
  0b11UL, // 5 TMC_COOLCONF_SEUP
  0b1UL, // 6
  0b0UL, // 7
  0b1111UL, // 8 TMC_COOLCONF_SEMAX
  0b111UL, // 9
  0b11UL, // 10
  0b1UL, // 11
  0b0UL, // 12
  0b11UL, // 13 TMC_COOLCONF_SEDN
  0b1UL, // 14
  0b1UL, // 15 TMC_COOLCONF_SEIMIN
  0b1111111UL, // 16 TMC_COOLCONF_SGT
  0b111111UL, // 17
  0b11111UL, // 18
  0b1111UL, // 19
  0b111UL, // 20
  0b11UL, // 21
  0b1UL, // 22
  0b0UL, // 23
  0b1UL, // 24 TMC_COOLCONF_SFILT
};

// DCCTRL OFFSETS
// for shifting incoming values to the right register position
#define TMC_DCCTRL_DC_TIME                        (0)
#define TMC_DCCTRL_DC_SG                          (16)

// DCCTRL MASKS
// mask the bits from the values we want to set
#define TMC_DCCTRL_MASK                           (0b111111110000001111111111UL)
#define TMC_DCCTRL_DC_TIME_MASK                   (0b1111111111UL)
#define TMC_DCCTRL_DC_SG_MASK                     (0b11111111UL)


// PWMCONF OFFSETS
// for shifting incoming values to the right register position
/// ivan - checked
#define TMC_PWMCONF_FREEWHEEL                     (20)
#define TMC_PWMCONF_PWM_SYMMETRIC                 (19)
#define TMC_PWMCONF_PWM_AUTOSCALE                 (18)
#define TMC_PWMCONF_PWM_FREQ                      (16)
#define TMC_PWMCONF_PWM_GRAD                      (8)
#define TMC_PWMCONF_PWM_AMPL                      (0)

// PWMCONF MASKS
// mask the bits from the values we want to set
const int TMC_PWMCONF_MASKS[] = {
  0b11111111UL, // 0 TMC_PWMCONF_PWM_AMPL
  0b1111111UL, // 1
  0b111111UL, // 2
  0b11111UL, // 3
  0b1111UL, // 4
  0b111UL, // 5
  0b11UL, // 6
  0b1UL, // 7
  0b11111111UL, // 8 TMC_PWMCONF_PWM_GRAD
  0b1111111UL, // 9
  0b111111UL, // 10
  0b11111UL, // 11
  0b1111UL, // 12
  0b111UL, // 13
  0b11UL, // 14
  0b1UL, // 15
  0b11UL, // 16 TMC_PWMCONF_PWM_FREQ
  0b1UL, // 17
  0b1UL, // 18 TMC_PWMCONF_PWM_AUTOSCALE
  0b1UL, // 19 TMC_PWMCONF_PWM_SYMMETRIC
  0b11UL, // 20 TMC_PWMCONF_FREEWHEEL
  0b1UL, // 21
};

// ENCM_CTRL MASK
// mask the bits from the values we want to set
#define TMC_ENCM_CTRL_MASK          (0b11);

enum SpiStatus {
	RESET_FLAG = 0, DRIVER_ERROR = 1, SG_2 = 2, STANDSTILL = 3, 
	VELOCITY_REACHED = 4, POSITION_REACHED = 5, STATUS_STOP_L = 6,
	STATUS_STOP_R = 7
};

#define POSITION_MODE  0
#define VEL_POS_MODE   1
#define VEL_NEG_MODE   2
#define HOLD_MODE      3
 
