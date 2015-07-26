/*
 * leddriver.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#include "common.h"
#include "leddriver.h"
#include "multiplexer.h"
#include "spi.h"

uint8_t gsData[NUM_ROWS][GS_DATA_SIZE];

void TLC5940_Init(void)
{
	// Adjust clock and SMCLK speed.
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = DIVS0;

	P1SEL |= GSCLK_PIN; // Enable output of SMCLK to GSCLK pin.

	// Configure GPIO pins.
	enableGPIO(SELECT_1, BLANK_PIN + XLAT_PIN);
	setOutput(GSCLK_DDR, GSCLK_PIN);
	setOutput(XLAT_DDR, XLAT_PIN);
	setOutput(BLANK_DDR, BLANK_PIN);
	setLow(XLAT_PORT, XLAT_PIN);
	setHigh(BLANK_PORT, BLANK_PIN);

	// Set up timer for interrupt.
	CCR0 = 4096-1;
	CCTL0 |= CCIE;
	TACTL = TASSEL_2 + MC_1;

	// SPI.
	SPI_Init();

	_EINT(); // Enable interrupts.
}

void TLC5940_SetGS(uint8_t row, channel_t channel, uint16_t value)
{
	channel = NUM_CHANNELS - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 2;

	switch (channel % 2) {
	case 0:
		gsData[row][i] = (value >> 4);
		i++;
		gsData[row][i] = (gsData[row][i] & 0x0F) | (uint8_t)(value << 4);
		break;
	default: // case 1:
		gsData[row][i] = (gsData[row][i] & 0xF0) | (value >> 8);
		i++;
		gsData[row][i] = (uint8_t)value;
		break;
	}
}

void TLC5940_SetAllGS(uint16_t value)
{
	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	uint8_t row = 0;

	for(row = 0; row < NUM_ROWS; row++) {
		gsData_t i = 0;

		do {
			gsData[row][i++] = tmp1;
			gsData[row][i++] = tmp2;
			gsData[row][i++] = (uint8_t)value;
		} while (i < GS_DATA_SIZE);
	}
}

void TLC5940_SetColorGS(uint16_t color, uint16_t value)
{
	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	gsData_t offset = 24 * color;
	uint8_t row = 0;

	for(row = 0; row < NUM_ROWS; row++) {
		gsData_t i = 0;

		do {
			gsData[row][offset + i++] = tmp1;
			gsData[row][offset + i++] = tmp2;
			gsData[row][offset + i++] = (uint8_t)value;
		} while (i < 24);
	}
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	//const unsigned int NUMPulses = 500;
	//static unsigned int PWMCount = NUMPulses;
	static uint8_t xlatNeedsPulse = 0;
	uint8_t nextRow = 0;

	setHigh(BLANK_PORT, BLANK_PIN); // Turn off the current row.

	nextRow = Multiplexer_changeRow(); // Switch to the next row.

	if (xlatNeedsPulse) {
		pulse(XLAT_PORT, XLAT_PIN); // Latch in data for next row.
		xlatNeedsPulse = 0;
	}

	setLow(BLANK_PORT, BLANK_PIN); // Turn on the next row.

	// Below this we have 4096 cycles to shift in the data for the next cycle
	gsData_t DataByte = 0;

	for (DataByte = 0; DataByte < GS_DATA_SIZE; DataByte++) {
		SPI_Send(gsData[nextRow][DataByte]);
	}

	xlatNeedsPulse = 1;
}
