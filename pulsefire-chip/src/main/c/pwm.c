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
	uint8_t pwm_state = pf_data.pwm_state;
	uint8_t pulse_fire_mode = pf_conf.pulse_fire_mode;
	if (pulse_fire_mode==PULSE_FIRE_MODE_RESET && pwm_state != PWM_STATE_FIRE_RESET) {
		return; // no fire support when reset is forced to fire first.
	}
	if (pulse_fire_mode!=PULSE_FIRE_MODE_NOSYNC && pwm_state != PWM_STATE_IDLE && pwm_state != PWM_STATE_FIRE_RESET) {
		return; // no fire support when train is running synced.
	}
	pf_data.pwm_state = PWM_STATE_RUN; // Trigger pulse train on external interrupt pin if pulse_trigger
	pf_data.pulse_step = ZERO;         // goto step zero
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

uint16_t PWM_filter_data(uint16_t data) {

	// Apply user defined output enabled flags.
	if (pf_conf.pulse_bank==ZERO) {
		data = data & pf_conf.pulse_mask_a;
	} else {
		data = data & pf_conf.pulse_mask_b;
	}

	// hard limit on output bits, for cleaning extra shifted bits.
	uint16_t out_limit = ZERO;
	for (uint8_t i=ZERO;i < pf_conf.pulse_steps;i++) {
		out_limit |= (ONE << i);
	}
	data = data & out_limit;

	// Inverse per output bank
	if (pf_conf.pulse_bank==ZERO) {
		data = data ^ pf_conf.pulse_inv_a;
	} else {
		data = data ^ pf_conf.pulse_inv_b;
	}

	return data;
}

// Send data to the outputs from extern code
void PWM_send_output(uint16_t data) {
	Chip_out_pwm(PWM_filter_data(data));
}

void PWM_pulse_mode_off(void) {
	pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
	pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_CNT]=0xFFFF;
	pf_data.pwm_data_size++;
}

void PWM_pulse_mode_flash_zero(void) {
	uint16_t pwm_on = ZERO;
	uint16_t pwm_off = ZERO;
	if (pf_conf.pulse_bank==ZERO) {
		pwm_on = pf_conf.pwm_on_cnt_a[ZERO];
		pwm_off = pf_conf.pwm_off_cnt_a[ZERO];
	} else {
		pwm_on = pf_conf.pwm_on_cnt_b[ZERO];
		pwm_off = pf_conf.pwm_off_cnt_b[ZERO];
	}
	pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_ON);
	pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_CNT]=pwm_on;
	pf_data.pwm_data_size++;
	pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
	if (pwm_off > ZERO ) {
		pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_CNT]=pwm_off;
	} else {
		pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_CNT]=pwm_on;
	}
	pf_data.pwm_data_size++;
}

void PWM_pulse_mode_flash(void) {
	uint8_t index = pf_data.pwm_data_size;
	for (uint8_t i=ZERO;i < pf_conf.pulse_steps;i++) {
		uint16_t pwm_on = ZERO;
		uint16_t pwm_off = ZERO;
		if (pf_conf.pulse_bank==ZERO) {
			pwm_on = pf_conf.pwm_on_cnt_a[i];
			pwm_off = pf_conf.pwm_off_cnt_a[i];
		} else {
			pwm_on = pf_conf.pwm_on_cnt_b[i];
			pwm_off = pf_conf.pwm_off_cnt_b[i];
		}
		pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_ON);
		pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_on;
		index++;
		if (pwm_off > ZERO) {
			pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
			pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_off;
			index++;
		}
	}
	pf_data.pwm_data_size = index;
}

void PWM_pulse_mode_train(void) {
	uint8_t pulse_dir = pf_conf.pulse_dir;
	uint16_t pwm_data = ZERO;
	uint8_t index = pf_data.pwm_data_size;
	for (uint8_t i=ZERO;i < pf_conf.pulse_steps;i++) {
		uint16_t pwm_on = ZERO;
		uint16_t pwm_off = ZERO;
		if (pf_conf.pulse_bank==ZERO) {
			pwm_data = pf_conf.pulse_init_a << i;
			pwm_on = pf_conf.pwm_on_cnt_a[i];
			pwm_off = pf_conf.pwm_off_cnt_a[i];
		} else {
			pwm_data = pf_conf.pulse_init_b << i;
			pwm_on = pf_conf.pwm_on_cnt_b[i];
			pwm_off = pf_conf.pwm_off_cnt_b[i];
		}
		if (pulse_dir == PULSE_DIR_RL) {
			pwm_data = reverse_bits(pwm_data,pf_conf.pulse_steps);
		}
		pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(pwm_data);
		pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_on;
		index++;
		if (pwm_off > ZERO) {
			pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
			pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_off;
			index++;
		}
	}
	pf_data.pwm_data_size = index;
}

