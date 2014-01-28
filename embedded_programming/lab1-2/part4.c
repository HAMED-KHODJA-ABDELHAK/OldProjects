/* 
 * Lab 1 - Part 2(bare-bone) - Q4 -  Print brightness to teensy monitor.
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

void setup(void) 
{
	// Set led to output
	DDRB |= (1 << LED);

	// 0A handling the voltage variation, OB pulsing.	
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00); //we set WGM01 and WGM00 to 1, because this sets TCCR0A to mode3 or PULSE WIDTH MODULATION MODE.
    													  //we set COM0A1 to 1, because we want the following: "Clear OC0A on Compare Match, set OC0A at TOP"
    TCCR0B |= (1 << CS01) | (1 << CS00); //we set both CS01 and CS00 to 1, because we want clkI/O/64 (From prescaler). This is 64Hz Frequency. 
	OCR0A = 0;
	
	usb_init();
	while (!usb_configured());
	_delay_ms(250);
}

int main(void) 
{
	setup();
	
	char buf[MAX_STR] = "\0";
	int num_chars = 0;

	while (1) 
	{
		_delay_ms(SHRT_DLY1);
		PORTB |= (1 << LED); //turn ON. because we want the logical result of '0' OR '1' // '1' OR '1' to be '1'.	
		OCR0A += STEP; //Step through increments of STEP until we reach overflow (~256), where then it is reset back to 0.
	
		int val0A = OCR0A;
	
		/* Write the brightness to a buffer and send. */
		num_chars = snprintf(buf, MAX_STR -1, "The brightness is: %d\n", val0A);
		buf[num_chars] = '\0';
		send_str(buf);
		
		_delay_ms(SHRT_DLY1);
		PORTB &= (0 << LED);	//turn OFF. because we want the logical result of '0' AND '1' to be '0'.	
	}
}
