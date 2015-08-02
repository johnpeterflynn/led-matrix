/*
 * spi2.h
 *
 *  Created on: Jul 3, 2015
 *      Author: John
 */

#ifndef SPI2_H_
#define SPI2_H_

#include "common.h"

#define SPI2CLK_DDR P1DIR
#define SPI2CLK_PORT P1OUT
#define SPI2CLK_PIN BIT4

#define SPI2SIMO_DDR P1DIR
#define SPI2SIMO_PORT P1OUT
#define SPI2SIMO_PIN BIT2

void SPI2_Init();
void SPI2_Send(uint8_t data);

#endif /* SPI2_H_ */
