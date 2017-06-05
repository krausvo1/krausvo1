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

	move(3,0);
	printw("Press A to learn more about the game.");

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

			case 'A':
			case 'a':
				About();
				Start();
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

void CMenu::About()
{	
	move(0,0);
	printw("--------About the game--------");

	move(1,0);
	printw("Your task is to escort attackers");

	move(2,0);
	printw("across the map to the exit gate, marked with \'<\'.");

	move(3,0);
	printw("Attackers can be bought with money.");

	move(4,0);
	printw("You will get appropriate amount of money");

	move(5,0);
	printw("in the beginning of the game,\n");

	move(6,0);
	printw("more money can be earned during the game");

	move(7,0);
	printw("by escorting attackers. For each escorted");

	move(8,0);
	printw("attacker you receive 600$.");

	move(10,0);
	printw("You WIN the game if you manage");

	move(11,0);
	printw("to escort as many attackers as the goal states.");

	move(12,0);
	printw("You LOSE the game if all your attackers");

	move(13,0);
	printw("are dead and you run out of money to buy more of them.");

	move(15,0);
	printw("--------Controls--------");

	move(16,0);
	printw("To send an attacker in the game through a certain gate,");

	move(17,0);
	printw("press the number of the gate and buy desired type of the attacker.");

	move(19,0);
	printw("Press L during the game to switch displaying of logs ON/OFF");

	move(20,0);
	printw("Press S during the game to save the game.");

	move(21,0);
	printw("Press Q during the game to end the game.");

	move(23,0);	
	printw("################## PRESS ANY KEY NOW TO GO BACK TO MENU ##################");
	refresh();

	nodelay(stdscr, false);
	getchar();
}