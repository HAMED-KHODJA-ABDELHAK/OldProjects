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

/**************** Static Data Definitions *****************/


/****************** Static Functions **********************/
static void init_curses(void) {
	initscr(); /* Allocate and start buffering for curses. */
	cbreak(); /* Disable line buffering */
	keypad(stdscr, TRUE); /* Get F1, F2 keys... */
	start_color();
	noecho();
}

/**************** Global Data Definitions *****************/


/****************** Global Functions **********************/
/**
 * Main loop of the function.
 */
int main(void) {
	WINDOW *my_wins[3];
	PANEL *my_panels[3];
	int lines = 10, cols = 40, y = 2, x = 4;

	init_curses();

	for (int i = 0; i < 3; ++i) {
		my_wins[i] = newwin(lines, cols, y + i, x + (5 * i));
		box(my_wins[i], 0, 0);
		my_panels[i] = new_panel(my_wins[i]);
	}

	update_panels();

	doupdate();

	getch();
	endwin();

	return 0;
}

