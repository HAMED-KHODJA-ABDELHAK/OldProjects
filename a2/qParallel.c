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
#include "shared.h"

/******************* Constants/Macros *********************/
#define BUF_SIZE 1000

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/
static char buf[BUF_SIZE];

/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Implementation of the hyper quicksort for any given dimension. Topology is assumed to be entirely
 * in MPI_COMM_WORLD. Details follow traditional hypercube algorithm seen on page 422 of Parallel Computing (Gupta).
 * At the end, each processor with local_size elements in local will be ready to locally sort.
 */
void hyper_quicksort(const int dimension, const int id, int local[], int local_size,
		int recv[], int recv_size) {
	MPI_Status mpi_status;
	MPI_Request mpi_request;
	int pivot = 0, lt_size = 0, gt_size = 0, group = 0, member = 0, partner = 0;

	/* Iterate for all dimensions of cube. */
	for (int d = dimension-1; d >= 0; --d) {
		/* Determine partner the group and member number of id, and its partner. */
		lib_subgroup_info(d+1, id, &group, &member, &partner);

		/* Select and broadcast pivot. */
		if (id == ROOT) {
			pivot = lib_select_pivot(local, local_size);
			printf("PP: Pivot is: %d.\n", pivot);
		}
		MPI_Bcast(&pivot, 1, MPI_INT, ROOT, MPI_COMM_WORLD);


		/* Partition the array. */
		lib_partition_array(pivot, local, local_size, &lt_size, &gt_size);
		lib_trace_array(buf, BUF_SIZE, "PARTITIONED:", local, local_size, id);
		printf("%s", buf);
		MPI_Barrier(MPI_COMM_WORLD);

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
		lib_array_union(&local, &local_size, recv, recv_size);

		lib_trace_array(buf, BUF_SIZE, "RECV:", recv, recv_size, id);
		printf("%s", buf);
		MPI_Barrier(MPI_COMM_WORLD);

		lib_trace_array(buf, BUF_SIZE, "PARTITIONED:", local, local_size, id);
		printf("%s", buf);
		MPI_Barrier(MPI_COMM_WORLD);


		if (id == ROOT)
			printf("ROUND %d.\n", d);
		lib_trace_array(buf, BUF_SIZE, "HYPER:", local, local_size, id);
		printf("%s", buf);
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

	/* Protection from invalid use. */
	if (argc < 3)
		lib_error("MAIN: Bad usage, see top of respective c file.");

	/* Get the work amount from command for each process. */
	num_proc = atoi(*++argv);
	root_size = num_proc * world;

	/* Root only work, ensure good usage and proper input. */
	if (id == ROOT) {
		/* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
		root = (int *)malloc(root_size * sizeof(int));
		if (root == NULL)
			lib_error("MAIN: Can't allocate root_vals array on heap.");

		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			lib_generate_numbers(root, root_size);
			lib_write_file(INPUT, root, root_size);
		}

		/* Read back input from file into array on heap. */
		lib_read_file(INPUT, root, root_size);
	}

	/* Allocate a recv buf of size 3*n/p and a local size of n/p.
	 * The recv buf accounts for the fact that at most 3*n/p can be transferred if it
	 * keeps all values in round 1 and 2 then transfers. */
	recv = (int *)malloc(num_proc * 3 * sizeof(int));
	if (recv == NULL)
		lib_error("MAIN: Can't allocate recv array on heap.");

	local = (int *)malloc(num_proc * sizeof(int));
	if (local == NULL)
		lib_error("MAIN: Can't allocate local array on heap.");
	local_size = num_proc;
	recv_size = 3 * num_proc;

	/* Scatter to across processes and then do hyper quicksort algorithm. */
	MPI_Scatter(root, num_proc, MPI_INT, local, local_size, MPI_INT, 0, MPI_COMM_WORLD);
	lib_trace_array(buf, BUF_SIZE, "SCATTER:", local, local_size, id);
	printf("%s", buf);

	MPI_Barrier(MPI_COMM_WORLD);

	/* Rearrange the cube so that we have roughly sorted data. */
	hyper_quicksort(MAX_DIM, id, local, local_size, recv, recv_size);

	/* Quicksort local array and then send back to root. */
//	qsort(local, local_size, sizeof(int), lib_compare);
	MPI_Gather(local, local_size, MPI_INT, root, num_proc, MPI_INT, 0, MPI_COMM_WORLD);

	/* Last step, root has result write to output the sorted array. */
	if (id == ROOT) {
		lib_write_file(OUTPUT, root, root_size);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
		free(root);
	}

	free(local);
	free(recv);

	MPI_Finalize();

	return 0;
}
