/*
 * display.c
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#include "display.h"

static uint16_t ledChannels[] = {21, 22, 23, 30, 33, 34, 42, 43, 44};

uint8_t frameBuffer[NUM_PIXELS];
uint8_t currentFrame;

void Display_Init()
{
	currentFrame = 0;
}

void Display_SetPixel(uint16_t pixel, uint8_t value)
{
	//frameBuffer[pixel] = value;
	frameBuffer[ledChannels[pixel % 9] + (NUM_CHANNELS * (pixel / 9))] = value;
}

void Display_SetAllPixels(uint8_t value)
{
	uint8_t pixel;

	for(pixel = 0; pixel < NUM_PIXELS; pixel++) {
		frameBuffer[pixel] = value;
	}
}

/*void Display_SetAllColor(uint8_t color, uint8_t value)
{
	uint8_t pixel;

	for(pixel = color; pixel < NUM_CHANNELS; pixel += 3) {
		frameBuffer[pixel] = value;
	}
}*/

uint8_t Display_GetPixel(uint16_t pixel)
{
	//return frameBuffer[(row * NUM_CHANNELS) + channel];
	return frameBuffer[pixel];
}
