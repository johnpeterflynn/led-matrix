/*
 * multiplexer.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */
#include "multiplexer.h"

uint8_t nextRow;

void Multiplexer_Init()
{
	nextRow = 0;

	// Initialize pins.
	setHigh(P2OUT, QE_PIN);
	enableGPIO(SELECT_1, SER_PIN + RCLK_PIN);
	enableGPIO(SELECT_2, QE_PIN);
	setOutput(P1DIR, SER_PIN);
	setOutput(P1DIR, RCLK_PIN);
	setOutput(P2DIR, QE_PIN);

	// Clear all rows and prepare the active "travelling" bit.
	// Since there are two flip flops between the input and first
	// output, three between the input and the second, four between
	// the input and third, etc.. we need to buffer the active bit
	// in the first flip flop.
	Multiplexer_clearRows();
	Multiplexer_setupRows();
}

void Multiplexer_setupRows()
{
	// Buffer the active travelling bit.
	setLow(P1OUT, SER_PIN);
	pulse(P1OUT, RCLK_PIN);

	// Switch from high impedance to output mode on register pins.
	setLow(P2OUT, QE_PIN);
}

void Multiplexer_clearRows()
{
	int i = 0;

	// Switch to high impedance mode.
	setHigh(P2OUT, QE_PIN);

	// Turn off all rows.
	for(i = 0; i < NUM_REGISTERS + 1; i++) {
		setHigh(P1OUT, SER_PIN);
		pulse(P1OUT, RCLK_PIN);
	}
}

uint8_t Multiplexer_changeRow()
{
	// The active travelling bit is delayed by one cycle so
	// it needs to be clocked in at NUM_ROWS - 1 instead of 0.
	if(nextRow == NUM_ROWS - 1) {
		setLow(P1OUT, SER_PIN);
	}
	else {
		setHigh(P1OUT, SER_PIN);
	}
	pulse(P1OUT, RCLK_PIN);

	nextRow = (nextRow + 1) % NUM_ROWS;

	return nextRow;
}
