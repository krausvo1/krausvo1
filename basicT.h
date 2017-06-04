#ifndef BASICT_H
#define BASICT_H

#include "tower.h"

	/*!
	* Child class representing a basic tower
	*/
class CBasicTower : public CTower{
public:
	
	/*!
	* Constructor used to initialise a new basic tower
	*/
	CBasicTower							(const int & ypos, 
										 const int & xpos);
	/*!
	* Implicit destructor
	*/
	virtual ~CBasicTower(){};

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
};


#endif