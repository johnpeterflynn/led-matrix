#include "common.h"
#include "TLC5940.h"
#include "Multiplexer.h"
#include "DataBuffer.h"
#include "I2C.h"

//static uint16_t ledChannels[] = {1, 2, 3, 10, 11, 12, 21, 22, 23, 30, 33, 34, 42, 43, 44};
 static uint16_t ledChannels[] = {21, 22, 23, 30, 33, 34, 42, 43, 44};

void setup()
{
	// Adjust clock and SMCLK speed.
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_16MHZ;
	DCOCTL = CALDCO_16MHZ;
	BCSCTL2 = DIVS0;

	DataBuffer_Init();
	DataBuffer_SetAllPixels(0);
	Multiplexer_Init();
    TLC5940_Init();
    I2C_Init();

	_EINT(); // Enable interrupts.

    //int brightness = 255;
    //DataBuffer_SetPixel(ledChannels[0], brightness);
    //DataBuffer_SetPixel(ledChannels[4], brightness);
    //DataBuffer_SetPixel(ledChannels[7], brightness);
}

void loop()
{

}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    setup();
    //test();

    for(;;) {
    	loop();
    	//__bis_SR_register(LPM1_bits + GIE);       // Enter LPM4, enable interrupts
    }

	return 0;
}

