/*
 * Copyright (c) 2011, Willem Cazander
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided
 * that the following conditions are met:
 * 
 * * Redistributions of source code must retain the above copyright notice, this list of conditions and the
 *   following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
 *   the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */



#include "pwm.h"

#ifdef SF_ENABLE_PWM

// note: is also called from int.
void PWM_pulsefire(void) {
	if (pf_conf.pulse_trig == PULSE_TRIG_LOOP) {
		return; // no fire support in loop mode
	}
	if (pf_conf.pulse_fire_mode==PULSE_FIRE_MODE_RESET && pf_data.pwm_state != PWM_STATE_FIRE_RESET) {
		return; // no fire support when reset is forced to fire first.
	}
	if (pf_conf.pulse_fire_mode!=PULSE_FIRE_MODE_NOSYNC && pf_data.pwm_state != PWM_STATE_IDLE && pf_data.pwm_state != PWM_STATE_FIRE_RESET) {
		return; // no fire support when train is running synced.
	}
	pf_data.pwm_state            = PWM_STATE_RUN; // Trigger pulse train on external interrupt pin if pulse_trigger
	pf_data.pulse_step           = ZERO;                     // goto step zero
	pf_data.pulse_trig_delay_cnt = pf_conf.pulse_trig_delay; // reload trig timer
	pf_data.pulse_bank_cnt       = pf_conf.pulse_bank;       // load pulse bank after pulse
	pf_data.pulse_fire_cnt++;
	pf_data.pulse_fire_freq_cnt++;
	Chip_reg_set(CHIP_REG_PWM_OCR_A,ONE);
	Chip_reg_set(CHIP_REG_PWM_TCNT,ZERO);
	for (uint8_t i=0;i<FIRE_MAP_MAX;i++) {
		uint16_t v = pf_conf.pulse_fire_map[i][QMAP_VAR];
		if (v==QMAP_VAR_NONE) { continue; }
		Vars_setValueInt(v,pf_conf.pulse_fire_map[i][QMAP_VAR_IDX],ZERO,pf_conf.pulse_fire_map[i][QMAP_VALUE_A]);
	}
}

// Send data to the outputs
void PWM_send_output(uint16_t data) {

	// Reverse output data if needed.
	uint8_t pulse_dir = pf_conf.pulse_dir;
	if (pulse_dir == PULSE_DIR_LRRL) {
		pulse_dir = pf_data.pulse_dir_cnt;
	}
	if (pulse_dir == PULSE_DIR_RL) {
		data = reverse_bits(data,pf_conf.pulse_steps);
	}

	// Apply user defined output enabled flags.
	if (pf_data.pulse_bank_cnt==ZERO) {
		data = data & pf_conf.pulse_mask_a;
	} else {
		data = data & pf_conf.pulse_mask_b;
	}

	// hard limit on output bits, for cleaning extra shifted bits.
	uint16_t out_limit = ZERO;
	uint8_t i=ZERO;
	for (i=ZERO;i < pf_conf.pulse_steps;i++) {
		out_limit |= (ONE << i);
	}
	data = data & out_limit;

	// All output off override, but with respect to inverse request.
	if (pf_conf.pulse_enable == ZERO) {
		data = PULSE_DATA_OFF;
	}

//#ifdef SF_ENABLE_STV
//	if (pf_conf.stv_error_mode == PULSE_MODE_OFF) {
//		// This is so on error and mode off, output stays off while mode changes and innner if save few cycles.
//		if (pf_prog.stv_state == STV_STATE_ERROR_MAX || pf_prog.stv_state == STV_STATE_ERROR_MIN) {
//			data = PULSE_DATA_OFF;
//		}
//	}
//#endif

	// Inverse per output bank
	if (pf_data.pulse_bank_cnt==ZERO) {
		data = data ^ pf_conf.pulse_inv_a;
	} else {
		data = data ^ pf_conf.pulse_inv_b;
	}

	// Inverse output if requested
	if (pf_conf.pulse_inv > ZERO) {
		data = ~data;
	}

	Chip_out_pwm(data);
}

