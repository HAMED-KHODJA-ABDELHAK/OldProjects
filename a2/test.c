/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"
#include "mylib.h"

/******************* Constants/Macros *********************/
#define MAX_DIM			3
#define SEND_TAG		1

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
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
 * All elements at back will be strictly greater than pivot. The size variables tell you where to read.
 */
void in_place_partition(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size) {
	int *left = vals, *right = vals + vals_size-1;
	*lt_size = *gt_size = 0;

	/* Done when left and right cross. */
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

void val_print(int vals[], int size) {
	for (int i = 0; i < size; ++i)
		printf("%d ", vals[i]);
	printf("\n");
}

/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
	int vals_size = 10, vals[vals_size], recv[vals_size], id, world, recv_size;
	MPI_Request mpi_request;
	MPI_Status mpi_status;
	srand(time(NULL));

	/* Standard init for MPI, start timer after init. Get rank and size too. */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &world);

	for (int i = 0; i < vals_size; ++i)
		vals[i] = rand() % 100;

	/* Iterate for all dimensions of cube. */
	for (int d = MAX_DIM-1; d >= 0; ++d) {
		/* Determine partner that is opposite this dimension of cube. */
		int partner = id ^ (1<<d);

		/* Determine position in the cube. If below is true, I am in upper. */
		if (id & (1<<d)) {
			MPI_Isend(vals, 2, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_request);
			MPI_Recv(recv, vals_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_status);
		} else {
			MPI_Isend(vals, 3, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_request);
			MPI_Recv(recv, vals_size, MPI_INT, partner, SEND_TAG, MPI_COMM_WORLD, &mpi_status);
		}

		MPI_Get_count(&mpi_status, MPI_INT, &recv_size);

		printf("I am %d of size %d. I got %d ints from my partner %d.\nThe ints are:\n",
				id, world, recv_size, partner);
		for (int i = 0; i < recv_size; ++i) {
			printf("%d - ", recv[i]);
		}
		printf("\n");
	}

	return 0;
}

