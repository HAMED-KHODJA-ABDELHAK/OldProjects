/**
 * Library functions that are common to both my serial and parallel
 * quicksort implementations. Also in general the functions that can be tested.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "array_ops.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Generic error function, prints out the error and terminates execution.
 */
void lib_error(const char * const mesg) {
    printf("ERROR: %s.\n", mesg);
    exit(1);
}

/*
 * Standard increasing comparator for qsort.
 */
int lib_compare (const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

/*
 * Function generates size random numbers between 0 and MAX_VAL. All values put in vals array.
 */
void lib_generate_numbers(int vals[], int size) {
    srand(time(NULL));

    for (int i = 0; i < size; ++i)
        vals[i] = rand() % MAX_VAL;
}

/*
 * Returns the pivot based on passed in array.
 * Currently: Median of three algorithm, select median of first, last and middle elements.
 */
int lib_select_pivot(int vals[], const int size) {
//	const int start = 0, mid = size/2, end = size-1;
//	int large, small, pivot;

    qsort(vals, size, sizeof(int), lib_compare);
    return vals[size/2];
}

/*
 * Simple swap function.
 */
void lib_swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot. Pivot value passed in, not necessarily present.
 */
void lib_partition_by_pivot_val(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size) {
    int *left = vals, *right = vals + vals_size-1;
    *lt_size = 0;

    /* Done when left and right cross makes difference negative. */
    while (left < right) {
        /* Scan until left points to larger than pivot and right points to less than pivot.  */
        while (left < right && *left <= pivot) {
            ++left;
            ++*lt_size;
        }
        while (left < right && *right > pivot) {
            --right;
        }
        if (left < right)
            lib_swap(left, right);
    }

    *gt_size = vals_size - *lt_size;
}

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot. Pivot value is at pivot_index in the array.
 */
int lib_partition_by_pivot_index(int pivot_index, int *left, int *right) {
    /* Store them starting at left and moving right. */
    int *store_l = left, *store_r = right;
    int pivot_val = left[pivot_index];

    /* Move pivot to farthest left value in array. */
    lib_swap(left+pivot_index, left);

    while (left < right) {
        /* Scan until left points to larger than pivot and right points to less than pivot.  */
        while (*left <= pivot_val && left != store_r)
            ++left;
        while (*right > pivot_val)
            --right;

        if (left < right)
            lib_swap(left, right);
    }

    /* Right points to position of pivot. */
    lib_swap(right, store_l);

    /* Return index of pivot in partitioned array. */
    return right-store_l;
}

/*
 * Select the kth largest element from the range of values in the array, where 1 would be smallest and n the largest.
 * Starts at k = 1..N.
 */
int lib_select_kth(int kth, int *left, int *right) {
    int new_pivot_index = 0, rand_index = 0, pivot_dist = -1;
    int *store_l = left;

    /* Until we get to kth, select a random index and partition. Then we get new pivot index. */
    while (1) {
        rand_index = rand() % (right-left+1);
        new_pivot_index = lib_partition_by_pivot_index(rand_index, left, right);
        pivot_dist = new_pivot_index+1;

        /* If pivot dist = kth, we are there. If > kth, we move right pointer left. Else left pointer goes right. */
        if (pivot_dist == kth)
            break;
        else if (pivot_dist > kth)
            right = left+new_pivot_index-1;
        else {
            kth -= pivot_dist;
            left += new_pivot_index+1;
        }
    }

    /* Return index of kth in original array, should be kth-1. */
    return left+new_pivot_index - store_l;
}

/*
 * Integer power function, takes log(n) steps to compute.
 */
int lib_power(const int base, const unsigned int exp) {
    int temp;

    if (exp == 0)
        return 1;

    // Odd power.
    if (exp % 2) {
        temp = lib_power(base, (exp-1)/2);
        return temp * temp * base;
    } else {
        temp = lib_power(base, exp/2);
        return temp * temp;
    }
}

/*
 * Returns true only if the id passed in is the root of the given subgroup domain.
 * The root of any given subgroup has d 0's starting from the right.
 * That basically means it is some modulo, I'll also return the subgroup and it's partner.
 */
void lib_subgroup_info(const int dimension, subgroup_info_t *info) {
    info->group_size = lib_power(2, dimension);
    info->group_num = info->world_id / info->group_size;
    info->member_num = info->world_id % info->group_size;
    info->partner = info->world_id ^ (1<<(dimension-1));
}

/*
 * Function takes two arrays of passed size and merges them into array a.
 * Assumes that a is in fact a malloced array that can be freed.
 */
void lib_array_union(int *a[], int *a_size, const int b[], const int b_size) {
    /* New array of combined size, then copy in the contents of a and b. */
    int new_size = *a_size + b_size, *temp = NULL;

    /* Don't bother if new size is 0. */
    if (new_size != 0) {
        /* Malloc the new array and copy in vals from a and b. */
        temp = malloc(new_size * sizeof(int));
        memcpy(temp, *a, (*a_size)*sizeof(int));
        memcpy(temp+(*a_size), b, b_size*sizeof(int));

        /* Free old a array. Then update pointer and size. */
        if (*a != NULL)
            free(*a);
        *a = temp;
        *a_size = new_size;
    }
}

/*
 * Once gathered, root array contains a lot of empty values as -1.
 * Compress down array to be contiguous.
 */
void lib_compress_array(int world, int root[], int root_size) {
    int offset = root_size/world;
    int *left = root, *right = root+1;

    for (int i = 0; i < world; ++i) {
        right = root + i*offset;

        while (*right != -1 && (right - root) < root_size) {
            lib_swap(left, right);
            ++left;
            ++right;
        }
    }
}

/*
 * Select the required number of medians and order them at the front of the array.
 */
int lib_select_medians(int *vals, const int left, const int right) {
    int num_medians = (right-left+1)/5;
    int sub_left = 0, sub_right = 0, median_index = 0;

    /* For each group of five elements. */
    for (int i = 0; i < num_medians; ++i) {
        sub_left = left+i*5;
        sub_right = sub_left+4;

        /* Bounds protection. */
        if(sub_right > right)
            sub_right = right;

        /* Select the third largest element, the median. Swap to front. */
        median_index = lib_select_kth(3, vals+sub_left, vals+sub_right);
        lib_swap(vals+left+i, vals+sub_left+median_index);
    }

    /* Sort the new medians. */
	qsort(vals, num_medians, sizeof(int), lib_compare);

    return num_medians;
}

/*
 * Function isn't the nicest, could be refactored to be more elegant. For now,
 * it will calculate and select the right number of pivots and put them in pivots array for any dimension from 1-3.
 */
void lib_select_pivots_from_medians(const int dimension, int *pivots, const int pivots_size, int *vals, const int vals_size) {
	int num_pivots = lib_power(2, dimension) - 1, m_index = 0;
	if (pivots_size < num_pivots)
		lib_error("SELECT_PIVOTS: Pivots array is not large enough.");

	/* Select pivots from medians based on dimension. */
	if (dimension >= 1) {
		m_index = (int) (vals_size)/2 - 1;
		pivots[0] = vals[m_index];
	}

	if (dimension >= 2) {
		m_index = (int) (vals_size)/4;
		pivots[1] = vals[m_index];

		m_index = (int) (3*vals_size)/4;
		pivots[2] = vals[m_index];
	}

	if (dimension >= 3) {
		m_index = (int) (vals_size)/8;
		pivots[3] = vals[m_index];

		m_index = (int) (3*vals_size)/8;
		pivots[4] = vals[m_index];

		m_index = (int) (5*vals_size)/8;
		pivots[5] = vals[m_index];

		m_index = (int) (7*vals_size)/8;
		pivots[6] = vals[m_index];
	}
}


/* This function groups values into blocks of five and then selects a median.
 * All such medians are collected at the front and the median of this group is selected as the true median.
 */
int lib_median_of_medians(int *vals, int left, int right) {
     int num_medians = (right+1-left)/5;
     int sub_left, sub_right, median_index;

     for (int i = 0; i < num_medians; ++i) {
         /* Get the median of the five-element subgroup. */
         sub_left = left + i*5;
         sub_right = sub_left + 4;

         /* Ensure we never leave bounds. */
         if (sub_right > right)
           sub_right = right;

         median_index = lib_select_kth(3, vals+sub_left, vals+sub_right);
         /* Move the median to front of the list. */
         lib_swap(vals+left+i, vals+sub_left+median_index);
     }

     /* Select the median from our medians at the front of list. */
     return lib_select_kth((num_medians/2)+1, vals+left, vals+num_medians-1);
}
