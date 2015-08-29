/*
 * DataBuffer.c
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#include "DataBuffer.h"

//static uint16_t ledChannels[] = {21, 22, 23, 30, 33, 34, 42, 43, 44};

static volatile uint8_t frameBuffer[2][NUM_PIXELS];
static volatile uint8_t currentFrame;

void DataBuffer_Init()
{
	uint8_t pixel;

	currentFrame = 0;
	//finalByteReached = 0;
	//stopBitReached = 0;

	for(pixel = 0; pixel < NUM_PIXELS; pixel++) {
		frameBuffer[0][pixel] = 0;
		frameBuffer[1][pixel] = 0;
	}
}
/*
void DataBuffer_SwappBuffers()
{
	currentFrame = 1 - currentFrame;
}*/

void DataBuffer_InputByte(uint8_t data)
{
	//static channel_t currentByte = 0;
	//static uint8_t currentRow = 0;
	//static uint8_t dataValid = 0;



	// Check that the first byte is the correct header.
/*	if(currentByte == 0) {
		if(currentRow != data) {
			currentRow = 0;
			dataValid = 0;
		}
		else if(data == 0) {
			currentRow = 0;
			dataValid = 1;
		}
	}
	else if(dataValid) {
		DataBuffer_SetPixel(currentRow * 9/*NUM_CHANNELS* + currentByte - 1, data);
	}

	if(currentByte == /*NUM_CHANNELS* 9 - 1 + 1) {
		currentByte = 0;

		if(dataValid) {
			if(currentRow == NUM_ROWS - 1) {
				currentRow = 0;
				currentFrame = 1 - currentFrame;
			}
			else {
				currentRow++;
			}
		}
	}
	else {
		currentByte++;
	}*/
}

void DataBuffer_SetPixel(uint16_t pixel, uint8_t value)
{
	//frameBuffer[currentFrame][ledChannels[pixel % 9] + (NUM_CHANNELS * (pixel / 9))] = value;
	frameBuffer[currentFrame][pixel] = value;
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
