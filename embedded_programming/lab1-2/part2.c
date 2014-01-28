/*
 * Lab 1 Part 2 - When PINF0 is high disable the LED, else light the LED. 
 */
#include "reg.h"

void setup(void) 
{
	// Set led to output.
	DDRB |= (1 << LED);
	PORTB |= (1 << LED);

	// Set read to input_pullup.
	DDRF &= ~(1 << READ); 
	PORTF |= (1 << READ);
}

int main(void) 
{
	setup();
	
	while (1) 
	{
		if (PINF & (1 << READ))
		{
			PORTB &= ~(1 << LED);
		} 
		else
		{
			PORTB |= (1 << LED);
		}
		_delay_ms(250);	
	}
}
