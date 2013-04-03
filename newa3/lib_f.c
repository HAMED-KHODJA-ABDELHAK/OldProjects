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
 * Check if value has a simple square root. Val must be greater than zero.
 * If it has simple integer root, return it. Else return zero.
 */
int lib_sqrt(const unsigned int val) {
    int root = 1, root_sq = root * root;

    while (root_sq < val) {
        if ((val - root_sq) == 0)
            return root;

        root += 1;
        root_sq = root * root;
    }

    return 0;
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

/*
 * Generate an edge cost between two nodes given a probability of connection prob.
 */
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

/*
 * Dump a matrix to file in a nice 2D array format. Differs from below format.
 */
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

	fprintf(f, "\n\n");
}

/*
 * Write out a cost matrix in a condensed format.
 * Format of input.txt file is:
 * node_size
 * i	j	weight
 * i	j	weight
 * .....
 */
void lib_write_cost_matrix(const char *filename, int **c, const int size) {
	FILE *f;

	f = fopen(filename, "w");
	if (f == NULL)
		lib_error("WRITE_MATRIX: Failed to open file.");

	fprintf(f, "%d\n", size);

	for (int i = 0; i < size; ++i) {
		for (int j = 0; j < size; ++j) {
			if (c[i][j] != INF && i != j)
				fprintf(f, "%d %d %d\n", i, j, c[i][j]);
		}
	}

	fclose(f);
}

/*
 * Read in a cost matrix from our defined format.
 * Format of input.txt file is:
 * node_size
 * i	j	weight
 * i	j	weight
 * .....
 */
void lib_read_cost_matrix(const char *filename, int **c, const int size) {
	int i = 0, j = 0, w = 0;
	FILE *f;

	f = fopen(filename, "r");
	if (f == NULL)
		lib_error("READ_MATRIX: Failed to open file.");

	fscanf(f, "%d", &i);
	if (size != i)
		lib_error("READ_MATRIX: Node size of matrix and file do not agree.");

	while (feof(f) == 0 && ferror(f) == 0) {
		fscanf(f, "%d %d %d", &i, &j, &w);
		if (i < 0 || j < 0 || i > size-1 || j > size-1)
			continue;
		c[i][j] = w;
	}

	fclose(f);
}

