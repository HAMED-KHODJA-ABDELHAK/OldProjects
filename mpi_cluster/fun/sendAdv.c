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
	int a[10], b[10], rank, size;
	MPI_Status status;
	
	MPI_Init(&argc, &argv);
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Sendrecv(a, 10, MPI_INT, (rank+1)%size, 1,
				b, 10, MPI_INT, (rank-1+size)%size, 1, MPI_COMM_WORLD, &status);
	MPI_Finalize();
		
	return 0;
}

