/**
 * Master used to spawn other processes.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -spawn ./demo/spawn_master rnds workers
 * Arguments to master:
 * darts: Number of rounds across all workers.
 * workers: Number of workers to spawn.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define SLAVE 			"./demo/spawn_slave"
#define ARG_LEN 	20
#define REAL_PI 		3.14159265358979
#define DEF_DARTS		5000000
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
	int rank, size, tasks, darts, hits = 0, all_hits = 0;
	char darts_str[ARG_LEN];
	double start_init, start_spawn, pi;

	/* Standard startup. */
	MPI_Init(&argc, &argv);
	start_init = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Process args. */
	if (argc < 3)
		darts = DEF_DARTS;
	else
		darts = atoi(*++argv);
	tasks = atoi(*++argv);

	/* Each process gets darts/n work. Calculate and put in string to pass. Numbers > ARG_LEN truncated. */
	snprintf(darts_str, ARG_LEN, "%d", (int) darts/tasks);
	darts_str[ARG_LEN-1] = '\0';
	char *w_args[] = {darts_str, NULL};
	printf("Spawning %d slaves to throw %s darts each.\n", tasks, darts_str);

	/* Spawn workers then gather with reduce. */
	start_spawn = MPI_Wtime();
	MPI_Comm_spawn(SLAVE, w_args, tasks,
			MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,
			MPI_ERRCODES_IGNORE);
	printf("Time just for comm spawn is %.10f seconds.\n", MPI_Wtime() - start_spawn);

	/* Call reduce, this master contributes 0. recv_buf has total once finished. */
	MPI_Reduce(&hits, &all_hits, 1, MPI_INT, MPI_SUM, MPI_ROOT, everyone);
	pi = (4.0 * all_hits / darts);
	printf("The hit count was %d, the final value of PI is: %.40f.\n", all_hits, pi);
	printf("The percent deviation from reference: %.10f%%\n", (pi - REAL_PI)/REAL_PI *100);
	printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start_init);

	MPI_Finalize();

	return 0;
}
