#ifndef ADVANCEDA_H
#define ADVANCEDA_H
#include "attacker.h"

class CAdvancedAttacker : public CAttacker{
public:
	CAdvancedAttacker(const int & ypos, const int & xpos, const int & number, const int & health);
	CAdvancedAttacker(const CGate & start, const int & number);
	virtual ~CAdvancedAttacker(){};

	virtual void TakeHit(const int & damage, const bool & stun);
	virtual bool Move();
};

#endif