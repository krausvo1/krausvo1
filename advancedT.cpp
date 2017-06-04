#include "advancedT.h"

CAdvancedTower::CAdvancedTower(const int & ypos, const int & xpos):
					    	   CTower(T_ADVANCED, ypos, xpos), m_stun_ready(true), m_use_stun(false){}

void CAdvancedTower::Shoot(CAttacker & attacker){
	if(m_use_stun)
		attacker.TakeHit(5, false);
	else{
		attacker.TakeHit(10, m_stun_ready);
		m_stun_ready = false;
	}
}

int CAdvancedTower::ChooseTarget() const{
	return v_targets[0]->AttackerID();
}

void CAdvancedTower::ChargeStun(){
	m_stun_ready = true;
}

bool CAdvancedTower::InRange(const CAttacker & attacker){
	for(int j = - 1; j < m_range; j++)
		for(int k = - 1; k < m_range; k++)
			if((m_ypos + j == attacker.AttackerRealYpos()) && (m_xpos + k == attacker.AttackerRealXpos()) 
				&& ClearShot(attacker)){
				m_use_stun = false;
				return true;
			}

	if(((std::abs(attacker.AttackerRealYpos() - m_ypos) == 2 && m_xpos == attacker.AttackerRealXpos()) ||
	   (std::abs(attacker.AttackerRealXpos() - m_xpos) == 2 && m_ypos == attacker.AttackerRealYpos())) &&
		ClearShot(attacker)){
		m_use_stun = true;
		return true;
	}

	return false;
}

void CAdvancedTower::CheckRange(){
	bool top = false, bottom = false, left = false, right = false;

	for(unsigned int i = 0; i < v_borders.size(); i++){
		if(v_borders[i].t_ypos == m_ypos - 1 && v_borders[i].t_xpos == m_xpos){
			v_blind_spots.push_back(std::make_pair(m_ypos - 2, m_xpos));
			top = true;
		}

		if(v_borders[i].t_ypos == m_ypos + 1 && v_borders[i].t_xpos == m_xpos){
			v_blind_spots.push_back(std::make_pair(m_ypos + 2, m_xpos));
			bottom = true;
		}
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos - 1){
			v_blind_spots.push_back(std::make_pair(m_ypos, m_xpos - 2));
			left = true;
		}
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos + 1){
			v_blind_spots.push_back(std::make_pair(m_ypos, m_xpos + 2));
			right = true;
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

