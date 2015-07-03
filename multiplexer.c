/*
 * multiplexer.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */
#include "common.h"
#include "multiplexer.h"
#include "leddriver.h"
#include "stdint.h"

static uint16_t ledChannels[] = {1, 2, 3, 10, 11, 12, 21, 22, 23, 30, 33, 34, 42, 43, 44};

void Multiplexer_Init()
{
	enableGPIO(SELECT_1, SER_PIN + RCLK_PIN + SRCLK_PIN);
	enableGPIO(SELECT_2, QE_PIN);
	setOutput(P1DIR, SER_PIN);
	setOutput(P1DIR, RCLK_PIN);
	setOutput(P1DIR, SRCLK_PIN);
	setOutput(P2DIR, QE_PIN);

	//
	int i = 0;
	for(i = 0; i < NUM_REGISTERS; i++) {
		setHigh(P1OUT, SER_PIN);
		pulse(P1OUT, RCLK_PIN);
	}
	setLow(P2OUT, QE_PIN);
}

void Multiplexer_changeRow()
{
	const uint8_t numRows = 3;
	static uint8_t currentRow = 0;

	int i = 0;
	for(i = 1; i < 15; i += 3) {
		TLC5940_SetGS(ledChannels[i], 500);
	}
	TLC5940_SetGSUpdateFlag();

	//if(currentRow == 0) {
		setLow(P1OUT, SER_PIN);
		pulse(P1OUT, RCLK_PIN);
	//}
	//else {
	//	setHigh(P1OUT, SER_PIN);
	//	pulse(P1OUT, RCLK_PIN);
	//}

	//currentRow = (currentRow + 1) % numRows;
}
