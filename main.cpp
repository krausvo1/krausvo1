#include <curses.h>
#include "map.h"
#include "menu.h"
#include "attacker.h"
#include <unistd.h>



int main (const int argc, char *argv[]){
	CMenu menu;
	menu.Start();
	
	nodelay(stdscr, false);
	endwin();
	return 0;
}