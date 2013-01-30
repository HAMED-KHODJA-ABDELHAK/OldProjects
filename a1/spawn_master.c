/**
 * Master used to spawn other processes.
 * Use command: bsub -I -q COMP428 -n1 mpirun -spawn ./demo/master 6 from this dir.	 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>

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
	int rank, size, num_workers;
	char worker_name[] = "./demo/worker";
	MPI_Comm everyone;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	num_workers = atoi(*++argv);	

	MPI_Comm_spawn(worker_name, MPI_ARGV_NULL, num_workers,
			MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,
			MPI_ERRCODES_IGNORE);

	sleep(3);
	printf("Master woke up.\n");

	MPI_Finalize();
	
	return 0;
}

