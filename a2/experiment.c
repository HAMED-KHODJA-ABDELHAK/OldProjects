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
#include "array_ops.h"
#include "file_ops.h"

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

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot. Pivot will be in the middle and index will be returned.
 */
int lib_index_partition(int pivot_index, int *left, int *right) {
    int pivot_val = *(left+pivot_index);
    lib_swap(left+pivot_index, right);

    /* Store them starting at left and moving right. */
    int *store_l = left, *store_r = right;
    right--;

    while (left != right) {
        /* Scan until left points to larger than pivot and right points to less than pivot.  */
        while (left != right && *left < pivot_val)
            ++left;
        while (left != right && *right > pivot_val)
            --right;

        if (left != right)
            lib_swap(left, right);
    }

    /* Put pivot back. */
    lib_swap(right, store_r);

    /* Return index of pivot in partitioned array. */
    return right-store_l;
}

/*
 * function select(list, left, right, k)
     if left = right // If the list contains only one element
         return list[left]  // Return that element
     // select pivotIndex between left and right
     pivotNewIndex := partition(list, left, right, pivotIndex)
     pivotDist := pivotNewIndex - left + 1
     // The pivot is in its final sorted position,
     // so pivotDist reflects its 1-based position if list were sorted
     if pivotDist = k
         return list[pivotNewIndex]
     else if k < pivotDist
         return select(list, left, pivotNewIndex - 1, k)
     else
         return select(list, pivotNewIndex + 1, right, k - pivotDist)



 function select(list, left, right, k)
     loop
         // select pivotIndex between left and right
         pivotNewIndex := partition(list, left, right, pivotIndex)
         pivotDist := pivotNewIndex - left + 1
         if pivotDist = k
             return list[pivotNewIndex]
         else if k < pivotDist
             right := pivotNewIndex - 1
         else
             k := k - pivotDist
             left := pivotNewIndex + 1
 */


/*
 * Select the pos largest element from the range of values in the array.
 */
int select_index(int vals[], int left, int right, int pos) {
    int new_pivot_index, rand_index;

    while (1) {
        rand_index = rand() % (right-left +1);
        new_pivot_index = lib_index_partition(rand_index, vals+left, vals+right);
        int pivot_dist = new_pivot_index - left;

        if (pivot_dist == pos)
            return new_pivot_index;
        else if (pos < pivot_dist)
            right = left+new_pivot_index - 1;
        else {
            pos -= pivot_dist;
            left += new_pivot_index + 1;
        }
    }

    return pos;
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
//         median_index = select_index(vals, sub_left, sub_right, 2); //alternatively, use a faster method that works on lists of size 5
//         //move the median to a contiguous block at the beginning of the list
//         lib_swap(left+i, sub_left+median_index);
//     }
//     //select the median from the contiguous block
//     return select_index(vals, left, left+ num_medians, num_medians / 2);
//}

/**
 * Main loop of the function.
 */
//int main(int argc, char **argv) {
int main(void) {
    FILE *f;
    int buf_size = 200;
    char buf[buf_size];

    snprintf(buf, buf_size, LOG_FORMAT, 2);
    if ((f = fopen(buf, "w+")) == NULL)
        return -1;

    lib_log(f, "TESTING", "This is a simple line.");
    int ar_size = 10, ar[] = {2, 5, 8, 9, 1, 20, 4, 7, 16, 11};

    int rc = lib_index_partition(4, ar, ar+9);
    //int rc = select_index(ar, 0, 9, 4);
    printf("%d", ar[rc]);

    lib_trace_array(f, "TRACE", ar, ar_size);

    if (fclose(f) != 0)
        return -1;

    return 0;
}

