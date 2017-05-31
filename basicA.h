#ifndef BASICA_H
#define BASICA_H
#include "attacker.h"

class CBasicAttacker : public CAttacker{
public:
	CBasicAttacker(const int & ypos, const int & xpos, const int & number, const int & health, const bool & stunned); //LOAD
	CBasicAttacker(const CGate & start, const int & number); //NEW
	virtual ~CBasicAttacker(){};

	virtual void TakeHit(const int & damage, const bool & stun);
	virtual bool Move();

private:
	bool m_stunned;
};

#endif