void PWM_pulse_mode_ppm(void) {
	uint8_t pulse_dir = pf_conf.pulse_dir;
	uint8_t index = pf_data.pwm_data_size;
	uint8_t start = pf_conf.ppm_data_offset;
	if (start >= pf_conf.ppm_data_length) {
		start = ZERO;
	}
	for (uint8_t i=start;i < pf_conf.ppm_data_length;i++) {
		uint16_t pwm_data = ZERO;
		uint16_t pwm_on = ZERO;
		uint16_t pwm_off = ZERO;
		if (pf_conf.pulse_bank==ZERO) {
			pwm_on = pf_conf.pwm_on_cnt_a[i];
			pwm_off = pf_conf.pwm_off_cnt_a[i];
		} else {
			pwm_on = pf_conf.pwm_on_cnt_b[i];
			pwm_off = pf_conf.pwm_off_cnt_b[i];
		}
		for (uint8_t p=ZERO;p < OUTPUT_MAX;p++) {
			uint16_t ppm_data = ZERO; // Shift all channel data out every step.
			if (pf_conf.pulse_bank==ZERO) {
				ppm_data = pf_conf.ppm_data_a[p];
			} else {
				ppm_data = pf_conf.ppm_data_b[p];
			}
			uint8_t i_idx = i;
			if (pulse_dir == PULSE_DIR_RL) {
				i_idx = pf_conf.ppm_data_length - ONE - i; // reverser
			}
			pwm_data |= ((ppm_data >> i_idx) & ONE) << p;
		}
		pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(pwm_data);
		pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_on;
		index++;
		if (pwm_off > ZERO) {
			pf_data.pwm_data[index][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
			pf_data.pwm_data[index][PWM_DATA_CNT]=pwm_off;
			index++;
		}
	}
	pf_data.pwm_data_size = index;
}

void PWM_calc_data_post_pulse(void) {
	if (pf_conf.pulse_post_delay > ZERO) {
		uint8_t index = pf_data.pwm_data_size;
		uint8_t max = pf_conf.pulse_post_mul;
		uint16_t data = PWM_filter_data(PULSE_DATA_OFF);
		if (pf_conf.pulse_post_hold==PULSE_POST_HOLD_ON) {
			data = PWM_filter_data(PULSE_DATA_ON);
		} else if (pf_conf.pulse_post_hold==PULSE_POST_HOLD_LAST1 && index>=1) {
			data = pf_data.pwm_data[index-ONE][PWM_DATA_OUT];
		} else if (pf_conf.pulse_post_hold==PULSE_POST_HOLD_LAST2 && index>=2) {
			data = pf_data.pwm_data[index-ONE-ONE][PWM_DATA_OUT];
		}
		for (uint8_t i=0;i < max;i++) {
			pf_data.pwm_data[index][PWM_DATA_OUT]=data;
			pf_data.pwm_data[index][PWM_DATA_CNT]=pf_conf.pulse_post_delay;
			index++;
		}
		pf_data.pwm_data_size = index;
	}
}

void PWM_calc_data_pre_pulse(void) {
	// Calc pre delay steps
	if (pf_conf.pulse_pre_delay > ZERO) {
		uint8_t max = pf_conf.pulse_pre_mul;
		for (uint8_t i=0;i < max;i++) {
			pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_OUT]=PWM_filter_data(PULSE_DATA_OFF);
			pf_data.pwm_data[pf_data.pwm_data_size][PWM_DATA_CNT]=pf_conf.pulse_pre_delay;
			pf_data.pwm_data_size++;
		}
	}
}

void PWM_calc_data_dir(void) {
	uint8_t pulse_dir = pf_conf.pulse_dir;
	if (pulse_dir <= PULSE_DIR_RL) {
		return; // no extra step in LR and RL.
	}
	uint8_t pulse_mode = pf_conf.pulse_mode;
	if (pulse_mode < PULSE_MODE_TRAIN) {
		return; // only train and ppm can be reversed.
	}
	if (pulse_dir == PULSE_DIR_LRPERL || pulse_dir == PULSE_DIR_LRPELR) {
		PWM_calc_data_pre_pulse(); // append extra pre pulse
	}

	// Auto reverse pulse data
	uint8_t index = pf_data.pwm_data_size;
	uint8_t start_index = pf_data.pwm_data_size-ONE;
	uint8_t stop_index = ZERO;
	if (pulse_dir == PULSE_DIR_LRRL_2) {
		if (pulse_mode == PULSE_MODE_TRAIN && pf_data.pwm_data[start_index][PWM_DATA_OUT]==PWM_filter_data(PULSE_DATA_OFF)) {
			start_index-=3; // small extra check in train mode for reverser-2 with duty steps.
		} else {
			start_index--;
		}
		stop_index = 1; // Skip first and last step when not in full mode.
		if (pf_conf.pulse_pre_delay > ZERO) {
			stop_index = pf_conf.pulse_pre_mul+1; // skip pre delay steps
		}
	} else {
		if (pf_conf.pulse_pre_delay > ZERO) {
			stop_index = pf_conf.pulse_pre_mul; // skip pre delay steps
		}
	}
	if (pulse_dir == PULSE_DIR_LRPELR || pulse_dir == PULSE_DIR_LRPERL) {
		if (pf_conf.pulse_pre_delay > ZERO) {
			start_index -= pf_conf.pulse_pre_mul; // move start the extra added steps.
		}
	}
	if (pulse_dir == PULSE_DIR_LRPOLR || pulse_dir == PULSE_DIR_LRPORL) {
		PWM_calc_data_post_pulse(); // append extra post pulse
		index = pf_data.pwm_data_size;
	}
	if (pulse_dir >= PULSE_DIR_LRLR) {
		for (uint8_t i=stop_index;i <= start_index;i++) {
			uint16_t pwm_data = pf_data.pwm_data[i][PWM_DATA_OUT];
			uint16_t pwm_cnt  = pf_data.pwm_data[i][PWM_DATA_CNT];
			pf_data.pwm_data[index][PWM_DATA_OUT] = pwm_data;
			pf_data.pwm_data[index][PWM_DATA_CNT] = pwm_cnt;
			index++;
		}
	} else {
		for (uint8_t i=start_index+ONE;i >= stop_index+ONE;i--) {
			uint16_t pwm_data = pf_data.pwm_data[i-ONE][PWM_DATA_OUT];
			uint16_t pwm_cnt  = pf_data.pwm_data[i-ONE][PWM_DATA_CNT];
			pf_data.pwm_data[index][PWM_DATA_OUT] = pwm_data;
			pf_data.pwm_data[index][PWM_DATA_CNT] = pwm_cnt;
			index++;
		}
	}
	pf_data.pwm_data_size = index;
}

