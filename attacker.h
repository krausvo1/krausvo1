#include "gate.h"
#include <curses.h>
#include <unistd.h>

#ifndef ATTACKER_H
#define ATTACKER_H

class CAttacker
{
public:
	CAttacker(const char & type, const CGate & start, const int & number, const int & health, const bool & stunned); //NEW
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health, const bool & stunned); //LOAD
	~CAttacker(){};
	virtual bool Move() = 0;
	bool AssignPath(const CGate & gate);
	bool CheckWin();
	bool IsAlive();
	virtual void TakeHit(const int & damage, const bool & stun) = 0;

	char m_attacker_type;

	int m_ypos;
	int m_xpos;

	bool m_hit;

	int m_health;
	int m_number;

	CGate m_start;

	int m_moves;
	bool m_attacker_won;

	bool m_stunned;

};

#endif