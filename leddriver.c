/*
 * leddriver.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#include "leddriver.h"
#include "multiplexer.h"
#include "spi.h"
#include "display.h"

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
	fbData_t DataByte = 0;
	for (DataByte = 0; DataByte < FB_DATA_SIZE; DataByte++) {
		SPI_Send(Display_GetPixel(nextRow, DataByte));
	}

	xlatNeedsPulse = 1;
}