#ifdef SF_ENABLE_SWC
boolean PWM_soft_warmup(void) {
	uint32_t sys_up_secs = millis()/1000;
	if (sys_up_secs == ZERO) {
		sys_up_secs = ONE; // very fast cpu here.
	}
	if (pf_conf.swc_delay > ZERO && pf_conf.swc_delay > sys_up_secs) {
		return true;
	}
	pf_data.swc_secs_cnt = sys_up_secs - pf_conf.swc_delay; // correct for pre delay
	if (pf_data.swc_secs_cnt == ZERO) {
		pf_data.swc_secs_cnt = ONE;
	}
	if (pf_data.swc_secs_cnt > pf_conf.swc_secs) {
		pf_data.swc_secs_cnt  = ZERO;
		// TODO: move after new time loop code in main
		for (uint8_t i=ZERO;i < SWC_MAP_MAX;i++) {
			uint16_t v = pf_conf.swc_map[i][QMAP_VAR];
			if (v==QMAP_VAR_NONE) {
				continue;
			}
			uint16_t value = pf_conf.swc_map[i][QMAP_VALUE_B];
			if (value==0xFFFF) {
				continue;
			}
			uint16_t vIdx = pf_conf.swc_map[i][QMAP_VAR_IDX];
			Vars_setValueInt(v,vIdx,ZERO,value);
		}
		return false; // we are done with startup.
	}
	uint16_t startup_duty = pf_conf.swc_duty * (pf_conf.swc_secs - pf_data.swc_secs_cnt)/2;
	if (pf_data.pulse_step == ZERO) {
		pf_data.swc_duty_cnt = ZERO; // this is not yet corrent for all modes
	}
	if (pf_data.pulse_step == ONE) {
		pf_data.swc_duty_cnt++;
		if (pf_data.swc_duty_cnt < startup_duty) {
			PWM_send_output(PULSE_DATA_OFF);
			return true; // wait
		}
	}
	return false; // run steps
}
#endif

boolean PWM_pulse_mode_flash(void) {
	if (pf_data.pulse_data != PULSE_DATA_OFF) {
		pf_data.pulse_data = PULSE_DATA_OFF;
	} else {
		pf_data.pulse_data = PULSE_DATA_ON;
	}
	return true;
}

boolean PWM_pulse_mode_train(void) {
	if (pf_conf.pulse_steps == ONE) {
		return PWM_pulse_mode_flash(); // small exception to make it work like expected
	}
	if (pf_data.pulse_bank_cnt==ZERO) {
		pf_data.pulse_data = pf_conf.pulse_init_a << pf_data.pulse_step;
	} else {
		pf_data.pulse_data = pf_conf.pulse_init_b << pf_data.pulse_step;
	}
	return true;
}

#ifdef SF_ENABLE_PPM
boolean PWM_pulse_mode_ppm(uint8_t step_zero,boolean interleaved) {
	// Do ppm seqence shifting
	uint16_t out_data = ZERO;
	if (pf_data.pulse_bank_cnt==ZERO) {
		uint16_t ppm_data = (pf_conf.ppm_data_a[step_zero] >> pf_data.ppm_idx[step_zero]) & ONE;
		out_data |= ppm_data << pf_data.pulse_step;
	} else {
		uint16_t ppm_data = (pf_conf.ppm_data_b[step_zero] >> pf_data.ppm_idx[step_zero]) & ONE;
		out_data |= ppm_data << pf_data.pulse_step;
	}

	// do future step with offset for phasing effect
	if (pf_conf.ppm_data_offset > ZERO && pf_conf.ppm_data_offset >= pf_data.ppm_idx[step_zero]) {
		int off_out = step_zero + ONE;
		if (off_out < pf_conf.pulse_steps) {
			if (pf_data.pulse_bank_cnt==ZERO) {
				out_data |= (((pf_conf.ppm_data_a[off_out] >> pf_data.ppm_idx[off_out]) & ONE) << off_out);
			} else {
				out_data |= (((pf_conf.ppm_data_b[off_out] >> pf_data.ppm_idx[off_out]) & ONE) << off_out);
			}
			if (pf_data.ppm_idx[off_out] >= pf_conf.ppm_data_length - ONE) {
				pf_data.ppm_idx[off_out] = ZERO;
			} else {
				pf_data.ppm_idx[off_out]++;
			}
		}
	}
	pf_data.pulse_data = out_data;
	if (pf_data.ppm_idx[step_zero] >= pf_conf.ppm_data_length - ONE) {
		pf_data.ppm_idx[step_zero] = ZERO;
		return true;
	} else {
		pf_data.ppm_idx[step_zero]++;
		if (interleaved) {
			return true;
		} else {
			return false;
		}
	}
}
#endif

