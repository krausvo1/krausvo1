#include "gate.h"
#include <curses.h>
#include <iostream>
#include <unistd.h>

using namespace std;

#ifndef ATTACKER_H
#define ATTACKER_H

#define A_BASIC '&'
#define A_ADVANCED '@'
#define A_DEAD 'X'


	/*!
	* Parent class representing a atacker
	*/
class CAttacker
{
public:

	/*!
	* Constructor used to initialise a new attacker loaded from a file
	* @param type character representing the attacker to be displayed displayed ('@' advanced attacker, '&' basic attacker)
	* @param ypos vertical position of the attacker
	* @param xpos horizontal position of the attacker
	* @param number ID number of the attacker
	* @param health number of health the attacker has
	*/
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & ID, const int & health);

	/*!
	* Constructor used to initialise a new basic attacker
	* @param type character representing the attacker to be displayed displayed ('@' advanced attacker, '&' basic attacker)
	* @param start the gate through which the attackers enters the game
	* @param number ID number of the attacker
	* @param health number of health the attacker starts with
	*/
	CAttacker(const char & type, const CGate & start, const int & ID, const int & health);

	/*!
	* Implicit destructor
	*/
	virtual ~CAttacker(){};

	/*!
	* Move this attacker to the next coordinate
	* @return true when attackers health is larger than zero, false when otherwise
	*/
	virtual bool Move() = 0;

	/*!
	* Assign a vector of coordinates to the attackers m_start gate
	* @param gate the gate keeping the vector of coordinates
	*/
	void AssignPath(const CGate & gate);

	/*!
	* Checkes whether the attacker has escaped through the exit gate or not
	* @return true when the attacker has escaped, false when otherwise
	*/
	bool CheckWin();

	/*!
	* Says whether the attacker is still alive and has not won yet
	* @return true when attackers health is larger than zero and it has not won yet, false when otherwise 
	*/
	bool IsInGame() const;

	/*!
	* Decreases the amount of attackers health
	* @param damage damage taken, based on the type of shootin tower
	* @param stun bool value m_is_escorted variable is being set to
	*/
	virtual void TakeHit(const int & damage, const bool & stun) = 0;

	/*!
	* Sets m_stunned variable to true or false
	* @param escorted bool value m_stunned variable is being set to 
	*/
	virtual void GiveWay(const bool & give_way);

	char AttackerType() const;

	int AttackerYpos() const;

	int AttackerXpos() const;

	int AttackerRealYpos() const;

	int AttackerRealXpos() const;

	bool IsHit() const;

	void SetHit(const int & hit);

	int AttackerHealth() const;

	int AttackerID() const;

	unsigned int AttackerMoves() const;

	bool HasWon() const;

	bool IsAssigned() const;

	bool IsStunned() const;

	CGate m_start;


protected:

	char m_attacker_type;

	int m_ypos;
	int m_xpos;

	int real_ypos;
	int real_xpos;

	bool m_hit;

	int m_health;
	int m_attacker_ID;

	unsigned int m_moves;
	bool m_attacker_won;

	bool m_stunned;

	bool m_assigned;

	//! Decides whether the attacker moves is giving way another attacker or not
	bool m_giving_way;
};

#endif