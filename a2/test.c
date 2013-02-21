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
#include <time.h>

/* Project Headers */
#include "mylib.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Simple swap function.
 */
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot. The size variables tell you where to read.
 */
void in_place_partition(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size) {
	int *left = vals, *right = vals + vals_size-1;
	*lt_size = *gt_size = 0;

	/* Done when left and right cross. */
	while ((right-left) > 0) {
		/* Scan until left points to larger than pivot and right points to less than pivot.  */
		while ((right-left) > 0 && *left <= pivot) {
			++left;
			++*lt_size;
		}
		while ((right-left) > 0 && *right > pivot) {
			--right;
			++*gt_size;
		}
		if ((right-left) > 0)
			swap(left, right);
	}

	printf("Exited the loop.\n");
}

void val_print(int vals[], int size) {
	for (int i = 0; i < size; ++i)
		printf("%d ", vals[i]);
	printf("\n");
}

/**
 * Main loop of the function.
 */
int main(void) {
	int id = 4;

	if (id & (1<<2))
		printf("True.");


	return 0;
}

