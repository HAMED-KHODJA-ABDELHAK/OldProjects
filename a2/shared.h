#ifndef _SHARED_H_
#define _SHARED_H_

/********************* Header Files ***********************/
/* C Headers */

/* Project Headers */

/******************* Constants/Macros *********************/
/* Flag to pass in to generate new input.txt */
#define GENERATE_FLAG 	"gen"
/* Max value of randomly generated numbers */
#define MAX_VAL 		10000
/* Root ID */
#define ROOT 			0
/* Maximum dimension of the hypercube */
#define MAX_DIM 		3
/* A tag for use in the send and recv */
#define SEND_TAG 		0

/******************* Type Declarations ********************/
/* Struct contains information on a subgroup in a hypercube. */
typedef struct subgroup_info_s {
	int group_size; /* Size of subgroup. */
	int group_num; /* Number of the group, starts at 0. */
	int member_num; /* Position in the relevant subgroup, starts at 0. */
	int partner; /* Partner to exchange data with during the hypercube round. */
	int world_id; /* ID of this process in the MPI_COMM_WORLD group. */
} subgroup_info_t;

/********************** Prototypes ************************/
/*
 * Generic error function, prints out the error and terminates execution.
 */
void lib_error(const char * const mesg);

/*
 * Standard increasing comparator for qsort.
 */
int lib_compare(const void *a, const void *b);

/*
 * Function generates size random numbers between 0 and MAX_VAL. All values put in vals array.
 */
void lib_generate_numbers(int vals[], int size);

/*
 * Returns the pivot based on passed in array.
 * Currently: Median of three algorithm, select median of first, last and middle elements.
 */
int lib_select_pivot(int vals[], const int size);

/*
 * Simple swap function.
 */
void lib_swap(int *a, int *b);

/*
 * Partition the passed in array in place. All elements at the front will be less than or equal to pivot.
 * All elements at back will be strictly greater than pivot.
 */
void lib_partition_array(int pivot, int vals[], const int vals_size, int *lt_size, int *gt_size);

/*
 * Integer power function, takes log(n) steps to compute.
 */
int lib_power(const int base, const unsigned int exp);

/*
 * Returns true only if the id passed in is the root of the given subgroup domain.
 * The root of any given subgroup has d 0's starting from the right.
 * That basically means it is some modulo, I'll also return the subgroup.
 */
void lib_subgroup_info(const int dimension, subgroup_info_t *info);

/*
 * Function takes two arrays of passed size and merges them into array a.
 * Assumes that a is in fact a malloced array that can be freed.
 */
void lib_array_union(int *a[], int *a_size, const int b[], const int b_size);

/*
 * Once gathered, root array contains a lot of empty values as -1.
 * Compress down array to be contiguous.
 */
void lib_compress_array(int world, int root[], int root_size);

#endif /* _SHARED_H_ */

