/*
 * I2C.c
 *
 *  Created on: Aug 2, 2015
 *      Author: John
 */

#include "I2C.h"
#include "DataBuffer.h"

//uint8_t PRxData;                     // Pointer to RX data
channel_t nextPixel;

void I2C_Init(void)
{
  nextPixel = 0;

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

}

//------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C master
// to the MSP430 memory.
//------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  P2OUT |= BIT3;

  DataBuffer_SetPixel(nextPixel, UCB0RXBUF);

  nextPixel = (nextPixel + 1) % (NUM_ROWS * 9);

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
  P2OUT |= BIT4;
  UCB0STAT &= ~(UCSTPIFG + UCSTTIFG);       // Clear interrupt flags
  P2OUT &= ~BIT4;
}
