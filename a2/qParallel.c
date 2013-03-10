/**
 * This program is the hyper quicksort parallel implementation.
 * This program implements the algorithm as described in the book.
 * There are many library functions used not in this file, see array_ops.c and file_ops.c for implementations.
 *
 * Use command: bsub -I -q COMP428 -n <tasks> mpirun -srun ./demo/qParallel <numbers> <mode>
 *
 * Arguments:
 * tasks: The amount of number of processes to start. Only 2, 4 and 8 are acceptable.
 * numbers: Amount of integers to put in generated file. Only needed if mode set to 'gen'.
 * mode: Flag that optionally makes master generate a new input.
 *      -> Use "gen" to generate new input.
 *      -> To read from input.txt, simply omit 'mode' and 'numbers'.
 *
 * Input format:
 * I accept any file that is formatted so every integer is separated by a comma. Any amount of whitespace
 * between comma and next integer is allowable. Example: 10, 20,\n30,    50,
 *
 * Logging:
 * I have created a rudimentary logging framework. It traces throughout execution the state of the arrays.
 * It is disabled by default, remove the comment on QDEBUG line and recompile to enable.
 *
 * TODO:
 *  -> Resolve outstanding scaling issue. Memory limited?
 *  -> Resolve CUnit issue on cirrus.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project Headers */
#include "mpi.h"
#include "array_ops.h"
#include "file_ops.h"

/******************* Constants/Macros *********************/
#define BUF_SIZE 			1000000
#define GATHER_SCALE 		2
#define QDEBUG 				1 // Enable this line for tracing code.
#define LOG_SIZE			100
/* Maximum dimension of the hypercube */
#define MAX_DIM 			3
/* A tag for use in the send and recv */
#define PIVOT_TAG			0
#define EXCHANGE_TAG 		1
#define MAX_PIVOTS			8

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/
/* Log file per process, buf used for anything not an array trace. */
static FILE *log;
static char log_buf[LOG_SIZE];

/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * I only allow program to run if size is of a hypercube with dimension 1, 2 or 3.
 * If not right size, return 0 and fail. Else return the dimension.
 */
int determine_dimension(const int world_size) {
    int dimension = 0;

    for (int d = 1; d <= MAX_DIM; ++d) {
        if (world_size == lib_power(2, d))
            dimension = d;
    }

    return dimension;
}

/*
 * Simple wrapper, acts as a multicast but only sends to members of a given subgroup.
 * Only ever called by root of a group, send to all ids above sender below group size.
 */
void send_pivot(int pivot, const subgroup_info_t * const info) {
    MPI_Request mpi_request;

    for (int i = 1; i < info->group_size; ++i)
        MPI_Isend(&pivot, 1, MPI_INT, info->world_id+i, PIVOT_TAG, MPI_COMM_WORLD, &mpi_request);
}

/*
 * Implementation of the hyper quicksort for any given dimension. Topology is assumed to be entirely
 * in MPI_COMM_WORLD. Details follow traditional hypercube algorithm seen on page 422 of Parallel Computing (Gupta).
 * At the end, each processor with local_size elements in local will be ready to locally sort.
 */
void hyper_quicksort(const int dimension, const int id, int *local[], int *local_size,
        int recv[], const int recv_size) {
    MPI_Status mpi_status;
    MPI_Request mpi_request;
    subgroup_info_t info = {0, 0, 0, 0, id}; /* Init struct to zero, except for id of caller. */
    int pivot = 0, lt_size = 0, gt_size = 0, received = 0;

    if (dimension < 1)
        lib_error("HYPER: Dimension can't be less than 1.");

    /* Iterate for all dimensions of cube. */
    for (int d = dimension-1; d >= 0; --d) {
        /* Determine the group and member number of id, and its partner. */
        lib_subgroup_info(d+1, &info);

#ifdef QDEBUG
        snprintf(log_buf, LOG_SIZE, "INFO: World_id, Group, mem, partner. %d %d %d %d.\n",
        		info.world_id, info.group_num, info.member_num, info.partner);
        lib_log(log, "INFO", log_buf);
#endif

        /* Select and broadcast pivot only to subgroup. */
        if (info.member_num == 0) {
            pivot = lib_select_pivot(*local, *local_size);
#ifdef QDEBUG
            snprintf(log_buf, LOG_SIZE, "ROUND: %d, GROUP: %d, pivot is: %d.\n", dimension-d, info.group_num, pivot);
            lib_log(log, "PIVOT", log_buf);
#endif
            send_pivot(pivot, &info);
        } else {
            MPI_Recv(&pivot, 1, MPI_INT, MPI_ANY_SOURCE, PIVOT_TAG, MPI_COMM_WORLD, &mpi_status);
        }

        /* Partition the array. */
        lib_partition_by_pivot_val(pivot, *local, *local_size, &lt_size, &gt_size);

#ifdef QDEBUG
        lib_trace_array(log, "PARTITIONED", *local, *local_size);
#endif

        /* Determine position in the cube. If below is true, I am in upper part of this dimension. */
        if (id & (1<<d)) {
            MPI_Isend(*local, lt_size, MPI_INT, info.partner, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_request);
            MPI_Recv(recv, recv_size, MPI_INT, info.partner, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_status);
            /* We have sent lower portion, move elements greater down. Update local_size.*/
            memmove(*local, *local+lt_size, gt_size*sizeof(int));
            *local_size = gt_size;
        } else {
            MPI_Isend(*local+lt_size, gt_size, MPI_INT, info.partner, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_request);
            MPI_Recv(recv, recv_size, MPI_INT, info.partner, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_status);
            /* We have sent upper portion of array, merely update size and ignore older elements. */
            *local_size = lt_size;
        }

        /* Get the received count and call array union function to merge into local. */
        MPI_Get_count(&mpi_status, MPI_INT, &received);
        lib_array_union(local, local_size, recv, received);

        /* Ensure all partner exchanges complete before proceeding to the next round. */
#ifdef QDEBUG
        lib_trace_array(log, "RECV", recv, received);
        lib_trace_array(log, "UNION", *local, *local_size);
#endif
    }
}

