/*
 * display.h
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "leddriver.h"

//extern uint8_t frameData[2][NUM_ROWS][GS_DATA_SIZE];

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

extern uint8_t gsData[NUM_ROWS][GS_DATA_SIZE];

extern uint8_t currentFrame;

void Display_Init();

#endif /* DISPLAY_H_ */
