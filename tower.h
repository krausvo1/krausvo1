#ifndef TOWER_H
#define TOWER_H
#include "attacker.h"
#include <vector>

class CTower{
public:
	CTower(const char & tower_type, const int & ypos, const int & xpos);
	~CTower(){};
	virtual void Shoot(CAttacker & attacker);
	bool InRange(const CAttacker & attacker);
	virtual int ChooseTarget();
	void ChargeStun();

	
	char m_tower_type;
	int m_xpos, m_ypos;
	int m_range;
	bool m_stun_ready; //ADVANCED

	std::vector<CAttacker> v_targets;

};

#endif