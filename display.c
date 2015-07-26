/*
 * display.c
 *
 *  Created on: Jul 19, 2015
 *      Author: John
 */

//extern uint8_t frameData[2][NUM_ROWS][GS_DATA_SIZE];

extern uint8_t currentFrame;

void Display_Init()
{
	currentFrame = 0;
}
