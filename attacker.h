#include "gate.h"

#ifndef ATTACKER_H
#define ATTACKER_H

class CAttacker
{
public:
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const CGate & exit); //LOAD
	CAttacker(const char & type, const CGate & start, const int & number); //NEW
	~CAttacker(){};
	virtual void Move();
	virtual void NewMove();



	char m_attacker_type;

	int m_xstart;
	int m_ystart;

	int m_ypos;
	int m_xpos;

	bool m_hit;

	int m_health;
	int m_number;

	CGate m_exit_gate;
	CGate m_start;

	int m_moves;

};

#endif