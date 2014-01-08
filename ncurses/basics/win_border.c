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
#include <ncurses.h> /* Note, LINES/COLS represent rows/cols from getmaxyx. */

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
	cbreak(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
}

/****************************** Global Data Definitions *******************************************/
WINDOW *new_win(int height, int width, int starty, int startx);
void destroy_win(WINDOW *win); 

/****************************** Global Functions **************************************************/
/**
 * Main loop of the function.
 */
int main(void) {
	WINDOW *win;
	int width = 10, height = 3, ch;

	init_curses();
	printw("Press F1 to exit");
	refresh();

	int starty = (LINES - height) / 2;
	int startx = (COLS - width) / 2;

	win = new_win(height, width, starty, startx);
	
	while ((ch = getch()) != KEY_F(1)) {
		switch (ch) {
			case KEY_LEFT:
				startx = startx-1 < 0 ? 0 : startx-1;
				break;
			case KEY_RIGHT:
				startx = startx+width+1 > COLS-1 ? COLS-width : startx+1;
				break;
			case KEY_UP:
				starty = starty-1 < 0 ? 0 : starty-1;
				break;
			case KEY_DOWN:
				starty = starty+height+1 > LINES-1 ? LINES-height: starty+1;
				break;
			default:
				continue;
		}
		destroy_win(win);
		win = new_win(height, width, starty, startx);
	}

	destroy_win(win);
	endwin();
	return 0;
}

WINDOW *new_win(int height, int width, int starty, int startx) {
	WINDOW *win = newwin(height, width, starty, startx);
	box(win, 0, 0);

	wrefresh(win);

	return win;
}

void destroy_win(WINDOW *win) {
	wborder(win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');

	wrefresh(win);
	delwin(win);
}

