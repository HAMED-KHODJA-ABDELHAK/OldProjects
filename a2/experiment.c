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
#include "CUnit/Basic.h"


/******************* Constants/Macros *********************/
#define VALS_SIZE        20

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/
static int vals_orig[] = {62, 58, 41, 85, 39, 10, 64, 69, 41, 5, 98, 27, 2, 97, 30, 22, 39, 94, 56, 21};
static int vals[VALS_SIZE];

/****************** Test Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot. Pivot will be in the middle and index will be returned.
 */
int lib_index_partition(int pivot_index, int *left, int *right) {
    int pivot_val = left[pivot_index];
    lib_swap(left+pivot_index, left);

    /* Store them starting at left and moving right. */
    int *store_l = left;
    left++;

    while (left != right) {
        /* Scan until left points to larger than pivot and right points to less than pivot.  */
        while (left != right && *left <= pivot_val)
            ++left;
        while (left != right && *right > pivot_val)
            --right;

        lib_swap(left, right);
    }

    /* Put pivot back, right is always one past where pivot should be. */
    right--;
    lib_swap(right, store_l);

    /* Return index of pivot in partitioned array. */
    return right-store_l;
}

/*
 * Select the pos largest element from the range of values in the array.
 */
int lib_select_index(int *left, int *right, int pos) {
    int new_pivot_index = 0, rand_index = 0, *store_l = left;

    while (1) {
        rand_index = rand() % (right-left+1);
        lib_trace_array(stdout, "START", vals, VALS_SIZE);
        new_pivot_index = lib_index_partition(rand_index, left, right);
        int pivot_dist = new_pivot_index;

        if (pivot_dist == pos)
            return new_pivot_index;
        else if (pos < pivot_dist)
            right = left+new_pivot_index - 1;
        else {
            pos -= pivot_dist + 1;
            left += new_pivot_index + 1;
        }
    }

    return left+pos-store_l;
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
//           sub_right = right;
//
//         median_index = select_index(vals, sub_left, sub_right, 2); //alternatively, use a faster method that works on lists of size 5
//         //move the median to a contiguous block at the beginning of the list
//         lib_swap(left+i, sub_left+median_index);
//     }
//     //select the median from the contiguous block
//     return select_index(vals, left, left+ num_medians, num_medians / 2);
//}

/*
 * Suite initialization function run before each test.
 */
int suite_init(void) {
    srand(time(NULL));

    return 0;
}

/* The suite cleanup function.
 */
int suite_clean(void) {

    return 0;
}

/*
 * Test the partitioning of the array when pivot is first element.
 */
void test_partition_first(void) {
    /* Expected position and values after partitioning. */
    int pos = 0, e_pos = 13, e_vals[] = {30, 58, 41, 21, 39, 10, 56, 39, 41, 5, 22, 27, 2, 62, 97, 98, 69, 94, 64, 85};
    memcpy(vals, vals_orig, VALS_SIZE*sizeof(int));

    pos = lib_index_partition(0, vals, vals+VALS_SIZE-1);

    CU_ASSERT(pos == e_pos);
    for (int i = 0; i < VALS_SIZE; ++i)
        CU_ASSERT(vals[i] == e_vals[i]);
}

/*
 * Test the partitioning of the array when pivot is last element.
 */
void test_partition_last(void) {
    /* Expected position and values after partitioning. */
    int pos = 0, e_pos = 3, e_vals[] = {10, 2, 5, 21, 39, 85, 64, 69, 41, 41, 98, 27, 58, 97, 30, 22, 39, 94, 56, 62};
    memcpy(vals, vals_orig, VALS_SIZE*sizeof(int));

    pos = lib_index_partition(VALS_SIZE-1, vals, vals+VALS_SIZE-1);

    CU_ASSERT(pos == e_pos);
    for (int i = 0; i < VALS_SIZE; ++i)
        CU_ASSERT(vals[i] == e_vals[i]);
}

/*
 * Test the partitioning of the array when pivot is the middle-ish element.
 */
void test_partition_mid(void) {
    /* Expected position and values after partitioning. */
    int pos = 0, e_pos = 10, e_vals[] = {27, 21, 41, 39, 39, 10, 22, 30, 2, 5, 41, 98, 62, 97, 69, 64, 85, 94, 56, 58};
    memcpy(vals, vals_orig, VALS_SIZE*sizeof(int));

    pos = lib_index_partition(8, vals, vals+VALS_SIZE-1);

    CU_ASSERT(pos == e_pos);
    for (int i = 0; i < VALS_SIZE; ++i)
        CU_ASSERT(vals[i] == e_vals[i]);
}

/*
 * Test the partitioning of the array when pivot is first element.
 */
void test_select_index(void) {
    /* Expected position and values after partitioning. */
    int pos = 0, e_pos = 10, e_val = 39;
    memcpy(vals, vals_orig, VALS_SIZE*sizeof(int));

    /* Get seventh smallest value. */
    pos = lib_select_index(vals, vals+VALS_SIZE-1, 7);

    CU_ASSERT(vals[pos] == e_val);
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
   CU_pSuite sharedSuite = NULL;

   /* Initialize the CUnit test registry. */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* Add a suite to the registry. */
   sharedSuite = CU_add_suite("Shared Suite", suite_init, suite_clean);
   if (NULL == sharedSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Add the tests to the suite. */
   if (
       (NULL == CU_add_test(sharedSuite, "Index Partition: First.......", test_partition_first)) ||
       (NULL == CU_add_test(sharedSuite, "Index Partition: Mid.........", test_partition_mid)) ||
       (NULL == CU_add_test(sharedSuite, "Index Partition: Last........", test_partition_last)) ||
       (NULL == CU_add_test(sharedSuite, "Index Partition: Last........", test_select_index))
      )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();
}
