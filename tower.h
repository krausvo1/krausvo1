#ifndef TOWER_H
#define TOWER_H
#include "attacker.h"
#include <vector>

class CTower{
public:
	CTower(const char & tower_type, const int & ypos, const int & xpos);
	~CTower(){};
	virtual bool Shoot(CAttacker & attacker);
	virtual void ShootAt(CAttacker & attacker);
	bool InRange(const CAttacer & attacker);
	int ChooseTarget();

	
	char m_tower_type;
	int m_xpos, m_ypos;
	int m_range;

	std::vector<CAttacker> v_targets;

};

#endif