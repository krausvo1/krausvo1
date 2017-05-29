#include "tower.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>


CTower::CTower(const char & tower_type, const int & ypos, const int & xpos):
		   	   m_tower_type(tower_type),m_xpos(xpos),m_ypos(ypos), m_range(2), m_stun_ready(true){}


bool CTower::InRange(const CAttacker & attacker){
	for(int j = - 1; j < m_range; j++)
		for(int k = - 1; k < m_range; k++)
			if((m_ypos + j == attacker.m_ypos) && (m_xpos + k + 1 == attacker.m_xpos))
				return true;

	return false;
}

void CTower::Shoot(CAttacker & attacker){
	// if(attacker.m_ypos == m_ypos - 2 || attacker.m_ypos == m_ypos + 2 || 
	//    	attacker.m_xpos == m_xpos - 2 || attacker.m_xpos == m_xpos + 2 )
	// {
	// 	attacker.TakeHit(0, false);
	// }
	// else{
		attacker.TakeHit(20, m_stun_ready);
		m_stun_ready = false;
	// }
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

void CTower::ChargeStun(){
	m_stun_ready = true;
}