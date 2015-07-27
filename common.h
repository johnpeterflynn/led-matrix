/*
 * common.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef COMMON_H_
#define COMMON_H_

//#include <msp430.h>
#include "msp430g2553.h"
#include "stdint.h"

#define NUM_ROWS 4

#define SELECT_1 P1SEL
#define SELECT_2 P2SEL

#define enableGPIO(sel, pin) ((sel) &= ~(pin))
#define setOutput(ddr, pin) ((ddr) |= (pin))
#define setLow(port, pin) ((port) &= ~(pin))
#define setHigh(port, pin) ((port) |= (pin))
#define pulse(port, pin) do {setHigh((port), (pin));setLow((port), (pin));} while (0);


#endif /* COMMON_H_ */
