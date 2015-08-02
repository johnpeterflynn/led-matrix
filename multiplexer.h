/*
 * multiplexer.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

#include "common.h"

#define SER_PIN BIT1
#define RCLK_PIN BIT1
#define QE_PIN BIT0

#define NUM_REGISTERS 8

extern uint8_t nextRow;

void Multiplexer_Init();

void Multiplexer_setupRows();

// Since the LEDs are active low they are "cleared" by setting all pins to high.
void Multiplexer_clearRows();

// Change to the row indicated by nextRow and return the next row to follow.
uint8_t Multiplexer_changeRow();

#endif /* MULTIPLEXER_H_ */
