/**
 * File operations for input and output are defined and exported via header file.
 * Namely, I define functions for both writing and reading the expected input/output format file.
 * It is a CSV file with each value on a single line followed by a command and a new line. If your input file differs,
 * modify it or generate a new one with my program.
 *
 * I've also defined a simple logging routine that opens a file per task and writes tracing information to it.
 * This logging is disabled via define flag at compile time.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Project Headers */
#include "array_ops.h"
#include "file_ops.h"

/******************* Constants/Macros *********************/


/******************* Type Definitions *********************/


/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/


/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/*
 * Counts the number of integers in a given file. File is formatted as a csv with a comma after every integer.
 */
int lib_count_integers(const char *filename) {
    int count = 0, temp = 0;
    FILE *f;

    if ((f = fopen(filename, "r")) == NULL)
        lib_error("COUNT: Failed to open file.");

    while (fscanf(f, "%d,", &temp) && ferror(f) == 0 && feof(f) == 0)
        count++;

    if (fclose(f) != 0)
        lib_error("READ: Failed to close properly.");

    return count;
}

/*
 * Opens the filename passed in and reads size numbers into the vals array.
 */
void lib_read_file(const char *filename, int *vals, const int size) {
    // Note on size, I start it at -1 to compensate for the below while loop going one extra time.
    FILE *f;

    if ((f = fopen(filename, "r")) == NULL)
        lib_error("READ: Failed to open file.");

    for (int i = 0; i < size; ++i)
        fscanf(f, "%d,", vals+i);

    if (fclose(f) != 0)
        lib_error("READ: Failed to close properly.");
}

/*
 * Function takes an array of values and prints them to a file.
 * Each integer on a separate line.
 */
void lib_write_file(const char *filename, const int *vals, const int size) {
    FILE *f;

    if ((f = fopen(filename, "w")) == NULL)
        lib_error("WRITE: Failed to open file.");

    for (int i = 0; i < size; ++i)
        fprintf(f, "%d,\n", vals[i]);

    if (fclose(f) != 0)
        lib_error("WRITE: Failed to close properly.");
}

/*
 * Simple logging function to log the status of buffers and other variables.
 * Each process has log for itself called log.rank.txt.
 * Format of log is: (date) tag: mesg
 */
void lib_log(FILE *f, const char *tag, const char *mesg) {
    time_t rawtime;
    struct tm *timeinfo;
    char date[30];

    /* Get current time and format string into date. */
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date, 30, "(%T)", timeinfo);

    fprintf(f, "%s %s: %s", date, tag, mesg);
    fflush(f);
}

/*
 * Tracing function, takes a file and traces the values in the array.
 */
void lib_trace_array(FILE *f, char *tag, int array[], int size) {
    int count = 0, buf_size = 200;
    char buf[buf_size];
    memset(buf, '\0', buf_size*sizeof(char));

    /* Using the standard logging function to store info. */
    lib_log(f, tag, "Tracing an array, numbers are:\n");

    /* Print blocks of 20 numbers into it and flush. */
    for (int i = 0; i < size; ++i) {
        count += snprintf(buf+count, buf_size-count, "%d ", array[i]);
        if (i != 0 && (i % 20) == 0) {
            count += snprintf(buf+count, buf_size-count, "\n");
            lib_log(f, tag, buf);

            count = 0;
            memset(buf, '\0', buf_size*sizeof(char));
        }
    }

    /* Final print if we still have stuff in the buffer. */
    if (count != 0) {
        count += snprintf(buf+count, buf_size-count, "\n");
        lib_log(f, tag, buf);
    }
}
