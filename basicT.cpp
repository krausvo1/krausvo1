#include "basicT.h"

CBasicTower::CBasicTower(const int & ypos, const int & xpos):CTower('T', ypos, xpos){}

void CBasicTower::Shoot(CAttacker & attacker){
	attacker.TakeHit(20, false);
}

bool CBasicTower::InRange(const CAttacker & attacker) const{
	for(int j = - 1; j < m_range; j++)
		for(int k = - 1; k < m_range; k++)
			if((m_ypos + j == attacker.real_ypos) && (m_xpos + k == attacker.real_xpos) 
				&& ClearShot(attacker))
				return true;

	return false;
}

int CBasicTower::ChooseTarget() const{
	for(unsigned int i = 0; i < v_targets.size(); i++){
		if(v_targets[i]->m_attacker_type == '@')
			return v_targets[i]->m_number;
	}

	if(!v_targets.empty())
		return v_targets[0]->m_number;

	return 0;
}

void CBasicTower::CheckRange(){
	bool top = false, left = false, right = false, bottom = false;

	for(unsigned int i = 0; i < v_borders.size(); i++){
		if(v_borders[i].t_ypos == m_ypos - 1 && v_borders[i].t_xpos == m_xpos)
			top = true;
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos - 1)
			left = true;
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos + 1)
			right = true;
		
		if(v_borders[i].t_ypos == m_ypos + 1 && v_borders[i].t_xpos == m_xpos)
			bottom = true;
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

// bool CBasicTower::InRange(const CAttacker & attacker){
// 	for(int j = - 1; j < m_range; j++)
// 		for(int k = - 1; k < m_range; k++)
// 			if((m_ypos + j == attacker.m_ypos) && (m_xpos + k == attacker.m_xpos) && attacker.IsAlive())
// 				v_targets.pushback(attacker);
// }