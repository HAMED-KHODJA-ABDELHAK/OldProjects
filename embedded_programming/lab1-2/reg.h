#ifndef _REG_H_
#define _REG_H_
/****************************** Constants/Macros **************************************************/
#define F_CPU       16000000UL
#define SHRT_DLY1   50
#define STEP        64  
#define MAX_STR     75
#define LED         PORTB7
#define LEDT1A		PORTB5
#define READ 		PORTF1
#define IR_PIN		1 // F1
#define USB_SERIAL_PRIVATE_INCLUDE /* Compatability, until make better build system. */
#define ADC_STRING "adc: %d\n"

/****************************** Header Files ******************************************************/
/* AVR Headers */
#include <stdint.h> /* Standard integers, like uint8_t */
#include <stdio.h> /* IO functions like sprintf */
#include <stdlib.h> /* Bunch of standard functions, like atoi, malloc, etc... */
#include <avr/interrupt.h> /* Defines cli, sei and other interrupt features. */
#include <avr/io.h> /* Defines board specific features. */
#include <avr/pgmspace.h> /* AVR Flash memory programming. */
#include <util/delay.h> /* Delay function. */

/* Project Headers */
#include "usb_serial.h"
#include "analog.h"

/****************************** Type Definitions **************************************************/
/* For enums: Try to namesapce the common elements. 
 * typedef enum {
 *	VAL_,
 * } name_e; 
 */

/* For structs: 
 * typedef struct name_s {
 *	int index;
 * } name_t;  
 */

/****************************** Prototypes ********************************************************/

#endif /* _REG_H_ */

