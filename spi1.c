/*
 * spi1.c
 *
 *  Created on: Jul 29, 2015
 *      Author: John
 */

/*void SPI1_Init()
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

  P1DIR |= BIT0;
  P1OUT &= ~BIT0;

  P1OUT &= ~BIT3;
  P1DIR |= BIT3;

 // while (!(P1IN & BIT4));                   // If clock sig from mstr stays low,
                                            // it is not yet in SPI mode
  //DCOCTL = CALDCO_16MHZ;
  //BCSCTL1 = CALBC1_16MHZ;

  //P1OUT |= BIT0;

  P1DIR |= BIT1;
  P1DIR &= ~BIT2;
  P1DIR &= ~BIT4;
  P1DIR &= ~BIT5;

  P1SEL = BIT1 + /*BIT2 +* BIT4 + BIT5;				// RXD, TXD, SCLK, STE // SOMI, SIMO?
  P1SEL2 = BIT1 /*+ BIT2 +* BIT4 + BIT5;
  UCA0CTL1 = UCSWRST;                       // **Put state machine in reset**
  UCA0CTL0 |= /*UCCKPH +* UCMSB + UCSYNC + UCMODE_1;      // 8-bit SPI master
  UCA0CTL0 &= ~UCCKPL;
  UCA0CTL0 &= ~UCCKPH;
  UCA0CTL0 &= ~UCMST;
  UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
  IE2 |= UCA0RXIE;                          // Enable USCI0 RX interrupt

  //_EINT(); // Enable interrupts.
  __bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
}

// Echo character
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR (void)
{
  while (!(IFG2 & UCA0RXIFG));
  uint8_t data = UCA0RXBUF;

  P1OUT ^= BIT0;
  if(data == 103)
    P1OUT ^= BIT3;
}*/


