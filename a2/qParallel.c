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
 * Partition one array into a less than or equal array (ltvals) and a greater than or equal array (gtvals).
 */
void partition(const int vals[], const int vals_size, const int pivot,
		int lt[], int *lt_size, int gt[], int *gt_size) {
	*lt_size = *gt_size = 0;

	for (int i = 0; i < vals_size; ++i) {
		if (vals[i] > pivot)
			gt[(*gt_size)++] = vals[i];
		else
			lt[(*lt_size)++] = vals[i];
	}
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
void in_place_partition(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size) {
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

	printf("Exited the loop.\n");
}

/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int id, size, num_proc, num_total, pivot;
	int *root_vals = NULL, *recv_buf = NULL;
	double start;

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Get the work amount from command for each process. */
	num_proc = atoi(*++argv);
	num_total = num_proc * size;

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

	/* Malloc a workspace for local stuff. */
	recv_buf = malloc(num_proc * sizeof(int));
	if (recv_buf == NULL)
		m_error("MAIN: Can't allocate recv_buf array on heap.");

	/* Scatter, all processes same until result at master. */
//	MPI_Scatter(root_vals, num_proc, MPI_INT, recv_buf, num_proc, MPI_INT, 0, MPI_COMM_WORLD);
//	qsort(recv_buf, num_proc, sizeof(int), compare);

	if (id == ROOT) {
		int ltvals_size = 0, gtvals_size = 0;

		pivot = select_pivot(root_vals, num_total);
		printf("Selected %d as the pivot.\n", pivot);

		in_place_partition(pivot, root_vals, num_total, &ltvals_size, &gtvals_size);

		write_file(OUTPUT"ltvals", root_vals, ltvals_size);
		write_file(OUTPUT"gtvals", root_vals+ltvals_size, gtvals_size);
	}

//	MPI_Gather(recv_buf, num_proc, MPI_INT, root_vals, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Last step, root has result write to output the sorted array. */
	if (id == ROOT) {
		write_file(OUTPUT, root_vals, num_total);
		free(root_vals);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	free(recv_buf);
	MPI_Finalize();

	return 0;
}
