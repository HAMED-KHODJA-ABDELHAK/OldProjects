/**
 * Serial implementation of pi calculation, used as baseline.
 * Will serially throw the number of darts passed as argument. If no darts argument, defaults to same as
 * the total darts for all rounds in the llnl parallel example.
 * Reports the time taken and deviation from reference PI, reference is first line of
 * 		(http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html).
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/serial <darts>
 *
 * Arguments to serial:
 * darts: Number of rounds across all workers. If not provided, uses DEF_DARTS.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define INPUT "input.txt"
#define OUTPUT "output.txt"
#define MAX_VAL 1000

/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/*
 * Generic error function, prints out the error and terminates execution.
 */
void m_error(const char * const mesg) {
	printf("ERROR: %s.\n", mesg);
	exit(1);
}

/*
 * Function is used only to generate the random values.
 */
void gen_input(int vals[], int size) {
	for (int i = 0; i < size; ++i)
		vals[i] = rand % MAX_VAL;
}

/*
 * Opens the file passed in and continues reading input until no numbers remain.
 * Returns the number of values read into the array.
 * Important note: vals will be allocated on heap, clear later with free.
 */
int read_file(const char *file, int *vals) {
	int size = 0, i;
	FILE *f;

	if ((f = fopen(file, 'r')) == NULL)
		m_error("READ: Failed to open file.");

	/* Keep scanning the file for ints and incrementing size. */
	while ((fscanf(f, "%d ", i)) != 0)
		++size;

	/* Go back to beginning of file, allocate an array of ints size big. */
	rewind(f);
	vals = calloc(size, sizeof(int));

	/* Now put them in the final array. */
	for (i = 0; i < size; ++i)
		fscanf(f, "%d ", vals[i]);

	if (fclose(f) != 0)
		m_error("READ: Failed to close properly.");

	return size;
}

/*
 * Function takes an array of values and prints them to a file.
 * All values written to a single line with spaces, new line at end.
 */
void write_file(const char *file, const int *vals, const int size) {
	FILE *f;

	if ((f = fopen(file, 'w')) == NULL)
		m_error("WRITE: Failed to open file.");

	for (int i = 0; i < size; ++i)
		fprintf(f, "%d ", vals[i]);
	fprintf(f, "\n");

	if (fclose(f) != 0)
		m_error("WRITE: Failed to close properly.");
}

int main(int argc, char **argv) {
	int rank, size, darts, hits = 0;
	double start, pi;

	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL));

	/* Test code. */
	int ar[64];
	gen_input(ar, 64);
	write_file(OUTPUT, ar, 64);

	printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	MPI_Finalize();

	return 0;
}
