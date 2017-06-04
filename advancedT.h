#ifndef ADVANCEDT_H
#define ADVANCEDT_H

#include <cmath>

#include "tower.h"

class CAdvancedTower : public CTower{
public:
	CAdvancedTower(const int & ypos, const int & xpos);
	virtual ~CAdvancedTower(){};

	virtual void ChargeStun();
	virtual void Shoot(CAttacker & attacker);
	virtual int ChooseTarget() const;
	virtual bool InRange(const CAttacker & attacker);
	virtual void CheckRange();


private:
	bool m_stun_ready;
	bool m_use_stun;
};

#endif