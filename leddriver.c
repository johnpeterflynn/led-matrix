/*
 * leddriver.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#include "common.h"
#include "leddriver.h"
#include "multiplexer.h"

uint8_t gsData[GS_DATA_SIZE];
volatile uint8_t gsUpdateFlag; // TODO: Protect variable from compiler optimizations.

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
	P1SEL  |= BIT5 + BIT7;
	P1SEL2 |= BIT5 + BIT7;

	UCB0CTL1 = UCSWRST;
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	//UCB0BR0 |= 8; // /2
	//UCB0BR1 = 0;
	UCB0CTL1 |= UCSSEL_2;
	UCB0CTL1 &= ~UCSWRST;

	_EINT(); // Enable interrupts.
}

void TLC5940_SetGS(channel_t channel, uint16_t value)
{
	channel = NUM_CHANNELS - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 2;

	switch (channel % 2) {
	case 0:
		gsData[i] = (value >> 4);
		i++;
		gsData[i] = (gsData[i] & 0x0F) | (uint8_t)(value << 4);
		break;
	default: // case 1:
		gsData[i] = (gsData[i] & 0xF0) | (value >> 8);
		i++;
		gsData[i] = (uint8_t)value;
		break;
	}
}

void TLC5940_SetAllGS(uint16_t value)
{
	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	gsData_t i = 0;

	do {
		gsData[i++] = tmp1;
		gsData[i++] = tmp2;
		gsData[i++] = (uint8_t)value;
	} while (i < GS_DATA_SIZE);
}

void TLC5940_SetColorGS(uint16_t color, uint16_t value)
{
	uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	gsData_t i = 0;
	gsData_t offset = 24 * color;

	do {
		gsData[offset + i++] = tmp1;
		gsData[offset + i++] = tmp2;
		gsData[offset + i++] = (uint8_t)value;
	} while (i < 24);
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	const unsigned int NUMPulses = 500;
	static unsigned int PWMCount = NUMPulses;
	static uint8_t xlatNeedsPulse = 0;

	setHigh(BLANK_PORT, BLANK_PIN);

	if (xlatNeedsPulse) {
		pulse(XLAT_PORT, XLAT_PIN);
		xlatNeedsPulse = 0;
	}

	//if(PWMCount < NUMPulses) {
	//	PWMCount++;
	//}
	//else {
		//changeRow();
	//	PWMCount = 0;
	//}

	setLow(BLANK_PORT, BLANK_PIN);

	// Below this we have 4096 cycles to shift in the data for the next cycle
	if(gsUpdateFlag) {
		gsData_t DataByte = 0;

		for (DataByte = 0; DataByte < GS_DATA_SIZE; DataByte++) {
			UCB0TXBUF = gsData[DataByte];
			while (!(IFG2 & UCB0TXIFG));
			while (UCB0STAT & UCBUSY);
		}

		xlatNeedsPulse = 1;
		gsUpdateFlag = 0;
	}
}
