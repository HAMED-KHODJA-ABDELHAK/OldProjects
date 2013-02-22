/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"
#include "shared.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
void val_print(int vals[], int size) {
	for (int i = 0; i < size; ++i)
		printf("%d ", vals[i]);
	printf("\n");
}

/**
 * Main loop of the function.
 */
//int main(int argc, char **argv) {
int main(void) {
	int dimension = 2, id = 6, group, member, partner;

	lib_subgroup_info(dimension, id, &group, &member, &partner);
	printf("I am member %d of group %d and my partner in COMM_WORLD is %d.\n",
			member, group, partner);

	return 0;
}

