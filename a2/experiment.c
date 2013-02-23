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

////returns the index of the median of medians.
////requires a variant of select, "selectIdx" which returns the index of the selected item rather than the value
//int medianOfMedians(int vals[], int left, int right) {
//     int num_medians = (right-left) / 5;
//     int sub_left, sub_right, median_index;
//
//     for (int i = 0; i < num_medians; ++i) {
//
//         // Get the median of the five-element subgroup.
//         sub_left = left + i*5;
//         sub_right = sub_left + 5;
//
//         if (sub_right > right)
//        	 sub_right = right;
//
//         median_index = selectIdx(list, subLeft, subRight, 2) //alternatively, use a faster method that works on lists of size 5
//         //move the median to a contiguous block at the beginning of the list
//         swap list[left+i] and list[medianIdx]
//     }
//     //select the median from the contiguous block
//     return selectIdx(list, left, left + numMedians, numMedians / 2)
//}

/**
 * Main loop of the function.
 */
//int main(int argc, char **argv) {
int main(void) {
	int *root, root_size = 10;
	root = (int *)malloc(root_size * 2 * sizeof(int));
	if (root == NULL)
		lib_error("MAIN: Can't allocate root array on heap.");
	memset(root, -1, root_size*sizeof(int));

	char buf[1000];
	lib_trace_array(buf, 1000, "T", root, root_size, 1);
	printf("%s", buf);

	return 0;
}

