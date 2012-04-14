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

// Include singleton
#ifndef _VARS_ENUM_H
#define _VARS_ENUM_H

// Enums, count as int 0,1,2,etc,etc
enum {
	PULSE_TRIG_LOOP,
	PULSE_TRIG_FIRE,
	PULSE_TRIG_EXT,
	PULSE_TRIG_EXT_FIRE
};
enum {
	PULSE_MODE_OFF,
	PULSE_MODE_FLASH,
	PULSE_MODE_FLASH_ZERO,
	PULSE_MODE_TRAIN,
	PULSE_MODE_PPM,
	PULSE_MODE_PPMA,
	PULSE_MODE_PPMI
};
enum {
	PULSE_DIR_LR,
	PULSE_DIR_RL,
	PULSE_DIR_LRRL
};
enum {
	PULSE_FIRE_MODE_NORMAL,
	PULSE_FIRE_MODE_NOSYNC,
	PULSE_FIRE_MODE_RESET
};
enum {
	PULSE_HOLD_MODE_STOP,
	PULSE_HOLD_MODE_CLEAR,
	PULSE_HOLD_MODE_ZERO,
	PULSE_HOLD_MODE_ZERO_CLR
};

enum {
	PWM_STATE_IDLE,
	PWM_STATE_RUN,
	PWM_STATE_STEP_DUTY,
	PWM_STATE_STEP_DUTY_DONE,
	PWM_STATE_WAIT_TRIG,
	PWM_STATE_WAIT_POST,
	PWM_STATE_FIRE_RESET,
	PWM_STATE_FIRE_HOLD,
	PWM_STATE_FIRE_END
};
enum {
	CHIP_REG_PWM_CLOCK,
	CHIP_REG_PWM_OCR_A,
	CHIP_REG_PWM_OCR_B,
	CHIP_REG_PWM_TCNT,
	CHIP_REG_CIT0_CLOCK,
	CHIP_REG_CIT0_MODE,
	CHIP_REG_CIT0_INT,
	CHIP_REG_CIT0_OCR_A,
	CHIP_REG_CIT0_COM_A,
	CHIP_REG_CIT0_OCR_B,
	CHIP_REG_CIT0_COM_B,
	CHIP_REG_CIP0_CLOCK,
	CHIP_REG_CIP0_MODE,
	CHIP_REG_CIP0_OCR_A,
	CHIP_REG_CIP0_COM_A,
	CHIP_REG_CIP0_OCR_B,
	CHIP_REG_CIP0_COM_B,
	CHIP_REG_CIP0_OCR_C,
	CHIP_REG_CIP0_COM_C,
	CHIP_REG_CIP1_CLOCK,
	CHIP_REG_CIP1_MODE,
	CHIP_REG_CIP1_OCR_A,
	CHIP_REG_CIP1_COM_A,
	CHIP_REG_CIP1_OCR_B,
	CHIP_REG_CIP1_COM_B,
	CHIP_REG_CIP1_OCR_C,
	CHIP_REG_CIP1_COM_C,
	CHIP_REG_CIP2_CLOCK,
	CHIP_REG_CIP2_MODE,
	CHIP_REG_CIP2_OCR_A,
	CHIP_REG_CIP2_COM_A,
	CHIP_REG_CIP2_OCR_B,
	CHIP_REG_CIP2_COM_B,
	CHIP_REG_CIP2_OCR_C,
	CHIP_REG_CIP2_COM_C
};

enum {
	ADC_STATE_IDLE,
	ADC_STATE_RUN,
	ADC_STATE_DONE
};

