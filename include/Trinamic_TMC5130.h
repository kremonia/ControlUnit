#pragma once
enum Mode { COOL_STEP = 0, STEALTH_CHOP = 1 };

#include <SPI.h>
#include "Trinamic_TMC5130_registers.h"

class Trinamic_TMC5130{
public:
	uint32_t _coolconf; // because coolconf only writable
	uint32_t _pwmconf;  // because pwmconf only writable

	uint8_t csPin = 0;
	uint8_t status = 0;
	void initSPI();
	void init(uint8_t param_cs);
	void TmcInitRegisters(byte mode);

  uint8_t readSTAT();
  uint8_t readREG( uint8_t address , uint32_t *data );
  uint8_t readSignedREG(uint8_t address,int32_t *data);
  uint8_t writeREG( uint8_t address, uint32_t data );
  uint8_t alterREG(uint8_t address, uint32_t data, uint32_t mask);

  // gconf registers
  uint8_t setGCONF(uint8_t bit, uint8_t value);
  uint8_t set_I_scale_analog(uint8_t value);
  uint8_t set_internal_rsense(uint8_t value);
  uint8_t set_en_pwm_mode(uint8_t value);
  uint8_t set_enc_commutation(uint8_t value);
  uint8_t set_shaft(uint8_t value);
  uint8_t set_diag0_error(uint8_t value);
  uint8_t set_diag0_otpw(uint8_t value);
  uint8_t set_diag0_stall(uint8_t value);
  uint8_t set_diag1_stall(uint8_t value);
  uint8_t set_diag1_index(uint8_t value);
  uint8_t set_diag1_onstate(uint8_t value);
  uint8_t set_diag1_steRS_skipped(uint8_t value);
  uint8_t set_diag0_int_pushpull(uint8_t value);
  uint8_t set_diag1_int_pushpull(uint8_t value);
  uint8_t set_small_hysteresis(uint8_t value);
  uint8_t set_stop_enable(uint8_t value);
  uint8_t set_direct_mode(uint8_t value);
  //uint8_t test_mode(uint8_t value);

  uint8_t set_X_COMPARE(uint32_t value);

  uint8_t set_IHOLD_IRUN(uint8_t ihold, uint8_t irun, uint8_t iholddelay);
  uint8_t set_TPOWERDOWN(uint8_t value);
  uint32_t get_TSTEP();
  uint8_t set_TPWMTHRS(uint32_t value);
  uint8_t set_TCOOLTHRS(uint32_t value);
  uint8_t set_THIGH(uint32_t value);

  uint8_t set_RAMPMODE(uint8_t value);
  uint8_t set_XACTUAL(uint32_t value);
  uint32_t get_XACTUAL();
  //uint8_t set_VACTUAL(uint32_t value);
  int32_t get_VACTUAL();
  uint8_t set_VSTART(uint32_t value);
  uint8_t set_A1(uint16_t value);
  uint8_t set_V1(uint32_t value);
  uint8_t set_AMAX(uint16_t value);
  uint8_t set_VMAX(uint32_t value);
  uint8_t set_DMAX(uint16_t value);
  uint8_t set_D1(uint16_t value);
  uint8_t set_VSTOP(uint32_t value);
  uint8_t set_TZEROWAIT(uint16_t value);
  uint8_t set_XTARGET(int32_t value);
  uint32_t get_XTARGET();
  //only tmc2130
  uint8_t set_XDIRECT(uint32_t value);
  uint8_t set_XDIRECT(int16_t coil_a, int16_t coil_b);
  int32_t get_XDIRECT();

  uint8_t set_VDCMIN(int32_t value);

  // sw_mode registers
  uint8_t set_SW_MODE(uint8_t position, uint8_t value);
  uint8_t set_en_softstop(uint8_t value);
  uint8_t set_sg_stop(uint8_t value);
  uint8_t set_en_latch_encoder(uint8_t value);
  uint8_t set_latch_r_inactive(uint8_t value);
  uint8_t set_latch_r_active(uint8_t value);
  uint8_t set_latch_l_inactive(uint8_t value);
  uint8_t set_latch_l_active(uint8_t value);
  uint8_t set_swap_lr(uint8_t value);
  uint8_t set_pol_stop_r(uint8_t value);
  uint8_t set_pol_stop_l(uint8_t value);
  uint8_t set_stop_r_enable(uint8_t value);
  uint8_t set_stop_l_enable(uint8_t value);

  // ramp_stat
  uint8_t get_RAMP_STAT(uint8_t position);
  uint8_t get_status_stop_l();
  uint8_t get_status_stop_r();
  uint8_t get_status_latch_l();
  uint8_t get_status_latch_r();
  uint8_t get_event_stop_l();
  uint8_t get_event_stop_r();
//  uint8_t get_event_stop_sg();
  uint8_t get_event_pos_reached();
  uint8_t get_velocity_reached();
  uint8_t get_position_reached();
  uint8_t get_vzero();
  uint8_t get_t_zerowait_active();
  uint8_t get_second_move();
  uint8_t get_status_sg();

