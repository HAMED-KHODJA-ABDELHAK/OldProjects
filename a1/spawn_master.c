/**
 * Master used to spawn other processes.
 * Use command: bsub -I -q COMP428 -n1 mpirun -spawn ./demo/master rnds workers
 * Arguments to master:
 * rnds: Number of rounds across all workers.
 * workers: Number of workers to spawn. 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 

/* Project Headers */ 
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define WORKER 		"./demo/spawn_worker"
#define RNDS_LEN 	20

/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/**
 * Main loop of the master.
 */
int main(int argc, char **argv) {
	MPI_Comm everyone;
	int rank, size, num_workers, total_rounds, send_buf = 0, recv_buf = 0; 
	char str_rounds[RNDS_LEN];
	double start;

	/* Standard startup. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Process args. */
	total_rounds = atoi(*++argv);
	num_workers = atoi(*++argv);

	/* Each process gets RNDS/n work. Calculate and put in string to pass. */
	snprintf(str_rounds, RNDS_LEN, "%d", (int) total_rounds/num_workers);
	str_rounds[RNDS_LEN-1] = '\0';
	char *w_args[] = {str_rounds, NULL};
	printf("Work for each worker is %s.\n", str_rounds); 

	/* Spawn workers then block with reduce. */
	MPI_Comm_spawn(WORKER, w_args, num_workers,
			MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,
			MPI_ERRCODES_IGNORE);

	/* Call reduce, this master contributes 0. recv_buf has total once finished. */	
	MPI_Reduce(&send_buf, &recv_buf, 1, MPI_INT, MPI_SUM, MPI_ROOT, everyone);
	printf("Master says PI is %.50f.\n", 4.0*recv_buf/total_rounds);
	printf("This program took %f seconds to complete.\n", MPI_Wtime() - start);

	MPI_Finalize();
	
	return 0;
}

