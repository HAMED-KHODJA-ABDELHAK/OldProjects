/**
 * Serial implementation of pi calculation, used as baseline.
 * Will serially throw the number of darts passed as argument.
 * Reports the time taken and deviation from reference PI, reference is first line of
 * 		(http://www.geom.uiuc.edu/~huberty/math5337/groupe/digits.html).
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/serial <darts>
 * Arguments to serial:
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
#define RADIUS		1.0
#define REAL_PI 	3.14159265358979
#define DEF_DARTS 	5000000
/****************************** Type Definitions **************************************************/


/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/*
 * Determines if a point is inside a circle of radius centred on origin.
 * X and Y are assumed to come in as numbers between 0 and 1 from rand.
 * If the distance from centre is less than OR equal to radius, we say it is inside.
 */
int in_circle(double x, double y, double r) {
    double cent_x = (2*r*x) - r, cent_y = (2*r*y) - r;
    double dist = (cent_x * cent_x) + (cent_y * cent_y);

    return dist < r*r;
}

/*
 * Function goes through darts rounds, each time randomly gets a pair of x,y coords that
 * are inside a 1x1 square. Checks if point is in circle, if so increments cnt.
 */
int throw_darts(unsigned int darts) {
	double x, y;
	int cnt = 0;

    for (unsigned int i = 0; i < darts; ++i) {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;

        if (in_circle(x, y, RADIUS))
            cnt++;
    }

	return cnt;
}

int main(int argc, char **argv) {
	int rank, size, darts, hits = 0;
	double start, pi;

	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL));

	/* Get workload from command argument and execute. */
	if (argc < 2)
		darts = DEF_DARTS;
	else
		darts = atoi(*++argv);
	hits = throw_darts(darts);

	/* Report final pi calculation, cleanup and report time taken. */
	pi = (4.0 * hits / darts);
	printf("The hit count was %d, the final value of PI is: %.40f.\n", hits, pi);
	printf("The percent deviation from reference: %.10f%%\n", (pi - REAL_PI)/REAL_PI *100);
	printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	MPI_Finalize();

	return 0;
}
