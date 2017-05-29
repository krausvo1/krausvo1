#include "basicT.h"

CBasicTower::CBasicTower(const char & tower_type, const int & ypos, const int & xpos):
								CTower(tower_type, ypos, xpos), m_range(2){}

virtual bool CBasicTower::Shoot(CAttacker & attacker){
	attacker.TakeHit(20, false);
}

virtual int CBasicTower::ChooseTarget(){
	for(unsigned int i = 0; i < v_targets.size(); i++){
		if(v_targets[i].m_attacker_type == '@')
			return v_targets[i].m_number;
	}

	if(!v_targets.empty())
		return v_targets[0].m_number;

	return 0;
}