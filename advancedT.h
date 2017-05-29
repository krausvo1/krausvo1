#ifndef ADVANCEDT_H
#define ADVANCEDT_H
#include "tower.h"
#include "attacker.h"

class CAdvancedTower : public CTower{
public:
	CAdvancedTower(const int & ypos, const int & xpos);
	~CAdvancedTower(){};

	virtual void ChargeStun();
	virtual void Shoot(CAttacker & attacker);
	virtual int ChooseTarget();
	virtual bool InRange(const CAttacker & attacker);

private:
	bool m_stun_ready;

};

#endif