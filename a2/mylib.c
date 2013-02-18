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


/******************* Type Definitions *********************/


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
int compare (const void *a, const void *b) {
	return *((int *)a) - *((int *)b);
}

/*
 * Function generates size random numbers between 0 and MAX_VAL. All values put in vals array.
 */
void gen_input(int vals[], int size) {
	srand(time(NULL));

	for (int i = 0; i < size; ++i)
		vals[i] = rand() % MAX_VAL;
}

/*
 * Opens the filename passed in and reads size numbers into the vals array.
 */
void read_file(const char *filename, int *vals, const int size) {
	// Note on size, I start it at -1 to compensate for the below while loop going one extra time.
	int i;
	FILE *f;

	if ((f = fopen(filename, "r")) == NULL)
		m_error("READ: Failed to open file.");

	for (i = 0; i < size; ++i)
		fscanf(f, "%d", vals+i);

	if (fclose(f) != 0)
		m_error("READ: Failed to close properly.");
}

/*
 * Function takes an array of values and prints them to a file.
 * Each integer on a separate line.
 */
void write_file(const char *filename, const int *vals, const int size) {
	FILE *f;

	if ((f = fopen(filename, "w")) == NULL)
		m_error("WRITE: Failed to open file.");

	for (int i = 0; i < size; ++i)
		fprintf(f, "%d\n", vals[i]);

	if (fclose(f) != 0)
		m_error("WRITE: Failed to close properly.");
}
