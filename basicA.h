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
	* @param ID ID number of the attacker
	* @param health number of health the attacker has
	*/
	CBasicAttacker(const int & ypos, const int & xpos, const int & ID, const int & health, const bool & stunned);

	/*!
	* Constructor used to initialise a new basic attacker
	* @param start the gate through which the attackers enters the game
	* @param ID ID number of the attacker
	*/
	CBasicAttacker(const CGate & start, const int & ID);

	/*!
	* Implicit destructor
	*/
	virtual ~CBasicAttacker(){};

	/*!
	* Decreases the amount of attackers health
	* @param damage damage taken, based on the type of shooting tower
	* @param stun bool value m_escorted variable is being set to
	*/
	virtual void TakeHit(const int & damage, const bool & stun);

	/*!
	* Move this attacker to the next coordinate
	* @return true when attackers health is larger than zero, false when otherwise 
	*/
	virtual bool Move();

private:

	//! Decides whether this attacker is going to be stunned for a round or not
	bool m_stunned;
};

#endif