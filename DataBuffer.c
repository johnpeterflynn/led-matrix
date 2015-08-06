/*
 * DataBuffer.c
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#include "DataBuffer.h"

static uint16_t ledChannels[] = {21, 22, 23, 30, 33, 34, 42, 43, 44};

uint8_t frameBuffer[2][NUM_PIXELS];
uint8_t currentFrame;

void DataBuffer_Init()
{
	uint8_t pixel;

	currentFrame = 0;

	for(pixel = 0; pixel < NUM_PIXELS; pixel++) {
		frameBuffer[0][pixel] = 0;
		frameBuffer[1][pixel] = 0;
	}
}

void DataBuffer_SetPixel(uint16_t pixel, uint8_t value)
{
	//frameBuffer[pixel] = value;
	frameBuffer[currentFrame][ledChannels[pixel % 9] + (NUM_CHANNELS * (pixel / 9))] = value;
}

void DataBuffer_SetAllPixels(uint8_t value)
{
	uint8_t pixel;

	for(pixel = 0; pixel < NUM_PIXELS; pixel++) {
		frameBuffer[currentFrame][pixel] = value;
	}
}

uint8_t DataBuffer_GetPixel(uint16_t pixel)
{
	return frameBuffer[currentFrame][pixel];
}
