#ifndef TOWER_H
#define TOWER_H

#include "attacker.h"
#include "border.h"

#define T_BASIC 'T'
#define T_ADVANCED 'I'
#define T_RANGE 2

	/*!
	* Parent class representing a tower
	*/
class CTower{
public:

	/*!
	* Constructor used to initialise a new tower
	*/
	CTower								(const char & type, 
										 const int & ypos, 
										 const int & xpos);
	
	/*!
	* Implicit destructor
	*/
	virtual ~CTower(){};
	
	/*!
	* Shoot an attacker
	* @param attacker attacker at which the tower is shooting
	*/
	virtual void Shoot					(CAttacker & attacker) = 0;
	
	/*!
	* Choose a target to shoot at
	* @return ID number of the attacker at which the tower will shoot
	*/
	virtual int ChooseTarget() const = 0;

	/*!
	* Tell whether the attacker is in the towers range or not
	* @param attacker the attacker being compared with the towers range
	* @return true when attacker is in the towers range, false when otherwise
	*/
	virtual bool InRange				(const CAttacker & attacker) = 0;
	
	/*!
	* Check for the towers surroundings to mark the blind spots, at which the tower cannot shoot
	*/
	virtual void CheckBlindSpots() = 0;

	/*!
	* Tell whether the tower is able to shoot at the attacker or not
	* @param attacker attacker being compared with the towers blind spots
	* @return true when the tower is able to shoot at the attacker, false when otherwise
	*/
	bool ClearShot						(const CAttacker & attacker) const;

	/*!
	* Set m_stun_ready to true so the tower is ready to stun a basic attacker if it can
	*/
	virtual void ChargeStun(){}

	/*!
	* Get the type of the tower
	* @return character representing the tower ('@' for advanced attacker, '&' for basic attacker)
	*/
	char TowerType() const;

	/*!
	* Get the horizontal position of the tower
	* @return the horizontal position of the tower
	*/
	int TowerXpos() const;
	
	/*!
	* Get the vertical position of the tower
	* @return the vertical position of the tower
	*/
	int TowerYpos() const;

	/*!
	* Add an attacker the tower is able to shoot at
	* @param attacker attacker which is being added to possible targets for the tower
	*/
	void AddTarget						(CAttacker * attacker);

	/*!
	* Tell whether there are any attackers the tower is able to shoot at or not
	* @return true when there are any, false when otherwise
	*/
	bool TargetsEmpty() const;

	/*!
	* Empty the vector containing possible targets the tower is able to shoot at
	*/
	void ClearTargets();

	/*!
	* Save vector of borders in the game
	* @param borders vector of borders that are in the game
	*/
	void AssignBorders					(const std::vector<TBorder> & borders);


protected:

	//! type of the tower
	char m_tower_type;

	//! vertical position of the tower
	int m_ypos;

	//! horizontal position of the tower
	int m_xpos;

	//! attackers the tower can shoot at
	std::vector<CAttacker*> v_targets;

	//! borders that are in the game
	std::vector<TBorder> v_borders;

	//! blind spots the tower is not able to shoot at
	std::vector<std::pair<int,int> > v_blind_spots;
};


#endif