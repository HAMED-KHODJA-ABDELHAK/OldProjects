/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>

/* Project Headers */
#include "lib_f.h"
#include "mpi.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/
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

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/**
 * Main loop of the function.
 */
int main(int argc, char **argv) {
    double start = 0.0;
    int rank = 0, world = 0;

     /* Standard init for MPI, start timer after init. */
    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

    if(world != 4)
        lib_error("MAIN: Fail.");

    int num_rows = lib_sqrt(world), list[num_rows];
    /* Row comm at the front, col comm at the back. */
    MPI_Comm comm[num_rows*2];
    MPI_Group group;

    for (int i = 0; i < num_rows; ++i) {
        lib_get_group_list(num_rows, i, 1, list);
        for (int i = 0; i < num_rows; ++i)
            printf("%d ", list[i]);
        printf("End round %d\n", i);
    }

    MPI_Finalize();

    return 0;
}

