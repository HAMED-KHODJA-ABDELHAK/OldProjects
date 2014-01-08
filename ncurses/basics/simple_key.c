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
#define HEIGHT 10
#define WIDTH 30

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
static char *choices[] = {
		"Choice 1", "Choice 2", "Choice 3", "Choice 4", "Exit"
};
static int n_choices = sizeof(choices)/sizeof(char *);

/****************************** Static Functions **************************************************/
static void init_curses(void) {
	initscr(); /* Allocate and start buffering for curses. */
	cbreak(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
	start_color();
	noecho();
	clear();
}

/****************************** Global Data Definitions *******************************************/
void print_menu(WINDOW *menu_win, int highlight);

/****************************** Global Functions **************************************************/
/**
 * Main loop of the function.
 */
int main(void) {
	WINDOW *menu_win;
	int highlight = 1, choice = 0, c;
	int startx = (80 - WIDTH)/2, starty = (24 - HEIGHT)/2;

	init_curses();
	menu_win = newwin(HEIGHT, WIDTH, starty, startx);
	keypad(menu_win, TRUE);

	mvprintw(0, 0, "Use arrow keys to go up and down.\nPress enter to select choice.");
	refresh();
	print_menu(menu_win, highlight);

	while (1) {
		c = wgetch(menu_win);
		switch (c) {
		case KEY_UP:
			if (highlight == 1)
				highlight = n_choices;
			else
				--highlight;
			break;
		case KEY_DOWN:
			if (highlight == n_choices)
				highlight = 1;
			else
				++highlight;
			break;
		case 10:
			choice = highlight;
			break;
		default:
			mvprintw(24, 0, "Character pressed was %3d, this isn't recognized.");
			refresh();
			break;
		}

		print_menu(menu_win, highlight);
		if (choice != 0)
			break;
	}

	mvprintw(23, 0, "You chose choice %d with choice string %s\n", choice, choices[choice-1]);
	getch();
	clrtoeol();
	endwin();
	return 0;
}

void print_menu(WINDOW *menu_win, int highlight) {
	int x = 2, y= 2;
	box(menu_win, 0, 0);

	for (int i = 0; i < n_choices; ++i) {
		if (highlight == i + 1) {
			wattron(menu_win, A_REVERSE);
			mvwprintw(menu_win, y, x, "%s", choices[i]);
			wattroff(menu_win, A_REVERSE);
		} else {
			mvwprintw(menu_win, y, x, "%s", choices[i]);
		}
		++y;
	}

	wrefresh(menu_win);
}
