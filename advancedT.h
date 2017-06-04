#ifndef ADVANCEDT_H
#define ADVANCEDT_H

#include <cmath>

#include "tower.h"

	/*!
	* Child class representing an advanced tower
	*/
class CAdvancedTower : public CTower{
public:

	/*!
	* Constructor used to initialise a new advanced tower
	*/
	CAdvancedTower						(const int & ypos, 
										 const int & xpos);
	/*!
	* Implicit destructor
	*/
	virtual ~CAdvancedTower(){};
	
	/*!
	* Shoot an attacker
	* @param attacker attacker at which the tower is shooting
	*/
	virtual void Shoot					(CAttacker & attacker);
	
	/*!
	* Choose a target to shoot at
	* @return ID number of the attacker at which the tower will shoot
	*/
	virtual int ChooseTarget() const;

	/*!
	* Tell whether the attacker is in the towers range or not
	* @param attacker the attacker being compared with the towers range
	* @return true when attacker is in the towers range, false when otherwise
	*/
	virtual bool InRange				(const CAttacker & attacker);
	
	/*!
	* Check for the towers surroundings to mark the blind spots, at which the tower cannot shoot
	*/
	virtual void CheckBlindSpots();

	/*!
	* Set m_stun_ready to true so the tower is ready to stun a basic attacker if it can
	*/
	virtual void ChargeStun();


private:

	//! decides whether the tower has stun ready to use or not
	bool m_stun_ready;

	//! decides whether the tower uses stun while shooting or not
	bool m_use_stun;
};


#endif