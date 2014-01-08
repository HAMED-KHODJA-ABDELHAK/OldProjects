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

    int nodes = 4;
    int world = 4;
    int nodes_per_block = 2;
    int ele_per_block = nodes*nodes/world;
    int *store_c, **c;

    /* Allocate cost matrices on heap, they are nodes*nodes large. */
    store_c = (int *)malloc(nodes * nodes * sizeof(int));
    if (store_c == NULL)
        lib_error("MAIN: Can't allocate storage for cost on heap.");

    /* Take contiguous 1D array and make c be a 2d pointer into it. */
    c = (int **)malloc(nodes * sizeof(int*));
    if (c == NULL)
        lib_error("MAIN: Can't allocate cost pointers array on heap.");

    for (int i = 0; i < nodes; ++i)
        c[i] = store_c+(i*nodes);

    int *send = (int *)malloc(ele_per_block*sizeof(int));

    for (int i = 0; i < nodes; ++i) {
        for (int j = 0; j < nodes; ++j) {
            c[i][j] = i*nodes+j;
            printf("%d ", c[i][j]);
        }
        printf("\n");
    }


    for (int i = 0; i < nodes_per_block; ++i)
        memcpy(send+i*nodes_per_block, c[0+i], nodes_per_block*sizeof(int));

    for (int i = 0; i < ele_per_block; ++i)
        printf("%d ", send[i]);
    printf("\n");

    free(store_c);
    free(c);
    free(send);

    return 0;
}


