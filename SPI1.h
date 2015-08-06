/*
 * SPI1.h
 *
 *  Created on: Jul 3, 2015
 *      Author: John
 */

#ifndef SPI1_H_
#define SPI1_H_

#include "common.h"

#define SPI1CLK_DDR P1DIR
#define SPI1CLK_PORT P1OUT
#define SPI1CLK_PIN BIT4

#define SPI1SIMO_DDR P1DIR
#define SPI1SIMO_PORT P1OUT
#define SPI1SIMO_PIN BIT2

void SPI1_Init();
void SPI1_Send(uint8_t data);

#endif /* SPI1_H_ */
