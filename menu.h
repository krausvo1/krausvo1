#ifndef MENU_H
#define MENU_H

#include "game.h"

	/*!
	* Class for representation of graphical menu
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
	* @return file to be loaded
	*/
	void Load();
};


#endif