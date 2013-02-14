/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>

/* Project Headers */

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/
/* For enums: Try to namesapce the common elements.
 * typedef enum {
 *	VAL_,
 * } name_e;
 */

/* For structs:
 * typedef struct nam_s {
 *	int index;
 * } name_t;
 */

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Generic error function, prints out the error and terminates execution.
 */
void m_error(const char * const mesg) {
	printf("ERROR: %s.\n", mesg);
	exit(1);
}

/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
	int *vals, num_vals = argc;

	/* Get the work amount from command. */
	num_vals = atoi(*++argv);
	printf("Work is %d now.\n", num_vals);

	/* Allocate it on the heap, large amount of memory likely. */
	vals = (int *)calloc(num_vals, sizeof(int));
	if (vals == NULL)
		m_error("READ: Can't allocate vals array.");

	return 0;
}

