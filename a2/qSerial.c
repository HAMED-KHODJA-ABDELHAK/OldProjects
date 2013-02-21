/**
 * Reference implementation of quicksort with serial implementation as baseline.
 * This program will only serially calculate the quicksort of the input file.
 * If you don't know the number of words in your input: cat input.txt | wc, second number is word count.
 * See mylib.h/.c for functions not in this file.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/serial <work> <mode>
 *
 * Arguments to serial:
 * work: The amount of numbers to quicksort. This should match number in input.txt if not generated.
 * mode: Flag that optionally makes master generate a new input.
 * 		-> Use "gen" to generate new input.
 * 		-> Use "read" to use existing input.txt.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project Headers */
#include "mpi.h"
#include "mylib.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int rank, size, *vals = NULL, num_vals;
	double start;

	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	if (rank == ROOT) {

		if (argc < 3)
			m_error("MAIN: Error in usage, see notes in c file.");

		/* Get the work amount from command. */
		num_vals = atoi(*++argv);

		/* Allocate it on the heap, large amount of memory likely wouldn't fit on stack. */
		vals = (int *)malloc(num_vals * sizeof(int));
		if (vals == NULL)
			m_error("MAIN: Can't allocate vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			gen_input(vals, num_vals);
			write_file(INPUT, vals, num_vals);
		}

		/* Read back input from file into array on heap. */
		read_file(INPUT, vals, num_vals);

		/* Sort and output to file. */
		qsort(vals, num_vals, sizeof(int), compare);
		write_file(OUTPUT, vals, num_vals);

		free(vals);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	MPI_Finalize();

	return 0;
}