void PWM_calc_data(void) {
	// Reset data counter to zero
	pf_data.pwm_data_size = ZERO;

	// Calc pre pulse data
	PWM_calc_data_pre_pulse();

	// Calc pulse data based on mode.
	uint8_t pulse_mode = pf_conf.pulse_mode;
	switch (pulse_mode) {
		case PULSE_MODE_FLASH_ZERO:    PWM_pulse_mode_flash_zero();    break;
		case PULSE_MODE_FLASH:         PWM_pulse_mode_flash();         break;
		case PULSE_MODE_TRAIN:         PWM_pulse_mode_train();         break;
		case PULSE_MODE_PPM:           PWM_pulse_mode_ppm();           break;
		case PULSE_MODE_OFF:default:   PWM_pulse_mode_off();           break;
	}

	// Calc reverse or dub data step
	PWM_calc_data_dir();

	// Calc and add post delay step
	PWM_calc_data_post_pulse();
}

// Do all work per timer step cnt
void PWM_work_int(void) {
	uint8_t pwm_state = pf_data.pwm_state;
	if (pwm_state == PWM_STATE_FIRE_HOLD) {
		return; // wait in fire hold
	}
	uint8_t pulse_trig = pf_conf.pulse_trig;
	if (pulse_trig != PULSE_TRIG_LOOP && (pwm_state == PWM_STATE_IDLE || pwm_state == PWM_STATE_FIRE_RESET)) {
		return; // disable when in manual trigger fire.
	}

	// time step with counter
	uint8_t pwm_loop = pf_conf.pwm_loop;
	if (pwm_loop > ZERO) {
		if (pf_data.pwm_loop_cnt < pwm_loop) {
			pf_data.pwm_loop_cnt++;
			return;
		}
		pf_data.pwm_loop_cnt = ZERO;
	}

	// Get step data and wait time
	uint8_t pulse_step = pf_data.pulse_step;
	uint16_t data_out = pf_data.pwm_data[pulse_step][PWM_DATA_OUT];
	uint16_t data_cnt = pf_data.pwm_data[pulse_step][PWM_DATA_CNT];

	// Check for output enable
	if (pf_conf.pulse_enable == ZERO) {
		data_out = PWM_filter_data(PULSE_DATA_OFF);
	}

	// use - for letting last output time correctly until off.
	if (pf_data.pwm_state == PWM_STATE_FIRE_END) {
		pf_data.pwm_state = PWM_STATE_IDLE;
		pf_data.pulse_fire = ZERO;
		Chip_out_pwm(PWM_filter_data(PULSE_DATA_OFF));
		return;
	}
	// check on automatic holding of the step.
	if (pf_conf.pulse_hold_auto > ZERO && pulse_step == pf_conf.pulse_hold_auto - ONE) {
		pf_data.pwm_state = PWM_STATE_FIRE_HOLD;
		if (pf_conf.pulse_hold_autoclr > ZERO) {
			data_out = PWM_filter_data(PULSE_DATA_OFF);
		}
	}

	// Set output and set registers.
	Chip_out_pwm(data_out);
	Chip_reg_set(CHIP_REG_PWM_OCR_A,data_cnt);
	Chip_reg_set(CHIP_REG_PWM_TCNT,ZERO);

	// Loop pulse steps
	if (pulse_step >= pf_data.pwm_data_size-ONE) {
		pulse_step = ZERO;
		if (pulse_trig != PULSE_TRIG_LOOP && pf_data.pwm_state != PWM_STATE_FIRE_HOLD) {
			pf_data.pwm_state = PWM_STATE_FIRE_END; // timeout after trigger
		}
	} else {
		pulse_step++; // Goto next step
	}
	pf_data.pulse_step = pulse_step;
}
#endif


