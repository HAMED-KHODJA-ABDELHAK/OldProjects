#ifndef _FILE_OPS_H_
#define _FILE_OPS_H_

/********************* Header Files ***********************/
/* C Headers */

/* Project Headers */

/******************* Constants/Macros *********************/
/* Input and output filenames. */
#define INPUT 			"input.txt"
#define OUTPUT 			"output.txt"

/* Log name, ensure it is sprinted with its rank. */
#define LOG_FORMAT		"log.%d.txt"
#define FILE_SIZE		15

/******************* Type Declarations ********************/

/********************** Prototypes ************************/
/*
 * Counts the number of integers in a given file. File is formatted as a csv.
 */
int lib_count_integers(const char *filename);

/*
 * Opens the filename passed in and reads size numbers into the vals array.
 */
void lib_read_file(const char *filename, int *vals, const int size);

/*
 * Function takes an array of values and prints them to a file.
 * Each integer on a separate line.
 */
void lib_write_file(const char *filename, const int *vals, const int size);

/*
 * Simple logging function to log the status of buffers and other variables.
 * Each process has log for itself called log.rank.txt.
 * Format of log is: date - tag: mesg
 */
void lib_log(FILE *f, const char *tag, const char *mesg);

/*
 * Tracing function, takes a buffer of adequate size and traces the values in array.
 * Prints the to the passed in files.
 */
void lib_trace_array(FILE *f, char *tag, int array[], int size);

#endif /* _FILE_OPS_H_ */
