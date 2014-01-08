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
int main(void) {
	char mesg[] = "Enter a string: ", str[80];

	init_curses();

	mvprintw(LINES/2, (COLS - strlen(mesg))/2, "%s", mesg);
	getstr(str);
	mvprintw(LINES-2, 0, "You entered: %s", str);

	refresh();
	getch();
	endwin();

	return 0;
}

