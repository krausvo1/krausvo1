#include "tower.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>


CTower::CTower(const char & tower_type, const int & ypos, const int & xpos):
		   	   m_tower_type(tower_type),m_xpos(xpos),m_ypos(ypos), m_range(3){}

bool CTower::Shoot(CAttacker & attacker){
	if(InRange(attacker)){
		attacker.m_hit = true;
		attacker.m_health -= 20;
		return true;
	}

	return false;
}

bool CTower::InRange(const CAttacker & attacker){
	for(int j = - 1; j < m_range - 1; j++)
		for(int k = - 1; k < m_range - 1; k++)
			if((m_ypos + j == attacker.m_ypos) && (m_xpos + k + 1 == attacker.m_xpos))
				return true;

	return false;
}

void CTower::ShootAt(CAttacker & attacker){
	attacker.m_hit = true;
	attacker.m_health -= 20;
	// if(attacker.m_health == 0)
	// 	attacker.m_dead = true;
}

int CTower::ChooseTarget(){
	for(unsigned int i = 0; i < v_targets.size(); i++){
		if(v_targets[i].m_attacker_type == '@')
			return v_targets[i].m_number;
	}

	if(!v_targets.empty())
		return v_targets[0].m_number;

	return 0;
}