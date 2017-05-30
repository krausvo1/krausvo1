#include "tower.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>


CTower::CTower(const char & type, const int & ypos, const int & xpos):
		   	   m_tower_type(type), m_ypos(ypos), m_xpos(xpos), m_range(2){}

bool CTower::ClearShot(const CAttacker & attacker) const{
	for(unsigned int i = 0; i < v_blind_spots.size(); i++)
		if(attacker.m_ypos == v_blind_spots[i].first && attacker.m_xpos == v_blind_spots[i].second){
			move(20, 40);
			printw("SLEPE MISTO");
			refresh();
			usleep(1000000);
			return false;
		}

	return true;	
}
