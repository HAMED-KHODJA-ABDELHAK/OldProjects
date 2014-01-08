/**
 * Template C file.
 */
/********************* Header Files ***********************/
/* C Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <ctype.h>
//#include <limits.h>
//#include <assert.h>

/* Project Headers */
#include <panel.h>

/******************* Constants/Macros *********************/
#define NLINES 		10
#define NCOLS		40

/******************* Type Definitions *********************/
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

void init_wins(WINDOW **wins, int n);
void win_show(WINDOW *win, char *label, int label_color);
void print_in_middle(WINDOW *win, int starty, int startx, 
	int width, char *string, chtype color); 

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/
static void init_curses(void) {
	initscr(); /* Allocate and start buffering for curses. */
	cbreak(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
	start_color();
	noecho();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
}

/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/**
 * Main loop of the function.
 */
int main(void) {
	WINDOW *my_wins[3];
	PANEL *my_panels[3], *top;
	int lines = 10, cols = 40, y = 2, x = 4;

	init_curses();
	init_wins(my_wins, 3);

	for (int i = 0; i < 3; ++i) 
		my_panels[i] = new_panel(my_wins[i]);
	
	for (int i = 0; i < 3; ++i) 
		set_panel_userptr(my_panels[i], my_panels[(i+1)%3]);

	update_panels();

	/* Show it on screen */
	attron(COLOR_PAIR(4));
	mvprintw(LINES-2, 0, "Use tab to browse through the windows. F1 to exit.");
	attroff(COLOR_PAIR(4));
	doupdate();

	top = my_panels[2];
	while ((ch = getch()) != KEY_F(1)) {
		switch (ch) {
			case 9: 
				top = (PANEL *)panel_userptr(top);
				top_panel(top);
				break;
		}
		update_panels();
		doupdate();
	}

	getch();
	endwin();

	return 0;
}

void init_wins(WINDOW **wins, int n) {
	int x = 2, y = 10 ;
	char label[80];

	for (int i = 0; i < n; ++i) {
		wins[i] = newwin(NLINES, NCOLS, y, x);
		sprintf(label, "Windows number %d", i + 1);	
		win_show(wins[i], label, i+1);
		y += 3;
		x += 7;
	}
}

void win_show(WINDOW *win, char *label, int label_color) {
	int startx, starty, height, width;

	getbegyx(win, starty, startx);
	getmaxyx(win, height, width);

	box(win, 0, 0);
	mvwaddch(win, 2, 0, ACS_LTEE);
	mvwhline(win, 2, 1, ACS_HLINE, width-2);
	mvwaddch(win, 2, width-1, ACS_RTEE);

	print_in_middle(win, 1, 0, width, label, COLOR_PAIR(label,color));
}

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color) {
	int length, x, y;
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

	length = strlen(string);
	temp = (width - length)/2;
	x = startx + (int) temp;
	wattron(win, color);
	mvwprintw(win, y, x, "%s", string);
	wattroff(win, color);
	refresh();
}
