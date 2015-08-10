/*
 * I2C.c
 *
 *  Created on: Aug 2, 2015
 *      Author: John
 */

#include "I2C.h"
#include "DataBuffer.h"

volatile static uint8_t finalByteReached;
volatile static uint8_t receiveActive;
//volatile static uint8_t frameInvalid;
volatile static channel_t currentByte;
volatile static uint8_t currentRow;

void I2C_Init(void)
{
  finalByteReached = 0;
  receiveActive = 0;
  //frameInvalid = 0;
  currentByte = 0;
  currentRow = 0;

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB0I2COA = 0x01;                         // Own Address is 001h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

  UCB0I2CIE |= UCSTPIE + UCSTTIE;           // Enable STT and STP interrupt
  IE2 |= UCB0RXIE;                          // Enable RX interrupt

  P2DIR |= BIT3;
  P2OUT &= ~BIT3;
  P2DIR |= BIT4;
  P2OUT &= ~BIT4;
  P2OUT &= ~BIT5;
  P2DIR |= BIT5;

  P1OUT &= ~BIT5;
  P1DIR |= BIT5;
}

void I2C_AcceptDatagram()
{
	P2OUT |= BIT5;
	// Datagram valid.

	if(currentRow == NUM_ROWS - 1) {
		// Frame valid.
	}

	finalByteReached = 0;
	P2OUT &= ~BIT5;
}

void I2C_ResetState()
{
	currentByte = 0;
	finalByteReached = 0;
}

//------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C master
// to the MSP430 memory.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	P2OUT |= BIT3;

	uint8_t data = UCB0RXBUF;

	//int i = 0;
	//for(i = 0; i < 100; i++){}

	if(currentByte == 0) {
		currentRow = data;
	}
	else if(currentByte < DATAGRAM_SIZE - 1) {
		DataBuffer_SetPixel(currentRow * 9/*NUM_CHANNELS*/ + (currentByte - 1), data);
	}
	else if(currentByte == DATAGRAM_SIZE - 1) {
		DataBuffer_SetPixel(currentRow * 9/*NUM_CHANNELS*/ + (currentByte - 1), data);

		finalByteReached = 1;
	}
	else {
		// Frame invalid.
		//frameInvalid = 1;

	}

	if(receiveActive) {
		currentByte++;
	}
	else {
		if(finalByteReached) {
			I2C_AcceptDatagram();
		}
		else {
			// Datagram invalid.
			P1OUT |= BIT5;
			P1OUT &= ~BIT5;
		}

		I2C_ResetState();
	}

	P2OUT &= ~BIT3;
}

//------------------------------------------------------------------------------
// The USCI_B0 state ISR is used to wake up the CPU from LPM0 in order to
// process the received data in the main program. LPM0 is only exit in case
// of a (re-)start or stop condition when actual data was received.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);       // Clear interrupt flags

	P2OUT |= BIT4;

	if(finalByteReached && receiveActive) {
		I2C_AcceptDatagram();

		// Datagram valid.
		I2C_ResetState();
	}

	//receiveActive = 1 - receiveActive;
	if(receiveActive) {
		receiveActive = 0;
	}
	else {
		receiveActive = 1;
		I2C_ResetState();
	}

	P2OUT &= ~BIT4;
}
