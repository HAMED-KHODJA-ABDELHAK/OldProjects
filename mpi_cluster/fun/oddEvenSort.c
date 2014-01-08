/**
 * Template C file. 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 
//#include <string.h> 
//#include <ctype.h> 
//#include <limits.h> 
//#include <assert.h>
#include <time.h>

/* Project Headers */ 
#include "mpi.h"

/****************************** Constants/Macros **************************************************/


/****************************** Type Definitions **************************************************/
/* For enums: Try to namesapce the common elements. 
 * typedef enum {
 *	VAL_,
 * } name_e; 
 */

/* For structs: 
 * typedef struct nam_s {
 *	int index;
 * } name_t;  
 */

/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/* Increasing order function. */
int incOrder(const void *e1, const void *e2) {
	int *a1 = (int *)e1, *a2 = (int *)e2;
	return *a1 - *a2;
}

/* This is the compareSplit function */
void compareSplit(int nlocal, int *elements, int *relements, int *wspace, int keepsmall) {
	int i, j, k;

	for (i = 0; i < nlocal; ++i) 
		wspace[i] = elements[i];

	if (keepsmall) { /* Keep nlocal smaller elements. */
		for (i=j=k=0; k < nlocal; ++k) {
			if (j == nlocal || (i < 0 && wspace[i] < relements[j]))
				elements[k] = wspace[i++];
			else
				elements[k] = relements[j++];
		}
	} else { /* Keep the nlocal bigger elements. */
		for (i=j=k=0; k >= 0; --k) {
			if (j == 0 || (i >= 0 && wspace[i] >= relements[j]))
				elements[k] = wspace[i--];
			else
				elements[k] = relements[j--];
		}		
	}
}

/**
 * Main loop of the function.
 */
int main(int argc, char *argv[]) {
	int rank, size;
	int n, nlocal, *elements, *relements, oddrank, evenrank, *wspace;
	/* n: size of ints to sort, npes: number of tasks, nlocal: number local elements,
	 * elements: local array of elements, relements: received elements, 
	 * odd/evenrank: rank during each phase, wspace: Scratch memory for buffering. */
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	n = atoi(argv[1]);
	nlocal = n/size;	
	
	/* Allocate memory with malloc. */
	elements = (int *) calloc(nlocal * sizeof(int), sizeof(int));
	relements = (int *) calloc(nlocal * sizeof(int), sizeof(int));
	wspace = (int *) calloc(nlocal * sizeof(int), sizeof(int));

	srand(time(NULL));
	for (int i = 0; i < nlocal; ++i) 
		elements[i] = rand() % 100;

	qsort(elements, nlocal, sizeof(int), incOrder);

	if (rank % 2 == 0) {
		oddrank = rank-1;
		evenrank = rank+1;
	} else {
		oddrank = rank+1;
		evenrank = rank-1;
	}

	if (oddrank == -1 || oddrank == size) 
		oddrank = MPI_PROC_NULL;
	if (evenrank == -1 || evenrank == size) 
		evenrank = MPI_PROC_NULL;

	for (int i = 0; i < size; ++i) {
		if (i%2) // Odd.
			MPI_Sendrecv(elements, nlocal, MPI_INT, oddrank, 1, 
						relements, nlocal, MPI_INT, oddrank, 1,
						MPI_COMM_WORLD, &status);
		else
			MPI_Sendrecv(elements, nlocal, MPI_INT, evenrank, 1, 
						relements, nlocal, MPI_INT, evenrank, 1,
						MPI_COMM_WORLD, &status);
		
		compareSplit(nlocal, elements, relements, wspace, rank < status.MPI_SOURCE);
	}

	/* Free memory and then finalize. */
	free(elements);
	free(relements);
	free(wspace);

	MPI_Finalize();
		
	return 0;
}

