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
#include <curses.h>

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
	noecho(); /* Disables user echo while getting values. */
}

/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/**
 * Main loop of the function.
 */
int main(void) {
	init_curses();


	char mesg[] = "Just a string";
	int row, col;

	getmaxyx(stdscr, row, col);
	mvprintw(row/2, (col - strlen(mesg))/2, "%s", mesg);
	mvprintw(row-2, 0, "This screen has %d rows and %d columns\n", row, col);
	printw("Try resizing your window if possible and then rerunning the program.");

	mvaddstr(2, 22, "Testing.");

	refresh();
	getch();
	endwin();

	return 0;
}

