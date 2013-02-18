/**
 * Reference implementation of quicksort with serial implementation as baseline.
 * This program will only serially calculate the quicksort of the input file.
 * If you don't know the number of words in your input: cat input.txt | wc, second number is word count.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/serial <work> <mode>
 *
 * Arguments to serial:
 * work: The amount of numbers per process, total = work * world size.
 * mode: Flag that optionally makes master generate a new input.
 * 		-> Use "gen" to generate new input.
 * 		-> Use "read" to use existing input.txt.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define INPUT "input.txt"
#define OUTPUT "output.txt"
#define ROOT 0
#define MAX_VAL 1000000
#define GEN "gen"

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
void read_file(const char *file, int **vals, const int size) {
	// Note on size, I start it at -1 to compensate for the below while loop going one extra time.
	int i;
	FILE *f;

	if ((f = fopen(file, "r")) == NULL)
		m_error("READ: Failed to open file.");

	for (i = 0; i < size; ++i)
		fscanf(f, "%d", (*vals)+i);

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

int main(int argc, char **argv) {
	int rank, size, *vals = NULL, num_vals, *recv_buf;
	double start;

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Get the work amount from command for each process. */
	num_vals = atoi(*++argv);

	/* Malloc a workspace for local stuff. */
	recv_buf = malloc(num_vals * sizeof(int));
	if (recv_buf == NULL)
		m_error("MAIN: Can't allocate wspace array on heap.");

	if (rank == ROOT) {
		if (argc < 3)
			m_error("MAIN: Bad usage, see top of respective c file.");

		/* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
		vals = (int *)malloc(num_vals * sizeof(int));
		if (vals == NULL)
			m_error("MAIN: Can't allocate vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GEN) == 0) {
			gen_input(vals, num_vals*size);
			write_file(INPUT, vals, num_vals);
		}

		/* Read back input from file into array on heap. */
		read_file(INPUT, &vals, num_vals);
	}

	/* Scatter, all processes same until result at master. */
	MPI_Scatter(vals, num_vals, MPI_INT, recv_buf, num_vals, MPI_INT, 0, MPI_COMM_WORLD);
	printf("My first digit is: %d.\n", *recv_buf);
	char buf[60];
	sprintf(buf, "%s-%d", OUTPUT, rank);
	write_file(buf, recv_buf, num_vals);

	/* Last step, root has result write to output the sorted array. */
	if (rank == ROOT) {
		write_file(OUTPUT, vals, num_vals);

		free(vals);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	free(recv_buf);
	MPI_Finalize();

	return 0;
}
