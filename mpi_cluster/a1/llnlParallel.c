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

/* Project Headers */
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define DARTS 50000     /* number of throws at dartboard */
#define ROUNDS 100      /* number of times "darts" is iterated */
#define MASTER 0        /* task ID of master task */
#define REAL_PI 	3.14159265358979

#define sqr(x)	((x)*(x))

/****************************** Type Definitions **************************************************/
double dboard(int);

/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/


/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/**********************************************************************
 * FILE: mpi_pi_reduce.c
 * OTHER FILES: dboard.c
 * DESCRIPTION:
 *   MPI pi Calculation Example - C Version
 *   Collective Communication example:
 *   This program calculates pi using a "dartboard" algorithm.  See
 *   Fox et al.(1988) Solving Problems on Concurrent Processors, vol.1
 *   page 207.  All processes contribute to the calculation, with the
 *   master averaging the values for pi. This version uses mpc_reduce to
 *   collect results
 * AUTHOR: Blaise Barney. Adapted from Ros Leibensperger, Cornell Theory
 *   Center. Converted to MPI: George L. Gusciora, MHPCC (1/95)
 * LAST REVISED: 02/23/12 Blaise Barney
**********************************************************************/

int main (int argc, char *argv[])
{
	double	homepi,         /* value of pi calculated by current task */
	pisum,	        /* sum of tasks' pi values */
	pi,	        /* average of pi after "darts" is thrown */
	avepi;	        /* average pi value for all iterations */
	int	taskid,	        /* task ID - also used as seed number */
	numtasks,       /* number of tasks */
	rc,             /* return code */
	i;
	//MPI_Status status;

	/* Obtain number of tasks and task ID */
	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
	printf("MPI task %d has started...\n", taskid);

	double start_time = 0.0;
	if (taskid == MASTER) {
		start_time = MPI_Wtime();
	}

	/* Set seed for random number generator equal to task ID */
	srand(taskid);

	avepi = 0;
	for (i = 0; i < ROUNDS; i++) {
		/* All tasks calculate pi using dartboard algorithm */
		homepi = dboard(DARTS);

		/* Use MPI_Reduce to sum values of homepi across all tasks
		 * Master will store the accumulated value in pisum
		 * - homepi is the send buffer
		 * - pisum is the receive buffer (used by the receiving task only)
		 * - the size of the message is sizeof(double)
		 * - MASTER is the task that will receive the result of the reduction
		 *   operation
		 * - MPI_SUM is a pre-defined reduction function (double-precision
		 *   floating-point vector addition).  Must be declared extern.
		 * - MPI_COMM_WORLD is the group of tasks that will participate.
		 */

		rc = MPI_Reduce(&homepi, &pisum, 1, MPI_DOUBLE, MPI_SUM,
				MASTER, MPI_COMM_WORLD);
		if (rc != MPI_SUCCESS)
			printf("%d: failure on mpc_reduce\n", taskid);

		/* Master computes average for this iteration and all iterations */
		if (taskid == MASTER) {
			pi = pisum/numtasks;
			avepi = ((avepi * i) + pi)/(i + 1);
			printf("   After %8d throws, average value of pi = %10.8f\n",
					(DARTS * (i + 1)),avepi);
			printf("The percent deviation from reference of pi: %.10f%%\n", ((pi - REAL_PI)/REAL_PI) * 100);
			printf("The percent deviation from reference of avepi: %.10f%%\n", ((avepi - REAL_PI)/REAL_PI) * 100);
		}
	}
	if (taskid == MASTER) {
		printf ("\nReal value of PI: 3.1415926535897 \n");
		printf("Operation took %f seconds.\n", MPI_Wtime() - start_time);
	}

	MPI_Finalize();
	return 0;
}

/*
Explanation of constants and variables used in this function:
  darts       = number of throws at dartboard
  score       = number of darts that hit circle
  n           = index variable
  r           = random number scaled between 0 and 1
  x_coord     = x coordinate, between -1 and 1
  x_sqr       = square of x coordinate
  y_coord     = y coordinate, between -1 and 1
  y_sqr       = square of y coordinate
  pi          = computed value of pi
*/
double dboard(int darts)
{
	double x_coord, y_coord, pi, r;
	int score, n;
	unsigned int cconst;  /* must be 4-bytes in size */
	/*************************************************************************
	 * The cconst variable must be 4 bytes. We check this and bail if it is
	 * not the right size
	 ************************************************************************/
	if (sizeof(cconst) != 4) {
		printf("Wrong data size for cconst variable in dboard routine!\n");
		printf("See comments in source file. Quitting.\n");
		exit(1);
	}
	/* 2 bit shifted to MAX_RAND later used to scale random number between 0 and 1 */
	cconst = 2 << (31 - 1);
	score = 0;

	/* "throw darts at board" */
	for (n = 1; n <= darts; n++)  {
		/* generate random numbers for x and y coordinates */
		r = (double)rand()/cconst;
		x_coord = (2.0 * r) - 1.0;
		r = (double)rand()/cconst;
		y_coord = (2.0 * r) - 1.0;

		/* if dart lands in circle, increment score */
		if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
			score++;
	}

	/* calculate pi */
	pi = 4.0 * (double)score/(double)darts;
	return(pi);
}

