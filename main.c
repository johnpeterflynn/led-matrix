#include "common.h"
#include "leddriver.h"
#include "multiplexer.h"

static uint16_t ledChannels[] = {1, 2, 3, 10, 11, 12, 21, 22, 23, 30, 33, 34, 42, 43, 44};

void setup()
{
	Multiplexer_Init();

    TLC5940_Init();
    TLC5940_SetAllGS(0);

	/*int i = 0;
	int j = 0;
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 3; j++) {
			TLC5940_SetGS(j, ledChannels[i*3+j], 4095);
		}
	}*/

    int brightness = 2000;
    TLC5940_SetGS(0, ledChannels[0], brightness);
    TLC5940_SetGS(1, ledChannels[1], brightness);
    TLC5940_SetGS(2, ledChannels[2], brightness);

    TLC5940_SetGS(0, ledChannels[9], brightness);
    TLC5940_SetGS(1, ledChannels[10], brightness);
    TLC5940_SetGS(2, ledChannels[11], brightness);

    //TLC5940_SetGS(2, ledChannels[3], 500);
    //TLC5940_SetGS(2, ledChannels[4], 500);
    //TLC5940_SetGS(2, ledChannels[5], 500);
}

void loop()
{
	//while(gsUpdateFlag);
	//Multiplexer_changeRow();
	//TLC5940_SetGSUpdateFlag();
    /*for (;;) {
		setHigh(P1OUT, SER_PIN);
		pulse(P1OUT, RCLK_PIN);
		__delay_cycles(160000);

    	int i = 0;
    	for(i = 0 ; i < 8; i++) {
    		setLow(P1OUT, SER_PIN);
    		pulse(P1OUT, RCLK_PIN);
    		__delay_cycles(160000);
    	}
    }*/

    /*channel_t i = 0;
    for(;;) {
    	while(gsUpdateFlag);
    	TLC5940_SetAllGS(0);
    	TLC5940_SetGS(i, 2000);
    	TLC5940_SetGSUpdateFlag();
    	__delay_cycles(1600000);
    	i = (i+1) % NUM_CHANNELS;
    }*/

	for(;;) {
	}
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    setup();

    for(;;) {
    	loop();
    }

	return 0;
}