#ifdef SF_ENABLE_PPM
boolean PWM_pulse_mode_ppma(void) {
	// Shift all channel data out every step.
	uint16_t out_data = ZERO;
	uint8_t i=ZERO;
	for (i=ZERO;i < pf_conf.pulse_steps;i++) {
		uint16_t ppm_data = ZERO;
		if (pf_data.pulse_bank_cnt==ZERO) {
			ppm_data = pf_conf.ppm_data_a[i];
		} else {
			ppm_data = pf_conf.ppm_data_b[i];
		}
		out_data |= ((ppm_data >> pf_data.ppm_idx[ZERO]) & ONE) << i;
	}
	pf_data.pulse_data = out_data;
	if (pf_data.ppm_idx[ZERO] >= pf_conf.ppm_data_length - ONE) {
		pf_data.ppm_idx[ZERO] = ZERO;
		pf_data.pulse_step = pf_conf.pulse_steps; // done so jmp to last step
		return true;
	} else {
		pf_data.ppm_idx[ZERO]++;
		return false;
	}
}
#endif

// Do all work per timer step cnt
// Timer interrupt for step on time
void PWM_do_work_a(void) {
	if (pf_data.pwm_state == PWM_STATE_STEP_DUTY || pf_data.pwm_state == PWM_STATE_FIRE_HOLD) {
		return; // waiting for step duty
	}
	if (pf_conf.pulse_trig != PULSE_TRIG_LOOP && (pf_data.pwm_state == PWM_STATE_IDLE || pf_data.pwm_state == PWM_STATE_FIRE_RESET)) {
		return; // disable when in manuale trigger fire.
	}
	uint8_t step_zero = pf_data.pulse_step;
/*
	if (pf_conf.pulse_mode == PULSE_MODE_TRAIN) {
		step_zero--; // corrected value for compa/compb and wait... mmmm ) | (pf_conf.pulse_mode == PULSE_MODE_PPMI)
		if (pf_data.pulse_step == ZERO) {
			step_zero = pf_conf.pulse_steps - ONE;
		}
	}
*/

	uint8_t step_ocr = step_zero;
	if (pf_conf.pulse_mode == PULSE_MODE_FLASH_ZERO) {
		step_ocr = ZERO; // always use timeings/data of channel 0 for every step.
	}
	if (pf_data.pulse_bank_cnt==ZERO) {
		Chip_reg_set(CHIP_REG_PWM_OCR_A,pf_conf.pwm_on_cnt_a[step_ocr]);
	} else {
		Chip_reg_set(CHIP_REG_PWM_OCR_A,pf_conf.pwm_on_cnt_b[step_ocr]);
	}
	Chip_reg_set(CHIP_REG_PWM_TCNT,ZERO);

	// time step with counter
	pf_data.pwm_loop_cnt++;
	if (pf_data.pwm_loop_cnt < pf_data.pwm_loop_max) {
		return;
	}
	if (pf_data.pulse_step == ZERO) {
		pf_data.pwm_loop_max = pf_conf.pwm_loop;// reload loop_max before first step but after wait loop of running step
	}
	pf_data.pwm_loop_cnt = ZERO;

	// wait loop to fine tune this step
	for (uint16_t i=ZERO;i < pf_conf.pwm_tune_cnt[step_zero];i++) {
		asm volatile ("nop"); // todo: scope out speed of this loop and tune.
	}

	// Check for soft startup
#ifdef SF_ENABLE_SWC
	if (pf_data.swc_secs_cnt > ZERO) {
		if (PWM_soft_warmup()) {
			return; // wait in startup mode
		}
	}
#endif

	// wait for pulse trig delay
	if (pf_conf.pulse_trig != PULSE_TRIG_LOOP && pf_data.pulse_trig_delay_cnt > ZERO) {
		pf_data.pulse_trig_delay_cnt--;
		return;
	}

	// wait for pulse post delay
	if (pf_data.pwm_state == PWM_STATE_WAIT_POST) {
		PWM_send_output(PULSE_DATA_OFF);
		pf_data.pulse_post_delay_cnt++;
		uint32_t pre_train_wait = ((F_CPU/pf_conf.pwm_on_cnt_a[0]/100) * pf_conf.pulse_post_delay) / pf_conf.pwm_loop;
		if (pf_data.pulse_post_delay_cnt < pre_train_wait) {
			return;
		}
		pf_data.pulse_post_delay_cnt = ZERO;
		pf_data.pwm_state = PWM_STATE_RUN;
	}

	// use - for letting last output time correctly until off.
	if (pf_data.pwm_state == PWM_STATE_FIRE_END) {
		pf_data.pwm_state = PWM_STATE_IDLE;
		pf_data.pulse_fire = ZERO;
		PWM_send_output(PULSE_DATA_OFF);
		return;
	}

	// do step duty timing
	uint16_t off_time = ZERO;
	if (step_ocr==ZERO) {
		step_ocr = pf_conf.pulse_steps - ONE;
	} else {
		step_ocr--; // corrected value for compb
	}
	if (pf_data.pulse_bank_cnt==ZERO) {
		off_time = pf_conf.pwm_off_cnt_a[step_ocr];
	} else {
		off_time = pf_conf.pwm_off_cnt_b[step_ocr];
	}
	if (off_time > ZERO) {
		if (pf_data.pwm_state != PWM_STATE_STEP_DUTY_DONE) {
			pf_data.pwm_state = PWM_STATE_STEP_DUTY;
			PWM_send_output(PULSE_DATA_OFF);
			Chip_reg_set(CHIP_REG_PWM_OCR_B,off_time);
			Chip_reg_set(CHIP_REG_PWM_TCNT,0xFFFF-8); // reset again, with some time so interrupts are enabled again, else we miss it sometimes.
			return;
		}
		pf_data.pwm_state = PWM_STATE_RUN;
	}

	// Calc pulse_data for postition based on pulse_mode
	boolean step_update = false;
	switch (pf_conf.pulse_mode) {
		case PULSE_MODE_FLASH_ZERO:
		case PULSE_MODE_FLASH:         step_update = PWM_pulse_mode_flash();              break;
		case PULSE_MODE_TRAIN:         step_update = PWM_pulse_mode_train();              break;
#ifdef SF_ENABLE_PPM
		case PULSE_MODE_PPM:           step_update = PWM_pulse_mode_ppm(step_zero,false); break;
		case PULSE_MODE_PPMA:          step_update = PWM_pulse_mode_ppma();               break;
		case PULSE_MODE_PPMI:          step_update = PWM_pulse_mode_ppm(step_zero,true);  break;
#endif
		case PULSE_MODE_OFF:default:   pf_data.pulse_data = PULSE_DATA_OFF;               break;
	}

	// Send data to output with output filtering code.
	PWM_send_output(pf_data.pulse_data);

	// only update pulse_step when needed
	if (step_update==false) {
		return;
	}

	// speed up after each step
	if (pf_conf.pwm_loop_delta > ZERO && pf_data.pwm_loop_max > pf_conf.pwm_loop_delta) {
		pf_data.pwm_loop_max = pf_data.pwm_loop_max-pf_conf.pwm_loop_delta;
	}

	// check on first/zero step todo automatic holding of the step.
	if (pf_conf.pulse_hold_auto > ZERO && pf_data.pulse_step == pf_conf.pulse_hold_auto - ONE) {
		pf_data.pwm_state = PWM_STATE_FIRE_HOLD;
		if (pf_conf.pulse_hold_autoclr > ZERO) {
			PWM_send_output(PULSE_DATA_OFF);
		}
		// goto next step for resume
	}

	// check for output rotation after last step		
	if (pf_data.pulse_step  >= pf_conf.pulse_steps - ONE) {
		pf_data.pulse_step     = ZERO;                    // goto step zero
		pf_data.pulse_trig_delay_cnt = pf_conf.pulse_trig_delay; // reload trig timer
		pf_data.pulse_bank_cnt = pf_conf.pulse_bank;      // load pulse bank after pulse
		if (pf_conf.pulse_dir    == PULSE_DIR_LRRL) {
			pf_data.pwm_loop_cnt = pf_data.pwm_loop_max;    // skip wait after reversal
			if (pf_data.pulse_dir_cnt  == PULSE_DIR_LR) {
				pf_data.pulse_dir_cnt     = PULSE_DIR_RL;
			} else { 
				pf_data.pulse_dir_cnt     = PULSE_DIR_LR;   // Auto direction reversal
			}
		}
		if (pf_conf.pulse_trig != PULSE_TRIG_LOOP) {
			pf_data.pwm_state = PWM_STATE_FIRE_END;         // timeout after trigger
		} else if (pf_conf.pulse_post_delay > ZERO) {
			pf_data.pwm_state = PWM_STATE_WAIT_POST;        // timeout after pulse train
		}
	} else {
		pf_data.pulse_step++;                             // Goto next step in pulse fire train
	}
}

// Timer interrupt for step off time
void PWM_do_work_b(void) {
	if (pf_data.pwm_state != PWM_STATE_STEP_DUTY) {
		return;
	}
	// time step with counter
	pf_data.pwm_loop_cnt++;
	if (pf_data.pwm_loop_cnt < pf_data.pwm_loop_max) {
		return;
	}

	pf_data.pwm_state = PWM_STATE_STEP_DUTY_DONE;
	pf_data.pwm_loop_cnt = pf_data.pwm_loop_max; // skip normal wait, so we can go to next step
	PWM_do_work_a();
}

#endif


