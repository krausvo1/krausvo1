#include <curses.h>
#include <iostream>
#include <cstdlib>
#include "menu.h"
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include "tower.h"
#include "gate.h"
#include "attacker.h"
#include "game.h"

using namespace std;

CMenu::CMenu(){
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
}

void CMenu::Start(){
	clear();
	move(0,0);
	printw("Press N to start new game.");

	move(1,0);
	printw("Press L to load previous game.");

	move(2,0);
	printw("Press Q to quit.");

	refresh();
	clear();

	move(0,0);
	char choice;
	choice = getchar();

	while(choice != 'N' && choice != 'n' && choice != 'L' && choice != 'l' &&
		  choice != 'Q' && choice != 'q')
		choice = getchar();
		
	switch(choice){
		case 'N':
		case 'n':
			printw("New game started!");
			usleep(1900000);
			refresh();
			NewGame();
			break;
		case 'L':
		case 'l':
			Load();
			break;
		case 'Q':
		case 'q':
			printw("Bye!");
			refresh();
			usleep(1900000);
			break;
	}

	refresh();
}

bool CMenu::Load(){
	clear();
	printw("Load game - please type the exact name of the savefile (max. 100 characters)");
	refresh();

	move(1,0);
	echo();
	curs_set(1);

	char file_name[100];
	getstr(file_name);

	noecho();
	curs_set(0);
	move(2,0);

	ifstream file(file_name, ios::in);

	if(!file.is_open()){
		printw("Error - cannot open file \"%s\"\nTry again? [Y/N]", file_name);
		refresh();

		while(1)
		{
			switch(getchar())
			{
				case 'Y':
				case 'y':
					Load();
					break;
				case 'N':
				case 'n':
					Start();
					break;
				default:
					continue;
			}

			break;
		}
	}
	else{

		if(!LoadGame(file)){
			Load();
		}
	}

	return true;
}
