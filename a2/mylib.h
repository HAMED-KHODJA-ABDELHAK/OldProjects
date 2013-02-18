#ifndef _MYLIB_H_
#define _MYLIB_H_

/********************* Header Files ***********************/
/* C Headers */

/* Project Headers */

/******************* Constants/Macros *********************/
#define INPUT "input.txt"
#define OUTPUT "output.txt"
#define GENERATE_FLAG "gen"
#define MAX_VAL 1000000
#define ROOT 0

/******************* Type Declarations ********************/


/********************** Prototypes ************************/

void m_error(const char * const mesg);
int compare (const void *a, const void *b);
void gen_input(int vals[], int size);
void read_file(const char *file, int *vals, const int size);
void write_file(const char *file, const int *vals, const int size);

#endif /* _MYLIB_H_ */