  uint32_t get_XLATCH();

  //ENCMODE
  uint8_t set_ENCMODE(uint8_t position,uint8_t value);
  uint8_t set_enc_sel_decimal(uint8_t value);
  uint8_t set_latch_x_act(uint8_t value);
  uint8_t set_clr_enc_x(uint8_t value);
  uint8_t set_neg_edge(uint8_t value);
  uint8_t set_pos_edge(uint8_t value);
  uint8_t set_clr_once(uint8_t value);
  uint8_t set_clr_cont(uint8_t value);
  uint8_t set_ignore_AB(uint8_t value);
  uint8_t set_pol_N(uint8_t value);
  uint8_t set_pol_B(uint8_t value);
  uint8_t set_pol_A(uint8_t value);

  //Encoder registers
  uint8_t set_X_ENC(uint32_t value);
  uint8_t get_X_ENC();
  uint8_t set_ENC_CONST(uint32_t value);
  uint8_t get_ENC_STATUS();
  uint8_t get_ENC_LATCH();

  uint8_t set_MSLUT0(uint32_t value);
  uint8_t set_MSLUT1(uint32_t value);
  uint8_t set_MSLUT2(uint32_t value);
  uint8_t set_MSLUT3(uint32_t value);
  uint8_t set_MSLUT4(uint32_t value);
  uint8_t set_MSLUT5(uint32_t value);
  uint8_t set_MSLUT6(uint32_t value);
  uint8_t set_MSLUT7(uint32_t value);
  uint8_t set_MSLUTSEL(uint32_t value);
  uint8_t set_MSLUTSTART(uint8_t start_sin, uint8_t start_sin90);
  uint16_t get_MSCNT();
  int32_t get_MSCURACT();

  // chopconf registers
  uint8_t set_CHOPCONF(uint8_t position, uint8_t value);
  uint8_t set_dedge(uint8_t value);
  uint8_t set_diss2g(uint8_t value);
  uint8_t set_intpol(uint8_t value);
  uint8_t set_mres(uint16_t value);
  uint8_t set_sync(uint8_t value);
  uint8_t set_vhighchm(uint8_t value);
  uint8_t set_vhighfs(uint8_t value);
  uint8_t set_vsense(uint8_t value);
  uint8_t set_tbl(uint8_t value);
  uint8_t set_chm(uint8_t value);
  uint8_t set_rndtf(uint8_t value);
  uint8_t set_disfdcc(uint8_t value);
  uint8_t set_fd(uint8_t value);
  uint8_t set_hend(uint8_t value);
  uint8_t set_hstrt(uint8_t value);
  uint8_t set_toff(uint8_t value);

  // coolconf register
  uint8_t alter_COOLCONF(uint32_t data, uint32_t mask);
  uint8_t set_COOLCONF(uint8_t position, uint8_t value);
  uint8_t set_sfilt(uint8_t value);
  uint8_t set_sgt(uint8_t value);
  uint8_t set_seimin(uint8_t value);
  uint8_t set_sedn(uint8_t value);
  uint8_t set_semax(uint8_t value);
  uint8_t set_seup(uint8_t value);
  uint8_t set_semin(uint8_t value);

  uint8_t set_DCCTRL(uint16_t dc_time, uint16_t dc_sg);

  //DRV_STATUS
  uint32_t get_DRV_STATUS();
  uint16_t get_sg_result();
  uint8_t get_fsactive();
  uint8_t get_csActual();
  uint8_t get_stallGuard();
  uint8_t get_ot();
  uint8_t get_otpw();
  uint8_t get_s2ga();
  uint8_t get_s2gb();
  uint8_t get_ola();
  uint8_t get_olb();
  uint8_t get_stst();

  //pwmconf registers
  uint8_t alter_PWMCONF(uint32_t data, uint32_t mask);
  uint8_t set_PWMCONF(uint8_t position, uint8_t value);
  uint8_t set_freewheel(uint8_t value);
  uint8_t set_pwm_symmetric(uint8_t value);
  uint8_t set_pwm_autoscale(uint8_t value);
  uint8_t set_pwm_freq(uint8_t value);
  uint8_t set_PWM_GRAD(uint8_t value);
  uint8_t set_PWM_AMPL(uint8_t value);

  uint8_t set_ENCM_CTRL(uint8_t value);

  uint8_t getStatus();
  boolean isReset();
  boolean isError();
  boolean isStallguard();
  boolean isStandstill();
};
