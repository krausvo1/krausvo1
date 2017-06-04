#include "menu.h"

int main (const int argc, char *argv[]){
	CMenu menu;
	menu.Start();
	
	nodelay(stdscr, false);
	endwin();
	return 0;
}