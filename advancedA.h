#ifndef ADVANCEDA_H
#define ADVANCEDA_H
#include "attacker.h"

	/*!
	* Class representing an advanced atacker
	*/
class CAdvancedAttacker : public CAttacker{
public:

	/*!
	* Constructor used to initialise an advanced attacker loaded from a file
	* @param ypos vertical position of the attacker
	* @param xpos horizontal position of the attacker
	* @param number ID number of the attacker
	* @param health number of health the attacker has
	*/
	CAdvancedAttacker(const int & ypos, const int & xpos, const int & number, const int & health);

	/*!
	* Constructor used to initialise a new advanced attacker
	* @param start the gate through which the attackers enters the game
	* @param number ID number of the attacker
	*/
	CAdvancedAttacker(const CGate & start, const int & number);

	/*!
	* Implicit destructor
	*/
	virtual ~CAdvancedAttacker(){};

	/*!
	* Decreases the amount of attackers health
	* @param damage damage taken, based on the type of shootin tower
	* @param stun bool value m_is_escorted variable is being set to (does not apply to this type of attacker)
	*/
	virtual void TakeHit(const int & damage, const bool & stun);

	/*!
	* Move this attacker to the next coordinate
	* @return true when attackers health is larger than zero, false when otherwise
	*/
	virtual bool Move();
};

#endif