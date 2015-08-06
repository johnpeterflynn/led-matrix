/*
 * TLC5940.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef TLC5940_H_
#define TLC5940_H_

#include "common.h"

#define BLANK_DDR P1DIR
#define BLANK_PORT P1OUT
#define BLANK_PIN BIT0

#define XLAT_DDR P1DIR
#define XLAT_PORT P1OUT
#define XLAT_PIN BIT3

//#define GSCLK_DDR P1DIR
//#define GSCLK_PORT P1OUT
//#define GSCLK_PIN BIT4

#define NUM_TLC5940 3

void TLC5940_Init(void);

void TLC5940_SendDataRow(uint8_t row);

#endif /* TLC5940_H_ */
