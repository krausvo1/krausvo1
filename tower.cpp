#include "tower.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>


CTower::CTower(const char & type, const int & ypos, const int & xpos, const int & range):
		   	   m_tower_type(type), m_ypos(ypos), m_xpos(xpos), m_range(range){}


// void CTower::Shoot(CAttacker & attacker){
// 	// if(attacker.m_ypos == m_ypos - 2 || attacker.m_ypos == m_ypos + 2 || 
// 	//    	attacker.m_xpos == m_xpos - 2 || attacker.m_xpos == m_xpos + 2 )
// 	// {
// 	// 	attacker.TakeHit(0, false);
// 	// }
// 	// else{
// 		attacker.TakeHit(20, m_stun_ready);
// 		m_stun_ready = false;
// 	// }
// }

// int CTower::ChooseTarget(){
// 	for(unsigned int i = 0; i < v_targets.size(); i++){
// 		if(v_targets[i].m_attacker_type == '@')
// 			return v_targets[i].m_number;
// 	}

// 	if(!v_targets.empty())
// 		return v_targets[0].m_number;

// 	return 0;
// }

// void CTower::ChargeStun(){
// 	m_stun_ready = true;
// }