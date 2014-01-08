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
void init_win_params(win_t *);
void print_win_params(win_t *);
void create_box(win_t *, bool);

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
	win_t win;
	int ch;

	init_curses();
	init_pair(1, COLOR_CYAN, COLOR_BLACK);

	/* Init params */
	init_win_params(&win);
	print_win_params(&win);
	attron(COLOR_PAIR(1));
	printw("Press F1 to exit.");
	refresh();
	attroff(COLOR_PAIR(1));

	create_box(&win, TRUE);
	while ((ch = getch()) != KEY_F(1)) {
		create_box(&win, FALSE);
		switch (ch) {
			case KEY_LEFT:
				win.startx = win.startx-1 < 0 ? 0 : win.startx-1;
				break;
			case KEY_RIGHT:
				win.startx = win.startx+WIDTH+1 > COLS-1 ? COLS-WIDTH-1 : win.startx+1;
				break;
			case KEY_UP:
				win.starty = win.starty-1 < 0 ? 0 : win.starty-1;
				break;
			case KEY_DOWN:
				win.starty = win.starty+HEIGHT+1 > LINES-1 ? LINES-HEIGHT-1: win.starty+1;
				break;
			default:
				continue;
		}
		create_box(&win, TRUE);
	}

	endwin();
	return 0;
}

void init_win_params(win_t *p_win) {
	p_win->height = HEIGHT;
	p_win->width = WIDTH;
	p_win->starty = (LINES - p_win->height)/2;
	p_win->startx = (COLS - p_win->width)/2;

	p_win->bord.topBottom = CHAR_T_B;
	p_win->bord.leftRight = CHAR_R_L;
	p_win->bord.corner = CHAR_CORNER;
}

void print_win_params(win_t *p_win) {
	mvprintw(25, 0, "%d %d %d %d\n", p_win->startx, p_win->starty,
			p_win->width, p_win->height);
	refresh();
}

void create_box(win_t *p_win, bool flag) {
	int x = p_win->startx, y = p_win->starty, w = p_win->width, h = p_win->height;

	if (flag == TRUE) {
		mvaddch(y, x, p_win->bord.corner);
		mvaddch(y, x+w, p_win->bord.corner);
		mvaddch(y+h, x, p_win->bord.corner);
		mvaddch(y+h, x+w, p_win->bord.corner);
		mvhline(y, x+1, p_win->bord.topBottom, w-1);
		mvhline(y+h, x+1, p_win->bord.topBottom, w-1);
		mvvline(y+1, x, p_win->bord.leftRight, h-1);
		mvvline(y+1, x+w, p_win->bord.leftRight, h-1);
	} else { // Erase the old window.
		for (int j = y; j <= y + h; ++j)
			for (int i = x; i <= x + w; ++i)
				mvaddch(j, i, ' ');
	}

	refresh();
}
