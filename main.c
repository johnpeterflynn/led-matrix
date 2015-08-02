#include "common.h"
#include "leddriver.h"
#include "multiplexer.h"
#include "display.h"

static uint16_t ledChannels[] = {1, 2, 3, 10, 11, 12, 21, 22, 23, 30, 33, 34, 42, 43, 44};

void test()
{
	DCOCTL = 0x00;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;
	BCSCTL2 = DIVS0;

	P2DIR |= BIT4;
	P2OUT &= ~BIT4;

	// Set up timer for interrupt.
	TA0CCR0 = 2*15-1;
	TA0CCTL0 |= CCIE;
	TA0CTL = TASSEL_2 + MC_1;

	////----
	P2DIR |= BIT2; // Set P2.2 to output direction
	P2SEL |= BIT2; // P2.2 to TA1.1

	TA1CCR0 = 1; // PWM Period
	TA1CCTL1 = OUTMOD_7; // CCR2 reset/set
	TA1CCR1 = 1;
	TA1CTL = TASSEL_2 + MC_1; // SMCLK, up mode
	////----

	_EINT(); // Enable interrupts.
}

void setup()
{
	Multiplexer_Init();
    TLC5940_Init();
    Display_Init();
    Display_SetAllPixels(0);

	/*int i = 0;
	int j = 0;
	for(i = 0; i < 5; i++) {
		for(j = 0; j < 3; j++) {
			Display_SetPixel(j, ledChannels[i*3+j], 4095);
		}
	}*/

    int brightness = 255;
    Display_SetPixel(0, ledChannels[6], brightness);
    Display_SetPixel(0, ledChannels[7], brightness);
    Display_SetPixel(0, ledChannels[8], brightness);

    Display_SetPixel(0, ledChannels[9], brightness);
    Display_SetPixel(0, ledChannels[10], brightness);
    Display_SetPixel(0, ledChannels[11], brightness);

    //Display_SetPixel(2, ledChannels[3], 500);
    //Display_SetPixel(2, ledChannels[4], 500);
    //Display_SetPixel(2, ledChannels[5], 500);
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

	//for(;;) {
	//}
}

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    setup();
    //test();

    for(;;) {
    	loop();
    	//__bis_SR_register(LPM4_bits + GIE);       // Enter LPM4, enable interrupts
    }

	return 0;
}

/*#pragma vector=TIMER0_A0_VECTOR//TIMERA0_VECTOR
__interrupt void Timer_A (void)
{
	P2OUT ^= BIT4;
}*/

