#ifndef _MYLIB_H_
#define _MYLIB_H_

/********************* Header Files ***********************/
/* C Headers */

/* Project Headers */

/******************* Constants/Macros *********************/
#define INPUT "input.txt"
#define OUTPUT "output.txt"
#define ROOT 0
#define GENERATE_FLAG "gen"

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

void m_error(const char * const mesg);

int compare (const void *a, const void *b);

void gen_input(int vals[], int size);

void read_file(const char *file, int *vals, const int size);

void write_file(const char *file, const int *vals, const int size);

#endif /* _MYLIB_H_ */

