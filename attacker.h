#ifndef ATTACKER_H
#define ATTACKER_H

#include <unistd.h>

#include "gate.h"

using namespace std;

#define A_BASIC '&'
#define A_ADVANCED '@'
#define A_DEAD 'X'


	/*!
	* Parent class representing an atacker
	*/
class CAttacker
{
public:

	/*!
	* Constructor used to initialise a new attacker loaded from a file
	* @param type character representing the attacker to be displayed displayed ('@' advanced attacker, '&' basic attacker)
	* @param ypos vertical position of the attacker
	* @param xpos horizontal position of the attacker
	* @param ID ID number of the attacker
	* @param health number of health the attacker has
	*/
	CAttacker(const char & type, const int & ypos, const int & xpos, const int & ID, const int & health);

	/*!
	* Constructor used to initialise a new basic attacker
	* @param type character representing the attacker to be displayed displayed ('@' advanced attacker, '&' basic attacker)
	* @param start the gate through which the attackers enters the game
	* @param ID ID number of the attacker
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
	* Set variable m_giving_way to true or false
	* @param give_way bool value m_giving_way is being set to
	*/
	void GiveWay(const bool & give_way);

	/*!
	* Get the type of the attacker
	* @return type of the attacker
	*/
	char AttackerType() const;

	/*!
	* Get the displaying vertical position of the attacker
	* @return displaying vertical position of the attacker
	*/
	int AttackerYpos() const;

	/*!
	* Get the displaying  horizontal position of the attacker
	* @return displaying horizontal position of the attacker
	*/
	int AttackerXpos() const;

	/*!
	* Get the real vertical position of the attacker
	* @return real vertical position of the attacker
	*/
	int AttackerRealYpos() const;

	/*!
	* Get the real horizontal position of the attacker
	* @return real horizontal position of the attacker
	*/
	int AttackerRealXpos() const;

	/*!
	* Tell whether the attacker is hit or not
	* @return true when the attacker is hit, false when otherwise
	*/
	bool IsHit() const;

	/*!
	* Set whether the attacker is hit or not
	* @param hit decides whether the attacker is hit or not
	*/
	void SetHit(const int & hit);

	/*!
	* Get the attackers health
	* @return attackers health
	*/
	int AttackerHealth() const;

	/*!
	* Get the attackers ID
	* @return attackers ID
	*/
	int AttackerID() const;

	/*!
	* Get the number of moves the attacker has already made
	* @return number of moves the attacker has already made
	*/
	unsigned int AttackerMoves() const;

	/*!
	* Tell whether the attacker has already won or not
	* @return true when the attacker has already won, false when otherwiset
	*/
	bool HasWon() const;

	/*!
	* Tell whether the attacker has path to follow assigned or not
	* @return true when the attacker has path to follow assigned, false when otherwise
	*/
	bool IsAssigned() const;

	/*!
	* Tell whether the attacker is being stunned this round or not
	* @return true when the attacker is being stunned this round, false when otherwise
	*/
	bool IsStunned() const;

	//! gate through which the attacker enters the game
	CGate m_start;


protected:

	//! the type of the attacker
	char m_attacker_type;

	//! vertical position of the attacker, at which the attacker is being displayed
	int m_ypos;

	//! horizontal position of the attacker, at which the attacker is being displayed
	int m_xpos;

	//! vertical position of the attacker, at which the attacker actually is
	int real_ypos;
	
	//! vertical horizontal of the attacker, at which the attacker actually is	
	int real_xpos;

	//! decides whether the attacker is hit or not
	bool m_hit;

	//! the amount of the attackers health
	int m_health;
	
	//! attackers ID number
	int m_attacker_ID;

	//! amount of moves the attacker has already made
	unsigned int m_moves;

	//! decide whether the attacker has already won the game or not
	bool m_attacker_won;

	//! decide whether the attacker is stunned for a round or not
	bool m_stunned;

	//! decide whether the attacker has a path to follow assigned or not
	bool m_assigned;

	//! Decides whether the attacker moves is giving way to another attacker or not
	bool m_giving_way;
};


#endif