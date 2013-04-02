/**
 * Reference implementation of floyd's algorithm serial implementation as baseline.
 * This program will only serially calculate the quicksort of the input file.
 * If you don't know the number of words in your input: cat input.txt | wc, second number is word count.
 * See mylib.h/.c for functions not in this file.
 *
 * Use command: bsub -I -q COMP428 -n1 mpirun -srun ./demo/serial <nodes> <mode>
 *
 * Arguments to serial:
 * nodes: The amount of nodes in a graph.
 * mode: Flag that makes master generate a new input.
 * 		-> Use "gen" to generate new input. OVERWRITES existing input file.
 * 		-> Use anything else to use existing input.txt.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "mpi.h"
#include "lib_f.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/

void serial_shortest(int **c, int **p, int size);

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int rank = 0, size = 0, nodes = 0, buf_size = 2000;
	int *store_c = NULL, *store_p = NULL, **c = NULL, **p = NULL;
	double start = 0.0;
	char buf[buf_size];
	FILE *log;

	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	/* Init rand, open output log and memset buffer for path. */
	srand(time(NULL));
	log = fopen(OUTPUT, "w");
	memset(buf, '\0', buf_size*sizeof(char));

	if (rank == ROOT) {
		if (argc < 3)
			lib_error("MAIN: Error in usage, see notes in c file.");
	}

	/* Get the number of nodes to use from command. */
	nodes = atoi(*++argv);

	/* Allocate cost matrices on heap, they are nodes*nodes large. */
	store_c = (int *)malloc(nodes * nodes * sizeof(int));
	if (store_c == NULL)
		lib_error("MAIN: Can't allocate storage for cost on heap.");

	/* Take contiguous 1D array and make it 2D. */
	c = (int **)malloc(nodes * sizeof(int*));
	if (c == NULL)
		lib_error("MAIN: Can't allocate cost pointers array on heap.");

	for (int i = 0; i < nodes; ++i)
		c[i] = store_c+(i*nodes);

	/* Allocate path same as cost above. */
	store_p = (int *)malloc(nodes * nodes * sizeof(int));
	if (store_p == NULL)
		lib_error("MAIN: Can't allocate storage for path on heap.");

	p = (int **)malloc(nodes * sizeof(int*));
	if (p == NULL)
		lib_error("MAIN: Can't allocate path pointers array on heap.");

	for (int i = 0; i < nodes; ++i)
		p[i] = store_p+(i*nodes);

	/* At this point, c and p are nxn matrices put on heap and freed later. Now initialise values. */
	lib_init_cost(c, nodes);
	lib_init_path(p, nodes);

	if (rank == ROOT) {
		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			lib_generate_graph(c, nodes);
			lib_write_cost_matrix(INPUT, c, nodes);
		}

		/* Read back input from file into array on heap. */
		lib_read_cost_matrix(INPUT, c, nodes);
		fprintf(log, "Initial arrays cost and path.\nCost:\n");
		lib_trace_matrix(log, c, nodes);
		fprintf(log, "Path:\n");
		lib_trace_matrix(log, p, nodes);

		serial_shortest(c, p, nodes);
		fprintf(log, "After determining the shortest path.\nCost:\n");
		lib_trace_matrix(log, c, nodes);
		fprintf(log, "Path:\n");
		lib_trace_matrix(log, p, nodes);

		/* Dump final cost and path matrix to anaylze later. */
		lib_write_cost_matrix(COST_FILE, c, nodes);
		lib_write_cost_matrix(PATH_FILE, p, nodes);

		fprintf(log, "Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

	/* Clean up the heap allocation. */
	free(c);
	free(p);
	free(store_c);
	free(store_p);
	fclose(log);
	MPI_Finalize();

	return 0;
}

/*
 * Find the shortest path, path is simply used to trace back the shortest path.
 * This is the simplest form of the algorithm. K rounds of using k at every point in the matrix.
 */
void serial_shortest(int **c, int **p, int size) {
	for (int k = 0; k < size; ++k) {
		for (int i = 0; i < size; ++i) {
			for (int j = 0; j < size; ++j) {
				int new_dist = c[i][k] + c[k][j];
				if (new_dist < c[i][j]) {
					c[i][j] = new_dist;
					p[i][j] = k;
				}
			}
		}
	}
}
