/**
 * This is the slave process.
 * --- It has functions to throw darts for n rounds.
 * --- It takes one argument, darts that says the number of rounds this process should go.
 * --- Result is returned with a reduce call.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define RADIUS 	0.5

/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/*
 * Determines if a point is inside a circle of radius 1 centered on origin.
 * Centers the coords passed in, assuming they are not centered.
 * If the distance from center is less than OR equal, we say it is inside.
 */
int in_circle(double x, double y, double r) {
    double cent_x = x-r, cent_y = y-r;
    double dist = (cent_x * cent_x) + (cent_y * cent_y);

    return dist <= r*r;
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

/**
 * Main loop of the worker threads.
 */
int main(int argc, char **argv) {
	int rank, size, darts, hits = 0, all_hits = 0;
	MPI_Comm parent;

	/* Init process and get some important info. */
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_get_parent(&parent);

	/* Seed the rand function, get the passed number of rounds. */
	srand(time(NULL) + rank*rank*rank);
	darts = atoi(*++argv);

	hits = throw_darts(darts);

	/* Reduce data back to master. */
	MPI_Reduce(&hits, &all_hits, 1, MPI_INT, MPI_SUM, 0, parent);

	MPI_Finalize();

	return 0;
}
