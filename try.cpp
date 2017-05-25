#include <curses.h>

int main (){
	initscr();
	noecho();
	curs_set(0);

	int maxheight,maxwidth;
	getmaxyx(stdscr,maxheight,maxwidth);
	initborders(maxheight,maxwidth);
	

	refresh();
	getchar();
	endwin();
	return 0;
}