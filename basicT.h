#ifndef BASICT_H
#define BASICT_H
#include "tower.h"
#include "attacker.h"


class CBasicTower : public CTower{
public:
	CBasicTower(const int & ypos, const int & xpos);
	virtual ~CBasicTower(){};

	virtual void Shoot(CAttacker & attacker);
	virtual int ChooseTarget() const;
	virtual bool InRange(const CAttacker & attacker) const;
	virtual void CheckRange();


};


#endif