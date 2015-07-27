/*
 * display.h
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "common.h"
#include "leddriver.h"

#if (16 * NUM_TLC5940 > 255)
	#define fbData_t uint16_t
#else
	#define fbData_t uint8_t
#endif

#if (16 * NUM_TLC5940 > 255)
	#define channel_t uint16_t
#else
	#define channel_t uint8_t
#endif

//#define FB_DATA_SIZE ((fbData_t)24 * NUM_TLC5940)
#define NUM_CHANNELS ((channel_t)16 * NUM_TLC5940)

extern uint8_t frameBuffer[NUM_ROWS][NUM_CHANNELS];

extern uint8_t currentFrame;

void Display_Init();

void Display_SetPixel(uint8_t row, channel_t channel, uint8_t value);

void Display_SetAllPixels(uint8_t value);

void Display_SetAllColor(uint8_t color, uint8_t value);

uint16_t Display_GetPixel(uint8_t row, channel_t channel);


#endif /* DISPLAY_H_ */
