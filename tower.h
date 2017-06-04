#ifndef TOWER_H
#define TOWER_H

#include "attacker.h"
#include "border.h"

#define T_BASIC 'T'
#define T_ADVANCED 'I'


class CTower{
public:
	CTower(const char & type, const int & ypos, const int & xpos);
	virtual ~CTower(){};
	virtual void Shoot(CAttacker & attacker) = 0;
	virtual int ChooseTarget() const = 0;
	virtual void ChargeStun(){}
	virtual bool InRange(const CAttacker & attacker) = 0;
	bool ClearShot(const CAttacker & attacker) const;
	virtual void CheckRange() = 0;

	char TowerType() const;
	int TowerXpos() const;
	int TowerYpos() const;

	void AddTarget(CAttacker * attacker);

	bool TargetsCount() const;

	void ClearTargets();

	void AssignBorders(const std::vector<TBorder> & borders);


protected:

	char m_tower_type;
	int m_ypos, m_xpos;
	int m_range;

	std::vector<CAttacker*> v_targets;
	std::vector<TBorder> v_borders;
	std::vector<std::pair<int,int> > v_blind_spots;
};

#endif