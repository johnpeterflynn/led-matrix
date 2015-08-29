/*
 * TLC5940.c
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#include "TLC5940.h"
#include "Multiplexer.h"
#include "SPI1.h"
#include "DataBuffer.h"

// Greater resolution and linearized brightness is achieved by applying gamma
// correction to the LEDs and using 12 bit values which is supported by the TLC5940.
const uint16_t GammaCorrection[] = {
		   0,    1,    1,    1,    1,    2,    2,    3,    4,    5,    6,    8,    9,   11,   12,   14,
		  16,   18,   20,   23,   25,   28,   30,   33,   36,   39,   42,   46,   49,   53,   56,   60,
		  64,   68,   72,   77,   81,   86,   90,   95,  100,  105,  110,  116,  121,  127,  132,  138,
		 144,  150,  156,  163,  169,  176,  182,  189,  196,  203,  210,  218,  225,  233,  240,  248,
		 256,  264,  272,  281,  289,  298,  306,  315,  324,  333,  342,  352,  361,  371,  380,  390,
		 400,  410,  420,  431,  441,  452,  462,  473,  484,  495,  506,  518,  529,  541,  552,  564,
		 576,  588,  600,  613,  625,  638,  650,  663,  676,  689,  702,  716,  729,  743,  756,  770,
		 784,  798,  812,  827,  841,  856,  870,  885,  900,  915,  930,  946,  961,  977,  992, 1008,
		1024, 1040, 1057, 1073, 1089, 1106, 1123, 1139, 1156, 1173, 1191, 1208, 1225, 1243, 1261, 1278,
		1296, 1314, 1333, 1351, 1369, 1388, 1407, 1425, 1444, 1463, 1483, 1502, 1521, 1541, 1561, 1580,
		1600, 1620, 1641, 1661, 1681, 1702, 1723, 1743, 1764, 1785, 1807, 1828, 1849, 1871, 1893, 1915,
		1936, 1959, 1981, 2003, 2025, 2048, 2071, 2094, 2117, 2140, 2163, 2186, 2210, 2233, 2257, 2281,
		2305, 2329, 2353, 2377, 2402, 2426, 2451, 2476, 2501, 2526, 2551, 2576, 2602, 2627, 2653, 2679,
		2705, 2731, 2757, 2783, 2810, 2836, 2863, 2890, 2917, 2944, 2971, 2998, 3026, 3053, 3081, 3109,
		3137, 3165, 3193, 3221, 3250, 3278, 3307, 3336, 3365, 3394, 3423, 3452, 3482, 3511, 3541, 3571,
		3601, 3631, 3661, 3691, 3722, 3752, 3783, 3814, 3845, 3876, 3907, 3939, 3970, 4002, 4033, 4065
};

void TLC5940_Init(void)
{
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
	TA0CTL = TASSEL_2 + MC_1 + ID_2;

	// A SMCLK source is required for the tlc5940 but P1.4 is taken
	// by SPI. This code generates an alternative clock source using
	// as timer.
	P2DIR |= BIT2; // Set P2.2 to output direction
	P2SEL |= BIT2; // P2.2 to TA1.1

	TA1CCR0 = 1; // PWM Period
	TA1CCTL1 = OUTMOD_7; // CCR2 reset/set
	TA1CCR1 = 1;
	TA1CTL = TASSEL_2 + MC_1 + ID_2; // SMCLK, up mode. div by 4

	// SPI.
	SPI1_Init();

	  P1DIR |= BIT5;
	  P1OUT &= ~BIT5;
}

/*void TLC5940_SendDataRow(uint8_t row)
{
	fbData_t DataByte = NUM_CHANNELS - 1 + 2;
	uint16_t pixelOffset = row * NUM_CHANNELS;

	// WARNING: Assuming NUM_CHANNELS is even.
	// TLC5940 uses 12 bits per channel but we're only using 8.
	// The following places the two 8 bit characters in the top 8 MSB
	// positions in two channels.
	// Awkward loop but this structure removes complexity from display.c.
	do {
		DataByte -= 2;

		SPI1_Send(DataBuffer_GetPixel(pixelOffset + DataByte));

		uint8_t tmp1 = DataBuffer_GetPixel(pixelOffset + DataByte - 1);

		SPI1_Send(tmp1 >> 4); // Put zeroes in top 4 MSB.
		SPI1_Send(tmp1 << 4); // Put zeroes in bottom 4 LSB.

	} while(DataByte > 1);

}*/

void TLC5940_SendDataRow(uint8_t row)
{
	fbData_t DataByte = NUM_CHANNELS - 1 + 2;
	uint16_t pixelOffset = row * NUM_CHANNELS;

	// WARNING: Assuming NUM_CHANNELS is even.
	// Awkward loop but this structure removes complexity from display.c.
	do {
		DataByte -= 2;

		uint16_t first = GammaCorrection[DataBuffer_GetPixel(pixelOffset + DataByte)];
		uint16_t second = GammaCorrection[DataBuffer_GetPixel(pixelOffset + DataByte - 1)];

		//SPI1_Send(0);
		//SPI1_Send(0);
		//SPI1_Send(0);
		SPI1_Send(255);//first >> 4);
		SPI1_Send(255);//first << 4 | second >> 8);
		SPI1_Send(255);//second);

	} while(DataByte > 1);

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	P1OUT |= BIT5;
	// Since there is ample time to shift the next row of data into the TLC5940
	// we call _EINT() (enable interrupts) to allow other more time sensitive interrupts
	// associated with I2C to run with fewer problems.
	_EINT();

	//P2OUT |= BIT1;

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

	P1OUT &= ~BIT5;
}
