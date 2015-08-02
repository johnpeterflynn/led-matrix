/*
 * leddriver.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#include "leddriver.h"
#include "multiplexer.h"
#include "spi2.h"
#include "display.h"

void TLC5940_Init(void)
{
	// Adjust clock and SMCLK speed.
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = DIVS0;

	//P1SEL |= GSCLK_PIN; // Enable output of SMCLK to GSCLK pin.

	// Configure GPIO pins.
	enableGPIO(SELECT_1, BLANK_PIN + XLAT_PIN);
	//setOutput(GSCLK_DDR, GSCLK_PIN);
	setOutput(XLAT_DDR, XLAT_PIN);
	setOutput(BLANK_DDR, BLANK_PIN);
	setLow(XLAT_PORT, XLAT_PIN);
	setHigh(BLANK_PORT, BLANK_PIN);

	// Set up timer for interrupt.
	TA0CCR0 = 2*4096-1;
	TA0CCTL0 |= CCIE;
	TA0CTL = TASSEL_2 + MC_1;

	// A SMCLK source is required for the tlc5940 but P1.4 is taken
	// by SPI. This code generates an alternative clock source using
	// as timer.
	P2DIR |= BIT2; // Set P2.2 to output direction
	P2SEL |= BIT2; // P2.2 to TA1.1

	TA1CCR0 = 1; // PWM Period
	TA1CCTL1 = OUTMOD_7; // CCR2 reset/set
	TA1CCR1 = 1;
	TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode


	// SPI.
	SPI2_Init();

	_EINT(); // Enable interrupts.

	//P2OUT &= ~BIT3;
	//P2DIR |= BIT3;
}

void TLC5940_SendDataRow(uint8_t row)
{
	fbData_t DataByte = 0;

	// WARNING: Assuming NUM_CHANNELS is even.
	// TLC5940 uses 12 bits per channel but we're only using 8.
	// The following places the two 8 bit characters in the top 8 MSB
	// positions in two channels.
	for (DataByte = 0; DataByte < NUM_CHANNELS; DataByte += 2) {
		SPI2_Send(Display_GetPixel(nextRow, DataByte));

		uint8_t tmp1 = Display_GetPixel(nextRow, DataByte + 1);

		SPI2_Send(tmp1 >> 4); // Put zeroes in top 4 MSB.

		SPI2_Send(tmp1 << 4); // Put zeroes in bottom 4 LSB.
	}

/*// This should be used instead but it doesn't work at the moment.
	for (DataByte = NUM_CHANNELS - 1; DataByte > 0 ; DataByte -= 2) {
		uint8_t tmp1 = Display_GetPixel(nextRow, DataByte);

		SPI_Send(255);//tmp1 << 4); // Put zeroes in bottom 4 LSB.
		SPI_Send(255);//tmp1 >> 4); // Put zeroes in top 4 MSB.
		SPI_Send(255);//Display_GetPixel(nextRow, DataByte - 1));
	}
*/
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	//P2OUT |= BIT3;
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
	TLC5940_SendDataRow(nextRow);

	xlatNeedsPulse = 1;
	//P2OUT &= ~BIT3;
}
