/**
 * Master used to spawn other processes. Calculates the work by each slave and then puts
 * it into a string argument. Spawns slaves and then waits for reduction. Then reports total
 * time take, pi, and deviation.
 * SPECIAL NOTE: This command should be executed from this directory, with the below relative command.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -spawn ./demo/spawn_master <workers> <darts>
 *
 * Arguments to master:
 * workers: Number of workers to spawn.
 * darts: Number of rounds across all workers. If not provided, uses DEF_DARTS.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define DEF_DARTS		5000000
#define SLAVE 			"./demo/spawn_slave"
#define ARG_LEN 		20
#define REAL_PI 		3.14159265358979

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
	double start_init, start_spawn, end_spawn, pi;

	/* Standard startup. */
	MPI_Init(&argc, &argv);
	start_init = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Process args. */
	if (argc < 2) {
		printf("You MUST tell the master how many slaves to spawn. Try again.\n");
		MPI_Finalize();
		exit(1);
	}
	tasks = atoi(*++argv);
	if (argc < 3)
		darts = DEF_DARTS;
	else
		darts = atoi(*++argv);

	/* Each process gets darts/n work. Calculate and put in string to pass as arg.
	 * Numbers > ARG_LEN truncated. */
	snprintf(darts_str, ARG_LEN, "%d", (int) darts/tasks);
	darts_str[ARG_LEN-1] = '\0';
	char *w_args[] = {darts_str, NULL};
	printf("Spawning %d slaves to throw %s darts each.\n", tasks, darts_str);

	/* Spawn workers then gather with reduce. */
	start_spawn = MPI_Wtime();
	MPI_Comm_spawn(SLAVE, w_args, tasks,
			MPI_INFO_NULL, 0, MPI_COMM_SELF, &everyone,
			MPI_ERRCODES_IGNORE);
	end_spawn = MPI_Wtime();

	/* Call reduce, this master contributes 0. recv_buf has total once finished. */
	MPI_Reduce(&hits, &all_hits, 1, MPI_INT, MPI_SUM, MPI_ROOT, everyone);
	pi = (4.0 * all_hits / darts);
	printf("The hit count was %d/%d, the final value of PI is: %.40f.\n", all_hits, darts, pi);
	printf("The percent deviation from reference: %.10f%%\n", ((pi - REAL_PI)/REAL_PI) * 100);
	printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start_init);
	printf("Time just for comm spawn is %.10f seconds.\n", end_spawn - start_spawn);

	MPI_Finalize();

	return 0;
}
