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
#define HEIGHT 3
#define WIDTH 10
#define CHAR_CORNER '+'
#define CHAR_T_B '-'
#define CHAR_R_L '|'

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
typedef struct win_border_s {
	chtype topBottom, leftRight, corner;
} win_border_t;

typedef struct win_s {
	int startx, starty;
	int height, width;
	win_border_t bord;
} win_t;

/****************************** Static Data Definitions *******************************************/
void print_in_middle(WINDOW *, int, int, int, char *); 

/****************************** Static Functions **************************************************/
static void init_curses(void) {
	initscr(); /* Allocate and start buffering for curses. */
	cbreak(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
	start_color();
	noecho();
}

/****************************** Global Data Definitions *******************************************/


/****************************** Global Functions **************************************************/
/**
 * Main loop of the function.
 */
int main(void) {
	init_curses();
	init_pair(1, COLOR_RED, COLOR_BLACK);

	attron(COLOR_PAIR(1));
	print_in_middle(stdscr, LINES/2, 0, 0, "Voila !!! In color ...");
	attroff(COLOR_PAIR(1));

	getch();
	endwin();
	return 0;
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string) {
	int x, y;
	float temp;

	if (win == NULL)
		win = stdscr;
	
	getyx(win, y, x);
	if (startx != 0) 
		x = startx;
	if (starty != 0)
		y = starty;
	if (width == 0)
		width = 80;

	temp = (width - strlen(string)) / 2;
	x = startx + (int) temp;
	mvwprintw(win, y, x, "%s", string);
	refresh();

}
