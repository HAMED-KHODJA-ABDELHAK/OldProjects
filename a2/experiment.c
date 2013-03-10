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

/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Test Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/

/*
 * Counts the number of integers in a given file. File is formatted as a csv.
 */
int lib_count_integers(const char *filename) {
    int count = 0;
    char buf[50];
    FILE *f;

    if ((f = fopen(filename, "r")) == NULL)
        lib_error("COUNT: Failed to open file.");

    while (fscanf(f, "%s,", buf) && ferror(f) == 0 && feof(f) == 0)
        count++;

    if (fclose(f) != 0)
        lib_error("READ: Failed to close properly.");

    return count;
}

/* The main() function for setting up and running the tests.
 * Returns a CUE_SUCCESS on successful running, another
 * CUnit error code on failure.
 */
int main() {
    printf("%d\n", lib_count_integers("temp.txt"));
}
