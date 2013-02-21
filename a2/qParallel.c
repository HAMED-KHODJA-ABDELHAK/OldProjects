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
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project Headers */
#include "mpi.h"
#include "mylib.h"

/******************* Constants/Macros *********************/
// Maximum dimension, expecting a hypercube of dimension 3.
#define MAX_DIM 3

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Returns the pivot based on passed in array.
 * Currently: Median of three algorithm, select median of first, last and middle elements.
 */
int select_pivot(int vals[], int size) {
	const int start = 0, mid = size/2, end = size-1;
	int large, small, pivot;

	/* Find the largest and smallest of first two. */
	if (vals[start] > vals[mid]) {
		large = start;
		small = mid;
	} else {
		large = mid;
		small = start;
	}

	/* If end greater than large it is median. Else determine middle of small and end. */
	if (vals[end] > vals[large]) {
		pivot = vals[large];
	} else {
		if (vals[end] > vals[small]) {
			pivot = vals[end];
		} else {
			pivot = vals[small];
		}
	}

	return pivot;
}

/*
 * Simple swap function.
 */
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot.
 */
void partition(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size) {
	int *left = vals, *right = vals + vals_size-1;
	*lt_size = *gt_size = 0;

	/* Done when left and right cross makes difference negative. */
	while ((right-left) > 0) {
		/* Scan until left points to larger than pivot and right points to less than pivot.  */
		while ((right-left) > 0 && *left <= pivot) {
			++left;
			++*lt_size;
		}
		while ((right-left) > 0 && *right > pivot) {
			--right;
			++*gt_size;
		}
		if ((right-left) > 0)
			swap(left, right);
	}
}

/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int id, world, num_proc, num_total, pivot, lt_size, gt_size, recv_size, local_size;
	int *root_vals = NULL, *recv = NULL, *local;
	double start;
	MPI_Status mpi_status;

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world);

	/* Get the work amount from command for each process. */
	num_proc = atoi(*++argv);
	num_total = num_proc * world;

	/* Root only work, ensure good usage and proper input. */
	if (id == ROOT) {
		if (argc < 3)
			m_error("MAIN: Bad usage, see top of respective c file.");

		/* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
		root_vals = (int *)malloc(num_total * sizeof(int));
		if (root_vals == NULL)
			m_error("MAIN: Can't allocate root_vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			gen_input(root_vals, num_total);
			write_file(INPUT, root_vals, num_total);
		}

		/* Read back input from file into array on heap. */
		read_file(INPUT, root_vals, num_total);
	}

	/* Allocate a recv buf of size n/p and a local size of 4*n/p (max case where receives all elements in 3 exchanges). */
	recv = (int *)malloc(num_proc * sizeof(int));
	if (recv == NULL)
		m_error("MAIN: Can't allocate recv array on heap.");

	local = (int *)malloc(num_proc * 4 * sizeof(int));
	if (local == NULL)
		m_error("MAIN: Can't allocate root_vals array on heap.");

	/* Scatter to across processes and then do hyper quicksort algorithm. */
	MPI_Scatter(root_vals, num_proc, MPI_INT, local, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Iterate for all dimensions of cube. */
	for (int d = MAX_DIM-1; d >= 0; ++d) {
		/* Determine partner that is opposite this dimension of cube. */
		int partner = id ^ (1<<d);

		/* Select and broadcast pivot. */
		if (id == ROOT) {
			pivot = select_pivot(root_vals, num_total);
		}
		MPI_Bcast(&pivot, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

		/* Partition the array. */
		partition(pivot, local, local_size, &lt_size, &gt_size);

		/* Determine position in the cube. If below is true, I am in upper. */
		if (id & (1<<d)) {
			MPI_Sendrecv(local, lt_size, MPI_INT, partner, ROOT,
						recv, num_proc, MPI_INT, partner, MPI_ANY_TAG,
						MPI_COMM_WORLD, &mpi_status);
			/* We have sent lower portion, move elements greater down. Update local_size.*/
			memmove(local, local+lt_size, gt_size*sizeof(int));
			local_size = gt_size;
		} else {
			MPI_Recv(recv, recv_size, MPI_INT, partner, MPI_ANY_TAG, MPI_COMM_WORLD, &mpi_status);
			MPI_Send(local+lt_size, gt_size, MPI_INT, partner, 0, MPI_COMM_WORLD);
			/* We have sent upper portion of array, merely update size and ignore older elements. */
			local_size = lt_size;
		}

		/* All this to make local the union of recv and local buffers. */
		MPI_Get_count(&mpi_status, MPI_INT, &recv_size);
		int *temp = malloc(local_size + recv_size);
		memcpy(temp, local, local_size*sizeof(int));
		memcpy(temp+local_size, recv, recv_size*sizeof(int));
		local_size += recv_size;
		free(local);
		local = temp;
	}

	/* Quicksort local array and then send back to root. */
	qsort(local, local_size, sizeof(int), compare);
	MPI_Gather(local, local_size, MPI_INT, root_vals, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Last step, root has result write to output the sorted array. */
	if (id == ROOT) {
		write_file(OUTPUT, root_vals, num_total);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	MPI_Finalize();

	return 0;
}
