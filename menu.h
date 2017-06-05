#ifndef MENU_H
#define MENU_H

#include "game.h"

	/*!
	* Class representing a graphical menu
	*/
class CMenu : public CGame{
public:
	
	/*!
	* Constructor initialises ncurses features
	*/
	CMenu();

	/*!
	* Implicit destructor
	*/	
	~CMenu(){};
	
	/*!
	* Initialise the graphical menu and recieve players choice
	*/	
	void Start();

	/*!
	* Receive name of the file to be loaded
	*/
	void Load();

	/*!
	* Display additional information about the game
	*/
	void About();
};

#endif