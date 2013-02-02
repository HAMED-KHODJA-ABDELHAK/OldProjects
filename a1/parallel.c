/**
 * Parallel implementation of pi calculation, used as baseline.
 * Will throw darts/num_workers and then calculate PI.
 * Reports the time taken and deviation from reference PI, reference is first line of
 * 		(http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html).
 *
 * Use command: bsub -I -q COMP428 -n <n> mpirun -srun ./demo/serial <darts>
 * Arguments to serial:
 * n: Number of other tasks to start at same time.
 * darts: Number of darts to throw, basically the work load.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define RADIUS 1.0
#define REAL_PI 3.14159265358979
#define MASTER 0

/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/*
 * Simple function if improperly called.
 */
void usage() {
	printf("Usage: ./demo/parallel darts\ndarts: The number of darts to throw.\n");
	exit(1);
}

/*
 * Determines if a point is inside a circle of radius centered on origin.
 * X and Y are assumed to come in as numbers between 0 and 1 from rand.
 * If the distance from center is less than OR equal to radius, we say it is inside.
 */
int in_circle(double x, double y, double r) {
    double cent_x = 2*x - r, cent_y = 2*y - r;
    double dist = (cent_x * cent_x) + (cent_y * cent_y);

    return dist < r*r;
}

/*
 * Function goes through a number of rnds, each time randomly gets a pair of x,y coords that
 * are inside a 1x1 square. Checks if point is in circle, if so increments cnt.
 */
int throw_darts(unsigned int rnds) {
	double x, y;
	int cnt = 0;

    for (unsigned int i = 0; i < rnds; ++i) {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;

        if (in_circle(x, y, RADIUS))
            cnt++;
    }

	return cnt;
}

int main (int argc, char *argv[])
{
	int rank, size, darts, darts_per_task, hits = 0, all_hits = 0;
	double start, pi;



	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL) + rank*rank*rank);

	/* Get workload from command argument and execute. */
	if (argc < 2)
		usage();
	darts = atoi(*++argv);
	darts_per_task = darts/size;
	printf("%d tasks will now throw %d darts each and then reduce to master.\n", size, darts_per_task);
	hits = throw_darts(darts_per_task);

	/* All tasks reduce to the master their hit counts. */
	MPI_Reduce(&hits, &all_hits, 1, MPI_INT, MPI_SUM, MASTER, MPI_COMM_WORLD);

	/* Report final pi calculation, cleanup and report time taken. */
	if (rank == MASTER) {
		pi = (4.0 * all_hits / darts);
		printf("The hit count was %d, the final value of PI is: %.40f.\n", hits, pi);
		printf("The percent deviation from reference: %.10f%%\n", (pi - REAL_PI)/REAL_PI *100);
		printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}
	MPI_Finalize();

	return 0;
}
