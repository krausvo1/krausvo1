#ifndef TOWER_H
#define TOWER_H
#include "attacker.h"
#include "border.h"
#include <vector>

class CTower{
public:
	CTower(const char & type, const int & ypos, const int & xpos);
	virtual ~CTower(){};
	virtual void Shoot(CAttacker & attacker) = 0;
	virtual int ChooseTarget() const = 0;
	virtual void ChargeStun(){}
	virtual bool InRange(const CAttacker & attacker) const = 0;
	bool ClearShot(const CAttacker & attacker) const;
	virtual void CheckRange() = 0;


	
	char m_tower_type;
	int m_ypos, m_xpos;
	int m_range;

	std::vector<CAttacker*> v_targets;
	std::vector<TBorder> v_borders;
	std::vector<std::pair<int,int> > v_blind_spots;
};

#endif