#include "gate.h"

#ifndef ATTACKER_H
#define ATTACKER_H

class CAttacker
{
public:
	CAttacker(const char & type, const CGate & start, const int & number); //NEW
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health); //LOAD
	~CAttacker(){};
	virtual bool Move();
	bool AssignPath(const CGate & gate);
	bool CheckWin();
	virtual void TakeHit(const int & damage, const bool & stun);

	char m_attacker_type;

	int m_ypos;
	int m_xpos;

	bool m_hit;

	int m_health;
	int m_number;

	CGate m_start;

	int m_moves;
	bool m_attacker_won;

	bool m_stunned; //nebude!

};

#endif