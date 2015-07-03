/*
 * multiplexer.h
 *
 *  Created on: Jul 1, 2015
 *      Author: John
 */

#ifndef MULTIPLEXER_H_
#define MULTIPLEXER_H_

#define SER_PIN BIT1
#define RCLK_PIN BIT2
#define SRCLK_PIN BIT6
#define QE_PIN BIT0

#define NUM_REGISTERS 8

void Multiplexer_Init();
void Multiplexer_changeRow();

#endif /* MULTIPLEXER_H_ */
