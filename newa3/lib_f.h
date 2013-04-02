#ifndef _LIB_F_H_
#define _LIB_F_H_

/********************* Header Files ***********************/
/* C Headers */

/* Project Headers */

/******************* Constants/Macros *********************/
/* Flag to pass in to generate new input.txt */
#define GENERATE_FLAG       "gen"
/* Max value for infinite value in matrix cost. */
#define INF					10000
/* Root ID */
#define ROOT                0
#define INPUT				"input.txt"
#define OUTPUT				"output.txt"

/******************* Type Declarations ********************/
/* For enums: Try to namesapce the common elements.
 * typedef enum {
 *	VAL_,
 * } name_e;
 */

/* For structs:
 * typedef struct name_s {
 *	int index;
 * } name_t;
 */

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
 * Initialise the cost matrix with correct values.
 */
void lib_init_cost(int **c, int size);

/*
 * Initialise the path matrix with correct values.
 */
void lib_init_path(int **p, int size);


/*
 * Trace a given matrix to the file.
 */
void lib_trace_matrix(FILE *f, int **a, int size);

/*
 * Return a random integer value for edge, with probability p of being connected.
 */
int lib_edge_cost(double prob);

/*
 * Generate a graph of the requested size randomly. Costs go between 1 and 10, no negatives
 * just to prevent having to deal with negative cycles that can break this aglorithm.
 */
void lib_generate_graph(int **c, int size);

#endif /* _LIB_F_H_ */

