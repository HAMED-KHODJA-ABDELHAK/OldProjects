/**
 * Template C file for curses.
 * Note multiple types of functions:
 * func: Operate on default stdscr, no relocate.
 * wfunc: Pass window variable.
 * mvfunc: Relocate cursor to y,x.
 * mvwfunc: Window then relocate.
 */
/****************************** Header Files ******************************************************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>

/* Project Headers */
#include <curses.h> /* Note, LINES/COLS represent rows/cols from getmaxyx. */

/****************************** Constants/Macros **************************************************/


/****************************** Type Definitions **************************************************/
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

/****************************** Static Data Definitions *******************************************/


/****************************** Static Functions **************************************************/
static void init_curses(void) {
	initscr(); /* Allocate and start buffering for curses. */
	raw(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
	echo(); /* Disables user echo while getting values. */
}

/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/**
 * Main loop of the function.
 */
int main(int argc, char *argv[]) {
	int ch, prev;
	prev = EOF;
	FILE *fp = NULL;
	int y, x;

	if (argc != 2) {
		printf("Usage: %s <file.c>\n", argv[0]);
		exit(1);
	}

	char f_flag = 'r';
	fp = fopen(argv[1], &f_flag);

	if (fp == NULL) {
		perror("Cannot open input file.");
		exit(1);
	}

	init_curses();

	while ((ch = fgetc(fp)) != EOF) {
		getyx(stdscr, y, x);
		if (y == (LINES - 1)) { /* Got to last line, clear and keep printing. */
			printw("<- Press any key->");
			getch();
			clear(); /* Clear screen */
			move(0, 0);
		}

		if (prev == '/' && ch == '*') {
			attron(A_BOLD);
			getyx(stdscr, y, x);
			move(y, x-1);
			printw("%c%c", prev, ch);
		} else {
			printw("%c", ch);
		}

		refresh();
		if (prev == '*' && ch == '/')
			attroff(A_BOLD);

		prev = ch;
	}

	getch();

	endwin();
	if (fp != NULL)
		fclose(fp);

	return 0;
}

