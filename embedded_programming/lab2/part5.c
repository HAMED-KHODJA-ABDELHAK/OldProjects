/*
 * Lab 2 - Q5
 * QUESTION: 5.	Modify your hardware setup to use one PWM pin and one digital pin to each H Bridge.
 *              Modify the code so that one direction uses the digital pin as a sink, and the other
 *              direction uses the digital pin as a source.
 * NOTE: We will control both motors direction and speed through console commands
 * CONSOLE COMMANDS:
 *
 * command structure : MDVAL
 *		       M = MOTOR (1 OR 2)
 * 			D = DIRECTION (F OR R) *FORWARD OR REVERSE
 * 			 VAL = POWER (0 TO 255)
 */
#include "reg.h"

#define M1FWD OCR0A
#define M1BWD OCR0B

#define M2FWD OCR1A
#define M2BWD OCR1B

/*
 * INTERUPT FUNCTION CALLED FOR TIMER2 COMPARE AT FREQUENCY OF 1HZ
 */
ISR(TIMER2_COMPA_vect)
{
    /*
     * command structure : MDVAL
     * M = MOTOR (1 OR 2)
     * D = DIRECTION (F OR R) *FORWARD OR REVERSE
     * VAL = POWER (0 TO 255)
     */
    if (usb_serial_available())
    {
        char motor, direction;
        char buf[MAX_STR], pin[3];
        uint16_t speed = 0, cnt = 0;

        motor = usb_serial_getchar();
        direction = usb_serial_getchar();

        /* Take all chars after first, convert to integer. */
        while (usb_serial_available())
        {
            buf[cnt++] = usb_serial_getchar();
        }
        buf[cnt] = '\0';
        speed = atoi(buf);

        if (motor=='1')
        {
            if(direction=='F')
            {
                M1FWD = speed & 0xFF;
                M1BWD = 0;
            }

            else if(direction=='B')
            {
                M1BWD = speed & 0xFF;
                M1FWD = 0;
            }
        }

        else if(motor=='2')
        {
            if(direction=='F')
            {
                M2FWD = speed & 0xFF;
                M2BWD = 0;
            }

            else if(direction=='B')
            {
                M2BWD = speed & 0xFF;
                M2FWD = 0;
            }
        }
    }

}

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
 * interrupt setup with regards to timer2
 */
void setup_interrupt(void)
{
    // Reset timer 2 to 0.
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2  = 0;//initialize counter value to 0

    // set compare match register for 1hz increments
    OCR2A = 2000;// At this prescale, 1ms = 2 clocks.
    TCCR2B |= (1 << WGM22); // turn on CTC mode
    TCCR2B |= (1 << CS22) | (1 << CS20); // Set CS22 and CS20 bits for 1024 prescaler

    TIMSK2 |= (1 << OCIE2A); //ENABLE TIMER1 COMPARE INTERRUPT
}

void setup(void)
{
    cli();

    //motor1 = PORTB6 & PORTB5
    //motor2 = PORTB7 & PORTD0
    DDRB |= (1<<PORTB7)| (1<<PORTB6) | (1<<PORTB5);
    DDRD |= (1<<PORTD0);

    //setup timer0 for motor 1
    TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00);
    TCCR0B = (1<<CS01)|(1<<CS00);

    //setup timer1 for motor 2
    TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);
    TCCR1B = (1<<CS11)|(1<<CS10);

    //output compare registers for motor 1
    M1FWD = 200;
    M1BWD = 0;

    //output compare registers for motor 2
    M2FWD = 200;
    M2BWD = 0;

    setup_interrupt();

    sei();

    usb_init();
    while (!usb_configured());
    _delay_ms(250);
}
int main(void)
{
    setup();

    while(1)
    {
        //doin nothing, since we have a timer2 interrupt
    }
}
