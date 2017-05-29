#include "basicT.h"

CBasicTower::CBasicTower(const int & ypos, const int & xpos):
								CTower('T', ypos, xpos, 2){}

void CBasicTower::Shoot(CAttacker & attacker){
	attacker.TakeHit(20, false);
}

int CBasicTower::ChooseTarget(){
	for(unsigned int i = 0; i < v_targets.size(); i++){
		if(v_targets[i]->m_attacker_type == '@')
			return v_targets[i]->m_number;
	}

	if(!v_targets.empty())
		return v_targets[0]->m_number;

	return 0;
}

bool CBasicTower::InRange(const CAttacker & attacker){
	for(int j = - 1; j < m_range; j++)
		for(int k = - 1; k < m_range; k++)
			if((m_ypos + j == attacker.m_ypos) && (m_xpos + k + 1 == attacker.m_xpos))
				return true;

	return false;
}