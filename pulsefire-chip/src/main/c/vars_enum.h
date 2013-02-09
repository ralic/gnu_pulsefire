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
	PULSE_TRIG_PULSE_FIRE
};
enum {
	PULSE_MODE_OFF,
	PULSE_MODE_FLASH,
	PULSE_MODE_FLASH_ZERO,
	PULSE_MODE_TRAIN,
	PULSE_MODE_PPM
};
enum {
	PULSE_DIR_LR,
	PULSE_DIR_RL,
	PULSE_DIR_LRRL_2,
	PULSE_DIR_LRRL,
	PULSE_DIR_LRPERL,
	PULSE_DIR_LRPORL,
	PULSE_DIR_LRLR,
	PULSE_DIR_LRPELR,
	PULSE_DIR_LRPOLR
};
enum {
	PULSE_POST_HOLD_OFF,
	PULSE_POST_HOLD_ON,
	PULSE_POST_HOLD_LAST1,
	PULSE_POST_HOLD_LAST2
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
	PWM_STATE_FIRE_RESET,
	PWM_STATE_FIRE_HOLD,
	PWM_STATE_FIRE_END
};
enum {
	PWM_DATA_OUT,
	PWM_DATA_CNT
};
enum {
	INT_TRIG_LOW,
	INT_TRIG_EDGE_ANY,
	INT_TRIG_EDGE_FALL,
	INT_TRIG_EDGE_RISE
};
enum {
	INT_MODE_OFF,
	INT_MODE_PULSE_FIRE,
	INT_MODE_PULSE_HOLD,
	INT_MODE_MAP
};
enum {
	INT_FREQ_MUL_1,
	INT_FREQ_MUL_2,
	INT_FREQ_MUL_3,
	INT_FREQ_MUL_4,
	INT_FREQ_MUL_5,
	INT_FREQ_MUL_6,
	INT_FREQ_MUL_7,
	INT_FREQ_MUL_8,
	INT_FREQ_MUL_9,
	INT_FREQ_MUL_10,
	INT_FREQ_MUL_11,
	INT_FREQ_MUL_12,
	INT_FREQ_MUL_100,
	INT_FREQ_DIV_1,
	INT_FREQ_DIV_2,
	INT_FREQ_DIV_3,
	INT_FREQ_DIV_4,
	INT_FREQ_DIV_5,
	INT_FREQ_DIV_6,
	INT_FREQ_DIV_7,
	INT_FREQ_DIV_8,
	INT_FREQ_DIV_9,
	INT_FREQ_DIV_10,
	INT_FREQ_DIV_11,
	INT_FREQ_DIV_12,
	INT_FREQ_DIV_100
};
enum {
	CHIP_REG_SPI_CLOCK,
	CHIP_REG_PWM_CLOCK,
	CHIP_REG_PWM_OCR_A,
	CHIP_REG_PWM_OCR_B,
	CHIP_REG_PWM_TCNT,
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
	VSC_MODE_OFF,
	VSC_MODE_ONCE_UP,
	VSC_MODE_ONCE_DOWN,
	VSC_MODE_LOOP_UP,
	VSC_MODE_LOOP_DOWN,
	VSC_MODE_LOOP_UPDOWN
};
enum {
	VSC_STATE_UP,
	VSC_STATE_DOWN,
	VSC_STATE_DONE
};
enum {
	LCD_BUTTON_ENTER,
	LCD_BUTTON_ESC,
	LCD_BUTTON_UP,
	LCD_BUTTON_DOWN
};
enum {
	LCD_MODE_OFF,
	LCD_MODE_PAGE,
	LCD_MODE_BUT2,
	LCD_MODE_BUT4
};
enum {
	LCD_HCD_1,
	LCD_HCD_2,
	LCD_HCD_3,
	LCD_HCD_4,
	LCD_HCD_5
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
	PFVB_DATA=1,
	PFVB_CPWM=2,
	PFVB_NOMENU=4,
	PFVB_NOMAP=8,
	PFVB_SLIMIT=16,
	PFVB_TRIG=32,
	PFVB_NORST=64,
	PFVB_PUSH=128
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
enum {
	SPI_CHIPS_NONE=0,
	SPI_CHIPS_OUT8=1,
	SPI_CHIPS_OUT16=2,
	SPI_CHIPS_DOC8=4,
	SPI_CHIPS_DOC16=8,
	SPI_CHIPS_LCD=16,
	SPI_CHIPS_FREE0=32,
	SPI_CHIPS_FREE1=64,
	SPI_CHIPS_FREE2=128
};

// enums for avr
enum {PIN2_OFF,PIN2_DIC2_IN,PIN2_DIC8_IN,PIN2_INT0_IN };
enum {PIN3_OFF,PIN3_DIC3_IN,PIN3_DIC9_IN,PIN3_INT1_IN };
enum {PIN4_OFF,PIN4_DIC4_IN,PIN4_DIC10_IN,PIN4_DOC4_OUT,PIN4_DOC10_OUT };
enum {PIN5_OFF,PIN5_DIC5_IN,PIN5_DIC11_IN,PIN5_DOC5_OUT,PIN5_DOC11_OUT,PIN5_CLOCK_IN };

// enums for avr mega
enum {PORTA_OFF,PORTA_OUT8, PORTA_DOC8 };
enum {PORTC_OFF,PORTC_OUT16,PORTC_DOC8,PORTC_DOC16 };

// end include
#endif
