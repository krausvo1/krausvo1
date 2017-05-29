#ifndef TOWER_H
#define TOWER_H
#include "attacker.h"
#include <vector>

class CTower{
public:
	CTower(const char & type, const int & ypos, const int & xpos, const int & m_range);
	~CTower(){};
	virtual void Shoot(CAttacker & attacker) = 0;
	virtual bool InRange(const CAttacker & attacker) = 0;
	virtual int ChooseTarget() = 0;
	virtual void ChargeStun(){}

	
	char m_tower_type;
	int m_ypos, m_xpos;
	int m_range;
	// bool m_stun_ready; //ADVANCED

	std::vector<CAttacker*> v_targets;

};

#endif