enum {
	LPM_INIT,
	LPM_IDLE,
	LPM_START,
	LPM_START_WAIT,
	LPM_STOP,
	LPM_RUN,
	LPM_DONE,
	LPM_DONE_WAIT,
	LPM_RECOVER,
	LPM_RECOVER_WAIT
};
enum {
	LCD_SIZE_2x16,
	LCD_SIZE_2x20,
	LCD_SIZE_4x20
};
enum {
	LCD_MENU_STATE_OFF,
	LCD_MENU_STATE_SELECT,
	LCD_MENU_STATE_VALUE,
	LCD_MENU_STATE_VALUE_INDEXED
};
enum {
	LCD_SEND_DATA,
	LCD_SEND_CMD,
	LCD_SEND_INIT
};
enum {
	DEV_DOT_0,
	DEV_DOT_10,
	DEV_DOT_100,
	DEV_DOT_1000,
	DEV_DOT_10000
};
enum {
	PFVF_TYPE,
	PFVF_VAR,
	PFVF_NAME,
	PFVF_MAX,
	PFVF_BITS,
	PFVF_DEF
};
enum {
	PFVT_NONE,
	PFVT_8BIT,
	PFVT_16BIT,
	PFVT_32BIT
};
enum {
	PFVB_NONE=0,
	PFVB_IDXA=1,
	PFVB_IDXB=2,
	PFVB_NOMENU=4,
	PFVB_NOMAP=8,
	PFVB_NOLIMIT=16,
	PFVB_TRIG=32,
	PFVB_DT0=64,
	PFVB_DT1=128
};
enum {
	QMAP_VAR,
	QMAP_VALUE_A,
	QMAP_VALUE_B,
	QMAP_VAR_IDX
};
enum {
	STV_STATE_OKE,
	STV_STATE_WARNING_MAX,
	STV_STATE_WARNING_MIN,
	STV_STATE_ERROR_MAX,
	STV_STATE_ERROR_MIN
};
enum {
	MAL_STATE_IDLE,
	MAL_STATE_RUN,
	MAL_STATE_ENDIF
};

// enums for avr
enum {PIN2_OFF,PIN2_TRIG_IN, PIN2_DIC2_IN,PIN2_DIC8_IN, PIN2_DOC2_OUT,PIN2_DOC8_OUT, PIN2_FREQ_IN,PIN2_FIRE_IN,PIN2_HOLD_FIRE_IN };
enum {PIN3_OFF,PIN3_MENU0_IN,PIN3_DIC3_IN,PIN3_DIC9_IN, PIN3_DOC3_OUT,PIN3_DOC9_OUT, PIN3_FREQ_IN,PIN3_FIRE_IN,PIN3_HOLD_FIRE_IN,PIN3_CIT0B_OUT };
enum {PIN4_OFF,PIN4_MENU1_IN,PIN4_DIC4_IN,PIN4_DIC10_IN,PIN4_DOC4_OUT,PIN4_DOC10_OUT };
enum {PIN5_OFF,PIN5_CLOCK_IN,PIN5_DIC5_IN,PIN5_DIC11_IN,PIN5_DOC5_OUT,PIN5_DOC11_OUT };

// enums for avr mega
enum {PIN18_OFF,PIN18_TRIG_IN, PIN18_DIC4_IN,PIN18_DIC6_IN,PIN18_DOC4_OUT,PIN18_DOC6_OUT, PIN18_FREQ_IN,PIN18_FIRE_IN,PIN18_HOLD_FIRE_IN };
enum {PIN19_OFF,PIN19_TRIG_IN, PIN19_DIC5_IN,PIN19_DIC7_IN,PIN19_DOC5_OUT,PIN19_DOC7_OUT, PIN19_FREQ_IN,PIN19_FIRE_IN,PIN19_HOLD_FIRE_IN };
enum {PIN47_OFF,PIN47_CLOCK_IN };
enum {PIN48_OFF,PIN48_MENU0_IN,PIN48_DIC4_IN,PIN48_DIC6_IN,PIN48_DOC4_OUT,PIN48_DOC6_OUT };
enum {PIN49_OFF,PIN49_MENU1_IN,PIN49_DIC5_IN,PIN49_DIC7_IN,PIN49_DOC5_OUT,PIN49_DOC7_OUT };


// end include
#endif
