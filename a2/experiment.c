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
	int buf_size = 200, id = 2, world = 4, ar_size = 5, ar[] = {20, 44, 1, 99, 7};
	char buf[buf_size], tag[] = "TEST";

	lib_trace_array(buf, buf_size, tag, ar, ar_size, id, world);
	printf("Expected: %s\n", buf);

	return 0;
}

