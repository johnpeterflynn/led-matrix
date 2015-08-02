/*
 * leddriver.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef LEDDRIVER_H_
#define LEDDRIVER_H_

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


#endif /* LEDDRIVER_H_ */
