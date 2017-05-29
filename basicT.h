#ifndef BASICT_H
#define BASICT_H
#include "tower.h"
#include "attacker.h"


class CBasicTower : public CTower{
public:
	CBasicTower(const int & ypos, const int & xpos);
	~CBasicTower(){};

	virtual void Shoot(CAttacker & attacker);
	virtual int ChooseTarget();
	virtual bool InRange(const CAttacker & attacker);

};


#endif