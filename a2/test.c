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
int main(int argc, char **argv) {
	int vals_size = 49, vals[vals_size], lt_size, gt_size;
	srand(time(NULL));

	for (int i = 0; i < vals_size; ++i)
		vals[i] = rand() % 100;

	val_print(vals, vals_size);

	in_place_partition(49, vals, vals_size, &lt_size, &gt_size);

	val_print(vals, vals_size);

	printf("Last less, first greater: %d %d\n", vals[lt_size-1], vals[lt_size]);

	return 0;
}

