#include "advancedT.h"

CAdvancedTower::CAdvancedTower(const char & tower_type, const int & ypos, const int & xpos):
								CTower(tower_type, ypos, xpos), m_range(3), m_stun_ready(true){}

virtual bool CAdvancedTower::Shoot(CAttacker & attacker){
	if(attacker.m_ypos == m_ypos - 2 || attacker.m_ypos == m_ypos + 2 || 
	   	attacker.m_xpos == m_xpos - 2 || attacker.m_xpos == m_xpos + 2 )
	{
		attacker.TakeHit(20, false);
	}
	else{
		attacker.TakeHit(20, m_stun_ready);
		m_stun_ready = false;
	}
}

virtual int CAdvancedTower::ChooseTarget(){
	return v_targets[0].m_number;
}

void CAdvancedTower::ChargeStun(){
	m_stun_ready = true;
}