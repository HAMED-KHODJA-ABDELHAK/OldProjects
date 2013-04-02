/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>

/* Project Headers */
#include "lib_f.h"

/******************* Constants/Macros *********************/
#define MAX_COST			10

/******************* Type Definitions *********************/
/* For enums: Try to namesapce the common elements.
 * typedef enum {
 *	VAL_,
 * } name_e;
 */

/* For structs:
 * typedef struct nam_s {
 *	int index;
 * } name_t;
 */

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
 * Function to init cost matrices, all values except diagonal to infinity, diagonal to 0.
 */
void lib_init_cost(int **c, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (j == i) {
				c[i][j] = 0;
			} else {
				c[i][j] = INF;
			}
		}
	}
}

/*
 * Function to init path matrices, all values to infinity.
 */
void lib_init_path(int **p, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			p[i][j] = INF;
		}
	}
}

void lib_trace_matrix(FILE *f, int **a, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (j == 0)
				fprintf(f, "The row %5d: ", i);

			fprintf(f, "%5d ", a[i][j]);

			if (j == size-1)
				fprintf(f, "\n");
		}
	}
}


int lib_edge_cost(double prob) {
	double p = (double) rand()/RAND_MAX;

	if (p <= prob)
		return (rand() % MAX_COST) + 1;
	else
		return INF;
}

/*
 * Generate a graph of the requested size randomly.
 */
void lib_generate_graph(int **c, int size) {
	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (j == i) {
				c[i][j] = 0;
			} else {
				c[i][j] = lib_edge_cost(0.5);
			}
		}
	}
}
