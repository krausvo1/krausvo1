#ifndef BASICA_H
#define BASICA_H
#include "attacker.h"

class CBasicAttacker : public CAttacker{
public:
	CBasicAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health); //LOAD
	CBasicAttacker(const char & type, const CGate & start, const int & number); //NEW
	~CBasicAttacker(){};

	virtual void TakeHit(const int & damage, const bool & stun);
	virtual bool Move();

private:
	bool m_stunned;
}

#endif