/**
 * Template C file. 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
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
void error(char *str) {
	printf("Error: %s.\n", str);
	exit(1);
}

/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
	int rank, size;
	MPI_Comm parent;

	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&parent);

	if (parent == MPI_COMM_NULL)
		error("No parent.");

	MPI_Comm_remote_size(parent, &size);
	if (size != 1)
		error("Something wrong with parent.");
	
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	printf("I am a worker number %d of %d.\n", rank, size);

	MPI_Finalize();
	
	return 0;
}

