#include "gate.h"
#include <curses.h>
#include <iostream>
#include <unistd.h>

using namespace std;

#ifndef ATTACKER_H
#define ATTACKER_H

class CAttacker
{
public:
	CAttacker(const char & type, const CGate & start, const int & number, const int & health); //NEW
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health); //LOAD
	virtual ~CAttacker(){};
	virtual bool Move() = 0;
	void AssignPath(const CGate & gate);
	bool CheckWin();
	bool IsInGame() const;
	virtual void TakeHit(const int & damage, const bool & stun) = 0;
	virtual void SetIsEscorted(const bool & escorted){}

	char m_attacker_type;

	int m_ypos;
	int m_xpos;

	int real_ypos;
	int real_xpos;

	bool m_hit;

	int m_health;
	int m_number;

	CGate m_start;

	int m_moves;
	bool m_attacker_won;

	bool m_stunned;

	bool m_is_assigned;
};

#endif