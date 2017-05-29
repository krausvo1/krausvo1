#include "advancedT.h"
#include <unistd.h>

CAdvancedTower::CAdvancedTower(const int & ypos, const int & xpos):
								CTower('I', ypos, xpos, 3), m_stun_ready(true){}

void CAdvancedTower::Shoot(CAttacker & attacker){
	if(attacker.m_xpos == m_xpos - 1 || attacker.m_xpos == m_xpos + 3){
		attacker.TakeHit(5, false);
	}
	else{
		attacker.TakeHit(10, m_stun_ready);
		m_stun_ready = false;
	}
}

int CAdvancedTower::ChooseTarget(){
	return v_targets[0]->m_number;
}

void CAdvancedTower::ChargeStun(){
	m_stun_ready = true;
}

bool CAdvancedTower::InRange(const CAttacker & attacker){
	for(int j = - 2; j < m_range; j++)
		for(int k = - 2; k < m_range; k++)
			if((m_ypos + j == attacker.m_ypos) && (m_xpos + k + 1 == attacker.m_xpos))
				return true;

	return false;
}