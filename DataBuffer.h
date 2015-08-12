/*
 * DataBuffer.h
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

#ifndef DATABUFFER_H_
#define DATABUFFER_H_

#include "common.h"
#include "TLC5940.h"

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

#define NUM_CHANNELS ((channel_t)16 * NUM_TLC5940)
#define NUM_PIXELS (NUM_ROWS * NUM_CHANNELS)

//extern volatile uint8_t frameBuffer[2][NUM_PIXELS];
//extern volatile uint8_t currentFrame;

void DataBuffer_Init();
inline void DataBuffer_SetRearFramePopulated();
inline uint8_t DataBuffer_GetRearFramePopulated();
void DataBuffer_SwapFrames();
void DataBuffer_InputByte(uint8_t data);
void DataBuffer_SetPixel(uint16_t pixel, uint8_t value);
void DataBuffer_SetAllPixels(uint8_t value);
//uint8_t DataBuffer_GetPixel(uint8_t row, channel_t channel);
uint8_t DataBuffer_GetPixel(uint16_t pixel);


#endif /* DATABUFFER_H_ */
