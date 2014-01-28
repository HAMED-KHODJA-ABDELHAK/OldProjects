/* 
 * Lab 2 - Q3 - Use phase correct PWM to drive the connected motors.
 *              We set 1 motor in forward direction and the 2nd motor in a backward direction.
 *
 * QUESTION: 3.	Write a sketch to set up one PWM to run a motor in 
 *              a “forward” direction, and the second PWM to run the other motor one in 
 *              a “backward direction
 */
#include "reg.h"

void setup(void) 
{
    cli(); 
	
    DDRB |= (1<<PORTB7) | (1<<PORTB6) | (1<<PORTB5);
    DDRD |= (1<<PORTD0);

    //setup timer0 for motor 1
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00);
    TCCR0B = (1<<CS01)|(1<<CS00); 

    //setup timer1 for motor 2
    TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);
    TCCR1B = (1<<CS11)|(1<<CS10);


    //output compare registers for motor 1
    OCR0A = 127;//50% power.
    OCR0B = 0;
    
    //output compare registers for motor 2
    OCR1A = 0;
    OCR1B = 127;//50 % power.

    sei();

   _delay_ms(250);
}

int main(void) 
{
	setup();

	while(1)
	{	
       
	}
}
