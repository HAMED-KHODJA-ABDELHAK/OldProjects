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
 * mode: Flag that optionally makes master generate a new input.
 * 		-> Use "gen" to generate new input.
 * 		-> Use "read" to use existing input.txt.
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
/*
 * Find the shortest path, path is simply used to trace back the shortest path.
 */
void serial_shortest(int **c, int **p, int size);

/*
 * Get the path from node i to j (zero index) with distance and path matrices.
 * You need to put i and j around what is returned from this function.
 */
//void make_path(const int i, const int j, const Matrix& dist, const Matrix& p);


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Main execution body.
 */
int main(int argc, char **argv) {
	int rank = 0, size = 0, nodes = 0;
	int *store_c = NULL, *store_p = NULL, **c = NULL, **p = NULL;
	double start = 0.0;
	FILE *log;

	/* Standard init for MPI, start timer after init. */
	MPI_Init(&argc, &argv);
	start = MPI_Wtime();
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	srand(time(NULL));
	log = fopen(OUTPUT, "w");

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

	/* At this point, c and p are nxn matrices put on heap and freed later. Now initialize values. */
	lib_init_cost(c, nodes);
	lib_init_path(p, nodes);

	if (rank == ROOT) {
		/* If requested, generate new input file. */
		if (strcmp(*++argv, GENERATE_FLAG) == 0) {
			lib_generate_graph(c, nodes);
			lib_trace_matrix(log, c, nodes);
			fprintf(log, "\n\n");
			lib_trace_matrix(log, p, nodes);
			fprintf(log, "\n\n");
//			lib_write_file(INPUT, vals, vals_size);
		}

		serial_shortest(c, p, nodes);
		lib_trace_matrix(log, c, nodes);
		fprintf(log, "\n\n");
		lib_trace_matrix(log, p, nodes);
//
//		/* Read back input from file into array on heap. */
//		lib_read_file(INPUT, vals, vals_size);
//
//		/* Sort and output to file. */
//		lib_write_file(OUTPUT, vals, vals_size);
		fprintf(log, "Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
	}

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
//		printf("Cost matrix at round %d.\n", k);
//		lib_trace_matrix(stdout, c, size);
	}
}

/*
 * Get the path from node i to j (zero index) with distance and path matrices.
 * You need to put i and j around what is returned from this function.
 */
void make_path(const int i, const int j, const int ** const c, const int ** const p, const int nodes) {
	if (c[i][j] == INF)
		return string("NO PATH.");

	int mid = p[i][j];

	if (mid == INF) {
		return string(" ");
	}

//	std::stringstream line;
//	line << make_path(i, mid, dist, p) << mid << make_path(mid, j, dist, p);
//
//	return line.str();
}
