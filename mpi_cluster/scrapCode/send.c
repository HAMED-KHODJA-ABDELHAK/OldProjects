/**
 * Template C file. 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
//#include <stdlib.h> 
//#include <string.h> 
//#include <ctype.h> 
//#include <limits.h> 
//#include <assert.h>

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
/**
 * Main loop of the function.
 */
int main(int argc, char *argv[]) {
	int rank, size, number, len;
	char name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	switch (rank) {
	case 0:
		number = -1;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		break;
	case 1:
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process %d received number %d from process %d.\n", rank, number, 0);
	}

	MPI_Get_processor_name(name, &len);
	printf("I am running on processor with name %s and rank %d.\n", name, rank);

	MPI_Finalize();

	return 0;
}

