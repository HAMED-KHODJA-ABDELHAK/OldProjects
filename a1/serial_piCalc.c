/**
 * Template C file. 
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <time.h>

/* Project Headers */ 
#include "mpi.h"

/****************************** Constants/Macros **************************************************/
#define FALSE 0
#define TRUE 1
#define RADIUS 0.5

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
int in_circle(double x, double y, double r) {
    double cent_x = x-r, cent_y = y-r;
    double dist = (cent_x * cent_x) + (cent_y * cent_y);

    return dist <= r*r;
}

int throw_darts(unsigned int rounds) {
	double x, y;
	int cnt = 0;

    for (unsigned long i = 0; i < rounds; ++i) {
        x = rand() / (float)RAND_MAX;
        y = rand() / (float)RAND_MAX;

        if (in_circle(x, y, RADIUS))
            cnt++;
    }   

	return cnt;
}

/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
	int rank, size, cnt;
	double start;

	MPI_Init(&argc, &argv);	

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	start = MPI_Wtime();
	srand(time(NULL));

	int rounds = atoi(*++argv);
	printf("Num darts is: %d.\n", rounds);
	cnt = throw_darts(rounds);

    printf("The number count was %d, the final value of PI is: %.40f.\n", cnt, (4.0 * cnt / rounds));
	printf("This program took %f seconds to complete.\n", MPI_Wtime() - start);

	MPI_Finalize();

	return 0;
}
