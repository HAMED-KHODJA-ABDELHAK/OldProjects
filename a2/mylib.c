/**
 * Library functions that are common to both my serial and parallel
 * quicksort implementations. Linked via an archive file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "mylib.h"

/******************* Constants/Macros *********************/
#define MAX_VAL 1000000

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

/*
 * Standard increasing comparator for qsort.
 */
int compare (const void *a, const void *b)
{
	int val_a = *((int *)a), val_b = *((int *)b);
	return val_a - val_b;
}

/*
 * Function is used only to generate the random values. All values will be in one large
 * array on the heap.
 */
void gen_input(int vals[], int size) {
	srand(time(NULL));

	for (int i = 0; i < size; ++i)
		vals[i] = rand() % MAX_VAL;
}

/*
 * Opens the file passed in and continues reading input until no numbers remain.
 * Returns the number of values read into the array.
 * Important note: vals will be allocated on heap, clear later with free.
 */
void read_file(const char *file, int *vals, const int size) {
	// Note on size, I start it at -1 to compensate for the below while loop going one extra time.
	int i;
	FILE *f;

	if ((f = fopen(file, "r")) == NULL)
		m_error("READ: Failed to open file.");

	for (i = 0; i < size; ++i)
		fscanf(f, "%d", vals+i);

	if (fclose(f) != 0)
		m_error("READ: Failed to close properly.");
}

/*
 * Function takes an array of values and prints them to a file.
 * All values written to a single line with spaces, new line at end.
 */
void write_file(const char *file, const int *vals, const int size) {
	FILE *f;

	if ((f = fopen(file, "w")) == NULL)
		m_error("WRITE: Failed to open file.");

	for (int i = 0; i < size; ++i)
		fprintf(f, "%d\n", vals[i]);

	if (fclose(f) != 0)
		m_error("WRITE: Failed to close properly.");
}
