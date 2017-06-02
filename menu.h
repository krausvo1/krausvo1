#ifndef MENU_H
#define MENU_H
#include "game.h"

class CMenu : public CGame{
public:
	CMenu();
	~CMenu(){};
	void Start();
	bool Load(); //true - loaded, false - back to menu
	
private:
	int maxwidth;
	int maxheight;
};


#endif