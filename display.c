/*
 * display.c
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#include "display.h"

uint8_t frameBuffer[NUM_ROWS][NUM_CHANNELS];
uint8_t currentFrame;

void Display_Init()
{
	currentFrame = 0;
}

void Display_SetPixel(uint8_t row, channel_t channel, uint8_t value)
{
	frameBuffer[row][NUM_CHANNELS - 1 - channel] = value;
}

void Display_SetAllPixels(uint8_t value)
{
	uint8_t row;

	for(row = 0; row < NUM_ROWS; row++) {
		channel_t channel;

		for(channel = 0; channel < NUM_CHANNELS; channel++) {
			frameBuffer[row][NUM_CHANNELS - 1 - channel] = value;
		}
	}
}

void Display_SetAllColor(uint8_t color, uint8_t value)
{
	uint8_t row;

	for(row = 0; row < NUM_ROWS; row++) {
		channel_t channel;

		for(channel = color; channel < NUM_CHANNELS; channel += 3) {
			frameBuffer[row][NUM_CHANNELS - 1 - channel] = value;
		}
	}
}

uint8_t Display_GetPixel(uint8_t row, channel_t channel)
{
	return frameBuffer[row][channel];
}
