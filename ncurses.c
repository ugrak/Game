#include <ncurses.h>

int main(int argc, char *argv[])
{
	int x = 0;
	int y = 0;
	int key;
	
	if ( NULL == ( initscr()) ) {
		fprintf(stderr,"Ncurses init error\n");
		return -1;
	}

	key = 0;
	keypad(stdscr,TRUE);
	do {
		clear();
		mvaddch(y,x,'*');
		refresh();
		key = getch();
		//mvaddch(y,x,' ');

		switch(key) {
		case KEY_UP: y = ( y + LINES - 1 ) % LINES; break;
		case KEY_DOWN: y = ( y  + 1 ) % LINES; break;
		case KEY_LEFT: x = ( x + COLS - 1 ) % COLS; break;
		case KEY_RIGHT: x = ( x  + 1 ) % COLS; break; 
		case ' ': clear(); break;
		default:
			break;
		}
	} while(key != 'q');
	endwin();return 0;
}

