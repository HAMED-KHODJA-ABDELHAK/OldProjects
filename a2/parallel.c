/**
 * Reference implementation of quicksort with serial implementation as baseline.
 * This program will only serially calculate the quicksort of the input file.
 * If you don't know the number of words in your input: cat input.txt | wc, second number is word count.
 * See mylib.h/.c for functions not in this file.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/parallel <work> <mode>
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
#include <string.h>

/* Project Headers */
#include "mpi.h"
#include "mylib.h"

/****************************** Constants/Macros **************************************************/


/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/

/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int rank, size, *vals = NULL, num_proc, num_total, *recv_buf;
	double start;

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Get the work amount from command for each process. */
	num_proc = atoi(*++argv);
	num_total = num_proc * size;

	/* Root only work, ensure good usage and proper input. */
	if (rank == ROOT) {
		if (argc < 3)
			m_error("MAIN: Bad usage, see top of respective c file.");

		/* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
		vals = (int *)malloc(num_total * sizeof(int));
		if (vals == NULL)
			m_error("MAIN: Can't allocate vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			gen_input(vals, num_total);
			write_file(INPUT, vals, num_total);
		}

		/* Read back input from file into array on heap. */
		read_file(INPUT, vals, num_total);
	}

	/* Malloc a workspace for local stuff. */
	recv_buf = malloc(num_proc * sizeof(int));
	if (recv_buf == NULL)
		m_error("MAIN: Can't allocate recv_buf array on heap.");

	/* Scatter, all processes same until result at master. */
	MPI_Scatter(vals, num_proc, MPI_INT, recv_buf, num_proc, MPI_INT, 0, MPI_COMM_WORLD);
	qsort(recv_buf, num_proc, sizeof(int), compare);
	MPI_Gather(recv_buf, num_proc, MPI_INT, vals, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Last step, root has result write to output the sorted array. */
	if (rank == ROOT) {
		write_file(OUTPUT, vals, num_total);

		free(vals);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	free(recv_buf);
	MPI_Finalize();

	return 0;
}
