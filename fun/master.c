/**
 * Template C file. 
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
	int rank, size, uni_size, *uni_sizep, flag;
	char worker_name[] = "worker";
	MPI_Comm everyone;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm_get_attr(MPI_COMM_WORLD, MPI_UNIVERSE_SIZE, &uni_sizep, &flag);

	if (!flag) {
		printf("This MPI does not support uni_size. How many processes total?\n");
		scanf("%d", &uni_size);
	} else {
		uni_size = *uni_sizep;
	}

	if (uni_size == 1) 
		error("No room to start workers.");
	
	MPI_Comm_spawn(worker_name, MPI_ARGV_NULL, uni_size-1,
			MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone
			MPI_ERRCODES_IGNORE);

	sleep(10);
	printf("Master woke up.\n");

	MPI_Finalize();
	
	return 0;
}

