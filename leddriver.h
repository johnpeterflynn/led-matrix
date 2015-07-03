/*
 * leddriver.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

#include "stdint.h"

#define BLANK_DDR P1DIR
#define BLANK_PORT P1OUT
#define BLANK_PIN BIT0

#define XLAT_DDR P1DIR
#define XLAT_PORT P1OUT
#define XLAT_PIN BIT3

#define GSCLK_DDR P1DIR
#define GSCLK_PORT P1OUT
#define GSCLK_PIN BIT4

#define NUM_TLC5940 3

#if (24 * NUM_TLC5940 > 255)
	#define gsData_t uint16_t
#else
	#define gsData_t uint8_t
#endif

#if (16 * NUM_TLC5940 > 255)
	#define channel_t uint16_t
#else
	#define channel_t uint8_t
#endif

#define GS_DATA_SIZE ((gsData_t)24 * NUM_TLC5940)
#define NUM_CHANNELS ((channel_t)16 * NUM_TLC5940)

extern uint8_t gsData[GS_DATA_SIZE];
extern volatile uint8_t gsUpdateFlag; // TODO: Protect variable from compiler optimizations.

static inline void TLC5940_SetGSUpdateFlag(void) {
//__asm__ volatile ("" ::: "memory");
	gsUpdateFlag = 1;
}

void TLC5940_Init(void);
void TLC5940_SetGS(channel_t channel, uint16_t value);
void TLC5940_SetAllGS(uint16_t value);
void TLC5940_SetColorGS(uint16_t color, uint16_t value);


#endif /* LEDDRIVER_H_ */
