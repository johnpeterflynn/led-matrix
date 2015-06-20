//#include <msp430.h>
#include "msp430g2553.h"
#include "stdint.h"

#define SELECT_1 P1SEL

#define BLANK_DDR P1DIR
#define BLANK_PORT P1OUT
#define BLANK_PIN BIT0

#define XLAT_DDR P1DIR
#define XLAT_PORT P1OUT
#define XLAT_PIN BIT3

#define GSCLK_DDR P1DIR
#define GSCLK_PORT P1OUT
#define GSCLK_PIN BIT4

#define SER_PIN BIT1

#define RCLK_PIN BIT2

#define SRCLK_PIN BIT6

#define enableGPIO(sel, pin) ((sel) &= ~(pin))
#define setOutput(ddr, pin) ((ddr) |= (pin))
#define setLow(port, pin) ((port) &= ~(pin))
#define setHigh(port, pin) ((port) |= (pin))
#define pulse(port, pin) do {setHigh((port), (pin));setLow((port), (pin));} while (0);

#define NUM_TLC5940 1

#if (24 * NUM_TLC5940 > 255)
	#define gsData_t uint16_t
#else
	#define gsData_t uint8_t
#endif

#if (16 * NUM_TLC5940 > 255)
	#define channel_t uint16_t
#else
	#define channel_t uint8_t
#endif

#define GS_DATA_SIZE ((gsData_t)24 * NUM_TLC5940)
#define NUM_CHANNELS ((channel_t)16 * NUM_TLC5940)

uint8_t gsData[GS_DATA_SIZE];
volatile uint8_t gsUpdateFlag; // TODO: Protect variable from compiler optimizations.

static inline void TLC5940_SetGSUpdateFlag(void) {
//__asm__ volatile ("" ::: "memory");
	gsUpdateFlag = 1;
}

void TLC5940_Init(void) {
	P1SEL |= GSCLK_PIN; // Enable output of SMCLK to GSCLK pin.

	// Configure GPIO pins.
	enableGPIO(SELECT_1, BLANK_PIN + XLAT_PIN);
	setOutput(GSCLK_DDR, GSCLK_PIN);
	setOutput(XLAT_DDR, XLAT_PIN);
	setOutput(BLANK_DDR, BLANK_PIN);
	setLow(XLAT_PORT, XLAT_PIN);
	setHigh(BLANK_PORT, BLANK_PIN);

	// Adjust clock and SMCLK speed.
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = DIVS0;

	// Set up timer for interrupt.
	CCR0 = 4096-1;
	CCTL0 |= CCIE;
	TACTL = TASSEL_2 + MC_1;

	_EINT(); // Enable interrupts.

	// SPI.
	P1SEL  |= BIT5 + BIT7;
	P1SEL2 |= BIT5 + BIT7;

	UCB0CTL1 = UCSWRST;
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	//UCB0BR0 |= 8; // /2
	//UCB0BR1 = 0;
	UCB0CTL1 |= UCSSEL_2;
	UCB0CTL1 &= ~UCSWRST;
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

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    TLC5940_Init();
    TLC5940_SetAllGS(2000);
    TLC5940_SetGSUpdateFlag();

	// Adjust clock and SMCLK speed.
	//DCOCTL = 0x00;
	//BCSCTL1 = CALBC1_16MHZ;
	//DCOCTL = CALDCO_16MHZ;
	//BCSCTL2 = DIVS0;

	enableGPIO(SELECT_1, SER_PIN + RCLK_PIN + SRCLK_PIN);
	setOutput(P1DIR, SER_PIN);
	setOutput(P1DIR, RCLK_PIN);
	setOutput(P1DIR, SRCLK_PIN);
	//setLow(XLAT_PORT, XLAT_PIN);
	//setHigh(BLANK_PORT, BLANK_PIN);

    for (;;) {
    	setHigh(P1OUT, SER_PIN);

    	int i = 0;
    	for(i = 0 ; i < 8; i++) {
    		pulse(P1OUT, SRCLK_PIN);
    		setLow(P1OUT, SER_PIN);
    		pulse(P1OUT, RCLK_PIN);
    		__delay_cycles(1600000);
    	}
    }


	return 0;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	static uint8_t xlatNeedsPulse = 0;

	setHigh(BLANK_PORT, BLANK_PIN);

	if (xlatNeedsPulse) {
		pulse(XLAT_PORT, XLAT_PIN);
		xlatNeedsPulse = 0;
	}

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
