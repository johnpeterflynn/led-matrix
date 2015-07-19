/*
 * spi.c
 *
 *  Created on: Jul 3, 2015
 *      Author: John
 */

#include "common.h"
#include "spi.h"

void SPI_Init()
{
	P1SEL  |= BIT5 + BIT7;
	P1SEL2 |= BIT5 + BIT7;

	UCB0CTL1 = UCSWRST;
	UCB0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;
	//UCB0BR0 |= 8; // /2
	//UCB0BR1 = 0;
	UCB0CTL1 |= UCSSEL_2;
	UCB0CTL1 &= ~UCSWRST;
}

void SPI_Send(uint8_t data)
{
	while (!(IFG2 & UCB0TXIFG));
	while (UCB0STAT & UCBUSY);
	UCB0TXBUF = data;
}
