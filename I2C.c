/*
 * I2C.c
 *
 *  Created on: Aug 2, 2015
 *      Author: John
 */

#include "I2C.h"
#include "DataBuffer.h"

static volatile uint8_t finalByteReached;		// Final byte in datagram received?
static volatile uint8_t receiveActive;			// Currently receiving datagram?
static volatile channel_t currentByte;			// Number of bytes received in datagram.
static volatile uint8_t currentRow;				// Current row (aka datagram number).
static volatile uint8_t numDatagramsReceived;	// Number of datagrams processed. Used to determine if frame is valid.
static uint8_t address;

void I2C_Init(void)
{
  finalByteReached = 0;
  receiveActive = 0;
  currentByte = 0;
  currentRow = 0;
  numDatagramsReceived = 0;
  address = 0;

  P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
  P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

  P2DIR &= ~BIT5;
  if((P2IN & BIT5) != 0) {
	  address = 0x02;
  }
  else {
	  address = 0x01;
  }

  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB0I2COA = address;/*0x01;*/                         // Own Address is 001h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

  UCB0I2CIE |= UCSTPIE + UCSTTIE;           // Enable STT and STP interrupt
  IE2 |= UCB0RXIE;                          // Enable RX interrupt

  P2DIR |= BIT3;
  P2OUT &= ~BIT3;
  P2DIR |= BIT4;
  P2OUT &= ~BIT4;

  //P1OUT &= ~BIT5;
  //P1DIR |= BIT5;
}

inline void I2C_AcceptDatagram()
{
	if(numDatagramsReceived == NUM_ROWS) {
		// Frame valid.
		//P2OUT |= BIT5;
		//P2OUT &= ~BIT5;
	}

	//finalByteReached = 0;
}

inline void I2C_ResetDatagramState()
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

	if(currentByte == 0) {
		if(data == 0) {
			numDatagramsReceived = 1;
		}
		else if(data == currentRow + 1) {
			numDatagramsReceived++;
		}
		else {
			numDatagramsReceived = 0;
		}

		currentRow = data;
	}
	else if(currentByte < DATAGRAM_SIZE - 1) {
		DataBuffer_SetPixel(currentRow * NUM_CHANNELS + (currentByte - 1), data);
	}
	else if(currentByte == DATAGRAM_SIZE - 1) {
		DataBuffer_SetPixel(currentRow * NUM_CHANNELS + (currentByte - 1), data);

		finalByteReached = 1;
	}
	else {
		// Frame invalid.
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
			//P1OUT |= BIT5;
			//P1OUT &= ~BIT5;
			// It is possible to arrive in this state when the USCIAB0RX interrupt isn't
			// triggered as expected. Typically the interrupt occurs at the start and end of
			// a data transfer but less or more interrupts can occur than expected in certain
			// corner cases. If so then we flip receiveActive to attempt to correct for
			// lost interrupt signals. The frame will be lost but the datagram receiving logic
			// will quickly settle into a known operating state for the next frame.
			numDatagramsReceived = 0;
			receiveActive = 1 - receiveActive;
		}

		I2C_ResetDatagramState();
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

		I2C_ResetDatagramState();
	}

	if(receiveActive) {
		receiveActive = 0;
	}
	else {
		receiveActive = 1;
		I2C_ResetDatagramState();
	}

	P2OUT &= ~BIT4;
}
