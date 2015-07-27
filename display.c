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
	frameBuffer[row][channel] = value;
	/*channel = NUM_CHANNELS - 1 - channel;
	uint16_t i = (uint16_t)channel * 3 / 2;

	switch (channel % 2) {
	case 0:
		frameBuffer[row][i] = (value >> 4);
		i++;
		frameBuffer[row][i] = (frameBuffer[row][i] & 0x0F) | (uint8_t)(value << 4);
		break;
	default: // case 1:
		frameBuffer[row][i] = (frameBuffer[row][i] & 0xF0) | (value >> 8);
		i++;
		frameBuffer[row][i] = (uint8_t)value;
		break;
	}*/
}

void Display_SetAllPixels(uint8_t value)
{
	uint8_t row;

	for(row = 0; row < NUM_ROWS; row++) {
		channel_t col;

		for(col = 0; col < NUM_CHANNELS; col++) {
			frameBuffer[row][col] = value;
		}
	}
	/*uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	uint8_t row = 0;

	for(row = 0; row < NUM_ROWS; row++) {
		fbData_t i = 0;

		do {
			frameBuffer[row][i++] = tmp1;
			frameBuffer[row][i++] = tmp2;
			frameBuffer[row][i++] = (uint8_t)value;
		} while (i < FB_DATA_SIZE);
	}*/
}

void Display_SetAllColor(uint8_t color, uint8_t value)
{
	uint8_t row;

	for(row = 0; row < NUM_ROWS; row++) {
		channel_t col;

		for(col = color; col < NUM_CHANNELS; col += 3) {
			frameBuffer[row][col] = value;
		}
	}
	/*uint8_t tmp1 = (value >> 4);
	uint8_t tmp2 = (uint8_t)(value >> 4) | (value << 4);
	fbData_t offset = 24 * color;
	uint8_t row = 0;

	for(row = 0; row < NUM_ROWS; row++) {
		fbData_t i = 0;

		do {
			frameBuffer[row][offset + i++] = tmp1;
			frameBuffer[row][offset + i++] = tmp2;
			frameBuffer[row][offset + i++] = (uint8_t)value;
		} while (i < 24);
	}*/
}

uint16_t Display_GetPixel(uint8_t row, channel_t channel)
{
	return frameBuffer[row][channel];
}
