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
#define MAX_DIM 		3
#define SEND_TAG 		0

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Returns the pivot based on passed in array.
 * Currently: Median of three algorithm, select median of first, last and middle elements.
 */
int select_pivot(const int vals[], const int size) {
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
 * Integer power function, takes log(n) steps to compute.
 */
int power(const int base, const unsigned int exp) {
	int temp;

	if (exp == 0)
		return 1;

	// Odd power.
	if (exp % 2) {
		temp = power(base, (exp-1)/2);
		return temp * temp * base;
	} else {
		temp = power(base, exp/2);
		return temp * temp;
	}
}

/*
 * Returns true only if the id passed in is the root of the given subgroup domain.
 * The root of any given subgroup has d 0's starting from the right.
 * That basically means it is some modulo, I'll also return the subgroup.
 */
int subgroup_root(const int dimension, const int id) {
	int val = power(2, dimension);

	return (id % val) == 0;
}

/*
 * Function takes two arrays of passed size and merges them into array a.
 * Assumes that a is in fact a malloced array that can be freed.
 */
void array_union(int *a[], int a_size, const int b[], const int b_size) {
	int *temp = malloc((a_size + b_size) * sizeof(int));
	memcpy(temp, *a, a_size*sizeof(int));
	memcpy(temp+a_size, b, b_size*sizeof(int));
	free(*a);
	*a = temp;
	a_size += b_size;
}

/*
 * Implementation of the hyper quicksort for any given dimension. Topology is assumed to be entirely
 * in MPI_COMM_WORLD. Details follow traditional hypercube algorithm seen on page 422 of Parallel Computing (Gupta).
 * At the end, each processor with local_size elements in local will be ready to locally sort.
 */
void hyper_quicksort(const int dimension, const int id, const int root[], const int root_size,
		int local[], int local_size, int recv[], int recv_size) {
	MPI_Status mpi_status;
	MPI_Request mpi_request;
	int pivot = 0, lt_size = 0, gt_size = 0;

	/* Iterate for all dimensions of cube. */
	for (int d = dimension-1; d >= 0; --d) {
		/* Determine partner that is opposite this dimension of cube. Use xor to flip dth bit. */
		int partner = id ^ (1<<d);

		/* Select and broadcast pivot. */
		if (id == ROOT)
			pivot = select_pivot(root, root_size);
		MPI_Bcast(&pivot, 1, MPI_INT, ROOT, MPI_COMM_WORLD);

		/* Partition the array. */
		partition(pivot, local, local_size, &lt_size, &gt_size);

		/* Determine position in the cube. If below is true, I am in upper. */
		if (id & (1<<d)) {
			MPI_Isend(local, lt_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_request);
			MPI_Recv(recv, recv_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_status);
			/* We have sent lower portion, move elements greater down. Update local_size.*/
			memmove(local, local+lt_size, gt_size*sizeof(int));
			local_size = gt_size;
		} else {
			MPI_Isend(local+lt_size, gt_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_request);
			MPI_Recv(recv, recv_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_status);
			/* We have sent upper portion of array, merely update size and ignore older elements. */
			local_size = lt_size;
		}

		/* Get the received count and call array union function to merge into local. */
		MPI_Get_count(&mpi_status, MPI_INT, &recv_size);
		array_union(&local, local_size, recv, recv_size);
	}
}

/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int id = 0, world = 0, num_proc = 0, root_size = 0, recv_size = 0, local_size = 0;
	int *root = NULL, *recv = NULL, *local = NULL;
	double start = 0.0;

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world);

	/* Get the work amount from command for each process. */
	num_proc = atoi(*++argv);
	root_size = num_proc * world;

	/* Root only work, ensure good usage and proper input. */
	if (id == ROOT) {
		if (argc < 3)
			m_error("MAIN: Bad usage, see top of respective c file.");

		/* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
		root = (int *)malloc(root_size * sizeof(int));
		if (root == NULL)
			m_error("MAIN: Can't allocate root_vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			gen_input(root, root_size);
			write_file(INPUT, root, root_size);
		}

		/* Read back input from file into array on heap. */
		read_file(INPUT, root, root_size);
	}

	/* Allocate a recv buf of size n/p and a local size of 4*n/p (max case where receives all elements in 3 exchanges). */
	recv = (int *)malloc(num_proc * sizeof(int));
	if (recv == NULL)
		m_error("MAIN: Can't allocate recv array on heap.");

	local = (int *)malloc(num_proc * 4 * sizeof(int));
	if (local == NULL)
		m_error("MAIN: Can't allocate local array on heap.");

	/* Scatter to across processes and then do hyper quicksort algorithm. */
	MPI_Scatter(root, num_proc, MPI_INT, local, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Rearrange the cube so that we have roughly sorted data. */
	hyper_quicksort(MAX_DIM, id, root, root_size, local, local_size, recv, recv_size);

	/* Quicksort local array and then send back to root. */
	qsort(local, local_size, sizeof(int), compare);
	MPI_Gather(local, local_size, MPI_INT, root, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Last step, root has result write to output the sorted array. */
	if (id == ROOT) {
		write_file(OUTPUT, root, root_size);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
		free(root);
	}

	free(local);
	free(recv);

	MPI_Finalize();

	return 0;
}
