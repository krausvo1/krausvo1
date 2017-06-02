#ifndef BASICA_H
#define BASICA_H
#include "attacker.h"
	
	/*!
	* Class representing a basic atacker
	*/
class CBasicAttacker : public CAttacker{
public:

	/*!
	* Constructor used to initialise a basic attacker loaded from a file
	* @param ypos vertical position of the attacker
	* @param xpos horizontal position of the attacker
	* @param number ID number of the attacker
	* @param health number of health the attacker has
	*/
	CBasicAttacker(const int & ypos, const int & xpos, const int & number, const int & health, const bool & stunned);

	/*!
	* Constructor used to initialise a new basic attacker
	* @param start the gate through which the attackers enters the game
	* @param number ID number of the attacker
	*/
	CBasicAttacker(const CGate & start, const int & number);

	/*!
	* Implicit destructor
	*/
	virtual ~CBasicAttacker(){};

	/*!
	* Decreases the amount of attackers health
	* @param damage damage taken, based on the type of shootin tower
	* @param stun bool value m_is_escorted variable is being set to
	*/
	virtual void TakeHit(const int & damage, const bool & stun);

	/*!
	* Move this attacker to the next coordinate
	* @return true when attackers health is larger than zero, false when otherwise 
	*/
	virtual bool Move();

	/*!
	* Sets m_stunned variable to true or false
	* @param escorted bool value m_stunned variable is being set to 
	*/
	virtual void SetIsEscorted(const bool & escorted);

private:

	//! Decides whether this attacker is going to be stunned for a round or not
	bool m_stunned;

	//! Decides whether the attacker moves directly behind an advanced attacker or not
	bool m_is_escorted;
};

#endif