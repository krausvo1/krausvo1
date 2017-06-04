#include "menu.h"

CMenu::CMenu()
{
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
}

void CMenu::Start()
{
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

	while(1)
	{
		switch(getchar())
		{
			//NEW GAME
			case 'N':
			case 'n':
				printw("New game started!");
				refresh();
				usleep(1900000);
				
				NewGame();
				break;

			//LOAD GAME
			case 'L':
			case 'l':
				Load();
				break;

			//QUIT GAME
			case 'Q':
			case 'q':
				printw("Bye!");
				refresh();
				usleep(1900000);
				break;

			default:
				continue;
		}

		break;
	}

	refresh();
}

void CMenu::Load()
{
	clear();
	printw("Load game - please type the exact name of the savefile (max. 100 characters)");
	refresh();

	move(3,0);
	echo();
	curs_set(1);

	//get name of the file from the user
	char file_name[50];
	getstr(file_name);

	noecho();
	curs_set(0);
	move(2,0);

	ifstream file(file_name, ios::in);

	if(!file.is_open())
	{
		printw("Error - cannot open file \"%s\"\nTry again? [Y/N]", file_name);
		refresh();

		while(1)
		{
			switch(getchar())
			{	
				//YES
				case 'Y':
				case 'y':
					Load();
					break;

				//NO
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
		
		//if the file fails to load, let the user try again
		if(!LoadGame(file)){
			Load();
		}
	}
}
