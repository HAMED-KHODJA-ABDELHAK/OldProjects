/**
 * This is the worker process.
 * --- It has functions to throw darts for n rounds.
 * --- It takes one argument, rnds that says the number of rounds this process should go.
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
#define RADIUS 0.5

/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/*
 * Simple wrapper function, prints a message with a prefix then exits.
 */
void error(char *str) {
	printf("Error: %s.\n", str);
	exit(1);
}

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
int throw_darts(unsigned long rnds) {
	double x, y;
	int cnt = 0;

    for (unsigned long i = 0; i < rnds; ++i) {
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
	int rank, size, rounds;
	int send_buf = 0, recv_buf = 0;
	MPI_Comm parent;

	/* Init process and get some important info. */
	MPI_Init(&argc, &argv);
	MPI_Comm_get_parent(&parent);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Seed the rand function, get the passed number of rounds. */
	srand(time(NULL) + rank);
	rounds = atoi(*++argv);
	printf("Will throw %d darts.\n", rounds);

	if (parent == MPI_COMM_NULL)
		error("No parent.");

	MPI_Comm_remote_size(parent, &size);
	if (size != 1)
		error("Something wrong with parent.");

	send_buf = throw_darts(rounds);
	printf("I am a worker number %d of %d, %d darts hit.\n", rank, size, send_buf);
	
	MPI_Reduce(&send_buf, &recv_buf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	if (rank == 0)
		printf("Reduced: %d.\n", recv_buf);

	MPI_Finalize();
	
	return 0;
}

