/* 
 * Lab 1 - Part 2(bare-bone) - Q5 -  16bit timer for LED blinking.
 */
#include "reg.h"

void send_str(const char *s)
{
	char c;
	while (1) {
		c = *s++; // Take the char.
		if (c == '\0') // Exit on '\0' byte.
		    break;
		usb_serial_putchar(c);
	}
	usb_serial_flush_output();
}

/*
 * INTERUPT FUNCTION CALLED FOR TIMER1 COMPARE AT FREQUENCY OF 1HZ
 */
ISR(TIMER1_COMPA_vect)
{
	cli();
	int16_t val = analogRead(IR_PIN);
	sei();
	char buf[MAX_STR];
	uint8_t num_chars = snprintf(buf, MAX_STR -1, ADC_STRING, val);
	buf[num_chars] = '\0';
	send_str(buf);
}

void setup(void) 
{
    cli(); 
	
	// Set led to output
    DDRB |= (1 << LED);

    // 0A handling the voltage variation, OB pulsing.   
    TCCR0A = (1 << COM0A0) | (1 << WGM01) | (1 << WGM00); //we set WGM01 and WGM00 to 1, because this sets TCCR0A to mode3 or PULSE WIDTH MODULATION MODE.
                                                          //we set COM0A1 to 1, because we want the following: "Clear OC0A on Compare Match, set OC0A at TOP"
    TCCR0B |= (1 << CS02)| (1 << CS00)|(1<<WGM02); //we set both CS01 and CS00 to 1, because we want clkI/O/64 (From prescaler). This is 64Hz Frequency. 
    OCR0A = 127;

    // Reset timer 1 to 0.
    TCCR1A = 0;// set entire TCCR1A register to 0
    TCCR1B = 0;// same for TCCR1B
    TCNT1  = 0;//initialize counter value to 0

    // set compare match register for 1hz increments
    OCR1A = 2000;// At this prescale, 1ms = 2 clocks.
    //EXPLAINS WHY WE PUT IT INTO CTC MODE: http://maxembedded.wordpress.com/2011/07/14/avr-timers-ctc-mode/
    TCCR1B |= (1 << WGM12); // turn on CTC mode 
    TCCR1B |= (1 << CS12) | (1 << CS10); // Set CS12 and CS10 bits for 1024 prescaler

    TIMSK1 |= (1 << OCIE1A); //ENABLE TIMER1 COMPARE INTERRUPT
             
    sei();

	usb_init();
	while (!usb_configured());
	_delay_ms(250);
}

int main(void) 
{
	setup();
	
	/* Test string construct. */
	char buf[MAX_STR] = "\0";
	uint8_t num_chars = snprintf(buf, MAX_STR -1, "Printing in while loop.\n");
	buf[num_chars] = '\0';

	while (1) 
	{
		// Empty now.
	}
}
