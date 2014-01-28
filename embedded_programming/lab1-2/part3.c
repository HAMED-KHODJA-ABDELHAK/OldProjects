/* 
 * Lab 1 - Part 2(bare-bone) - Q3 -  Cycle the brightness of the LED through 4 cycles.
 */
#include "reg.h"

void setup(void) 
{
	// Set led to output
	DDRB |= (1 << LED);

	// 0A handling the voltage variation, OB pulsing.	
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); //we set WGM01 and WGM00 to 1, because this sets TCCR0A to mode3 or PULSE WIDTH MODULATION MODE.
    													  //we set COM0A1 to 1, because we want the following: "Clear OC0A on Compare Match, set OC0A at TOP"
    TCCR0B |= (1 << CS01) | (1 << CS00); //we set both CS01 and CS00 to 1, because we want clkI/O/64 (From prescaler). This is 64Hz Frequency. 
	OCR0A = 0;
}

int main(void) 
{
	setup();

	while (1) 
	{
//		_delay_ms(SHRT_DLY1);
//		PORTB |= (1 << LED); //turn ON. because we want the logical result of '0' OR '1' // '1' OR '1' to be '1'.	
		OCR0A += STEP; //Step through increments of STEP until we reach overflow (~256), where then it is reset back to 0.
		_delay_ms(SHRT_DLY1);
//		PORTB &= (0 << LED);	//turn OFF. because we want the logical result of '0' AND '1' to be '0'.	
	}
}
