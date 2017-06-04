#include "tower.h"

CTower::CTower(const char & type, 
			   const int & ypos,
			   const int & xpos) :

		   	   m_tower_type(type), m_ypos(ypos), m_xpos(xpos)
{
}

bool CTower::ClearShot(const CAttacker & attacker) const
{
	for(unsigned int i = 0; i < v_blind_spots.size(); i++)
		if(attacker.AttackerRealYpos() == v_blind_spots[i].first && 
		    attacker.AttackerRealXpos() == v_blind_spots[i].second)
			return false;

	return true;	
}

void CTower::AssignBorders(const std::vector<TBorder> & borders)
{
	v_borders = borders;
}

void CTower::AddTarget(CAttacker * attacker)
{
	v_targets.push_back(attacker);
}

bool CTower::TargetsEmpty() const
{
	return v_targets.size() > 0;
}

void CTower::ClearTargets()
{
	v_targets.clear();
}

char CTower::TowerType() const
{
	return m_tower_type;
}

int CTower::TowerYpos() const
{
	return m_ypos;
}

int CTower::TowerXpos() const
{
	return m_xpos;
}