/*
 * Main execution body.
 */
int main(int argc, char **argv) {
    int id = 0, world = 0, num_per_proc = 0, root_size = 0, recv_size = 0, local_size = 0, dimension = 0;
    int *root = NULL, *recv = NULL, *local = NULL;
    char file[FILE_SIZE];
    double start = 0.0;

    /* Standard init for MPI, start timer after init. Get rank and size too. */
    MPI_Init(&argc, &argv);
    start = MPI_Wtime();
    MPI_Comm_rank(MPI_COMM_WORLD, &id);
    MPI_Comm_size(MPI_COMM_WORLD, &world);

#ifdef QDEBUG
    /* Open log file, overwrite on each open. */
    snprintf(file, FILE_SIZE, LOG_FORMAT, id);
    if ((log = fopen(file, "w")) == NULL)
        lib_error("MAIN: Could not open log file.");
#endif

    /* Determine the dimension of the cube. */
    dimension = determine_dimension(world);
    if (dimension == 0)
        lib_error("MAIN: This hypercube program only supports running with 2, 4 or 8 processors.");

    /* Get the work amount from command for each process. */
    num_per_proc = atoi(*++argv);
    root_size = num_per_proc * world;

    /* Root only work, ensure good usage and proper input. */
    if (id == ROOT) {
        /* Allocate the whole array on the heap, large amount of memory likely wouldn't fit on stack. */
        root = (int *)malloc(root_size * sizeof(int));
        if (root == NULL)
            lib_error("MAIN: Can't allocate root_vals array on heap.");

        /* If requested, generate new input file. */
        if (argc == 3 && strcmp(*++argv, GENERATE_FLAG) == 0) {
            lib_generate_numbers(root, root_size);
            lib_write_file(INPUT, root, root_size);
        }

        /* Read back input from file into array on heap. */
        lib_read_file(INPUT, root, root_size);
    }

    /*
     * Allocate a recv buffer with a bit of extra padding, accounts for small deviations in distribution.
     * Local will be reallocated based on need, start as num_per_proc.
     */
    recv_size = num_per_proc * GATHER_SCALE;
    local_size = num_per_proc;

    recv = (int *)malloc(recv_size * sizeof(int));
    if (recv == NULL)
        lib_error("MAIN: Can't allocate recv array on heap.");

    local = (int *)malloc(local_size * sizeof(int));
    if (local == NULL)
        lib_error("MAIN: Can't allocate local array on heap.");

    /* Scatter across the processes and then do hyper quicksort algorithm. Also, bcast the pivots. */
    MPI_Scatter(root, num_per_proc, MPI_INT, local, local_size, MPI_INT, ROOT, MPI_COMM_WORLD);
#ifdef QDEBUG
    lib_trace_array(log, "SCATTER", local, local_size);
#endif

    /* Rearrange the cube so that each processor has data strictly less than one with higher number.*/
    hyper_quicksort(dimension, id, &local, &local_size, recv, recv_size);

#ifdef QDEBUG
    lib_trace_array(log, "HYPER", local, local_size);
#endif

//    /*
//     * Reallocated root to be rescaled, mpi_gather doesn't know how many per process anymore.
//     * Set values to -1 and assume it won't be worse than the scaling factor of about 20%.
//     */
//    if (id == ROOT) {
//        free(root);
//        root_size *= GATHER_SCALE;
//        root = (int *)malloc(root_size * sizeof(int));
//        if (root == NULL)
//            lib_error("MAIN: Can't allocate root array on heap.");
//        memset(root, -1, root_size * sizeof(int));
//    }

    /* Quicksort local array and then send back to root. */
    qsort(local, local_size, sizeof(int), lib_compare);

    /* Gather each separately. */
	MPI_Request mpi_request;
	MPI_Status mpi_status;

    if (id == ROOT) {
    	int temp = 0;
    	memcpy(root, local, local_size*sizeof(int)); // Copy local into root, then get others.
    	recv_size = local_size;

    	for (int i = 1; i < world; ++i) {
    		MPI_Recv(root+recv_size, root_size-recv_size, MPI_INT, i, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_status);
    		MPI_Get_count(&mpi_status, MPI_INT, &temp);
    		recv_size += temp - 1;
    	}
    } else {
    	MPI_Isend(local, local_size, MPI_INT, ROOT, EXCHANGE_TAG, MPI_COMM_WORLD, &mpi_request);
    }
//    MPI_Gather(local, local_size, MPI_INT, root, GATHER_SCALE*num_per_proc, MPI_INT, ROOT, MPI_COMM_WORLD);

    /* Last step, root has to compress array due to uneven nature after gather. Then write to file. */
    if (id == ROOT) {
//        lib_compress_array(world, root, root_size);

#ifdef QDEBUG
        lib_trace_array(log, "GATHER", root, root_size);
#endif

        lib_write_file(OUTPUT, root, root_size);
        printf("Time elapsed from MPI_Init to MPI_Finalize is %.10f seconds.\n", MPI_Wtime() - start);
        free(root);
    }

    free(recv);
    /* May have been entirely deallocated if has no more at process. */
    if (local != NULL)
        free(local);

#ifdef QDEBUG
    fclose(log);
#endif

    MPI_Finalize();

    return 0;
}
