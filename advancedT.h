#ifndef ADVANCEDT_H
#define ADVANCEDT_H
#include "tower.h"
#include "attacker.h"

class CAdvancedTower : public CTower{
public:
	CAdvancedTower(const char & tower_type, const int & ypos, const int & xpos);
	~CAdvancedTower(){};

	void ChargeStun();

	virtual void Shoot(CAttacker & attacker);
	virtual int ChooseTarget();

private:
	bool m_stun_ready;

}

#endif