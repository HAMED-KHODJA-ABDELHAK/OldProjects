/* 
 * Lab 1 - Part 2(bare-bone) - Q6 - Use ADC to read the IR sensor.
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
	DDRB |= (1 << LEDT1A);

    // 0A handling the voltage variation, OB pulsing. 16BIT-Timer.  
    TCCR1A |= (1<<COM1A0);
    TCCR1B |= (1 << WGM12) | (1<<CS12) | (1<<CS10); // Set mode to CTC, compares with OCR1A, cs = fcpu/1024 = roughly 2 clocks per 1ms.
    OCR1A = 2 * 500; // Period is roughly 2 seconds.

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
		num_chars = snprintf(buf, MAX_STR -1, ADC_STRING, analogRead(IR_PIN));
		buf[num_chars] = '\0';
		send_str(buf);
		
		_delay_ms(SHRT_DLY1*3);
	}
}
