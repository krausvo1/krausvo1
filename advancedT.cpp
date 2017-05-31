#include "advancedT.h"
#include <unistd.h>
#include <cmath>

CAdvancedTower::CAdvancedTower(const int & ypos, const int & xpos):
								CTower('I', ypos, xpos), m_stun_ready(true){}

void CAdvancedTower::Shoot(CAttacker & attacker){
	if(std::abs(attacker.real_ypos - m_ypos) == 2 || std::abs(attacker.real_xpos - m_xpos) == 2)
		attacker.TakeHit(5, false);
	else{
		attacker.TakeHit(10, m_stun_ready);
		m_stun_ready = false;
	}
}

int CAdvancedTower::ChooseTarget() const{
	return v_targets[0]->m_number;
}

void CAdvancedTower::ChargeStun(){
	m_stun_ready = true;
}

bool CAdvancedTower::InRange(const CAttacker & attacker) const{
	for(int j = - 1; j < m_range; j++)
		for(int k = - 1; k < m_range; k++)
			if((m_ypos + j == attacker.real_ypos) && (m_xpos + k == attacker.real_xpos) 
				&& ClearShot(attacker))
				return true;

	if(((m_ypos - 2 == attacker.real_ypos && m_xpos == attacker.real_xpos) ||
	   (m_ypos + 2 == attacker.real_ypos && m_xpos == attacker.real_xpos) ||
	   (m_ypos == attacker.real_ypos && m_xpos == attacker.real_xpos - 2) ||
	   (m_ypos == attacker.real_ypos && m_xpos == attacker.real_xpos + 2)) && 
		ClearShot(attacker))
	{
		return true;
	}

	return false;
}

void CAdvancedTower::CheckRange(){
	bool top = false, left = false, right = false, bottom = false;

	for(unsigned int i = 0; i < v_borders.size(); i++){
		if(v_borders[i].t_ypos == m_ypos - 1 && v_borders[i].t_xpos == m_xpos){
			v_blind_spots.push_back(std::make_pair(m_ypos - 2, m_xpos));
			top = true;
		}
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos - 1){
			v_blind_spots.push_back(std::make_pair(m_ypos, m_xpos - 2));
			left = true;
		}
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos + 1){
			v_blind_spots.push_back(std::make_pair(m_ypos, m_xpos + 2));
			right = true;
		}
		
		if(v_borders[i].t_ypos == m_ypos + 1 && v_borders[i].t_xpos == m_xpos){
			v_blind_spots.push_back(std::make_pair(m_ypos + 2, m_xpos));
			bottom = true;
		}
	}

	if(top && left)
		v_blind_spots.push_back(std::make_pair(m_ypos - 1, m_xpos - 1));

	if(top && right)
		v_blind_spots.push_back(std::make_pair(m_ypos - 1, m_xpos + 1));

	if(bottom && left)
		v_blind_spots.push_back(std::make_pair(m_ypos + 1, m_xpos - 1));

	if(bottom && right)
		v_blind_spots.push_back(std::make_pair(m_ypos + 1, m_xpos + 1));
}

