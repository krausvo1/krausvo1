#include "basicT.h"

CBasicTower::CBasicTower(const int & ypos, 
						 const int & xpos) :

						 CTower(T_BASIC, ypos, xpos)
{
}

void CBasicTower::Shoot(CAttacker & attacker)
{
	attacker.TakeHit(10, false);
}

bool CBasicTower::InRange(const CAttacker & attacker)
{
	//check whether the attacker is in towers 3x3 range
	for(int j = - 1; j < T_RANGE; j++)
		for(int k = - 1; k < T_RANGE; k++)
			if((m_ypos + j == attacker.AttackerRealYpos()) && 
			   (m_xpos + k == attacker.AttackerRealXpos()) &&
				ClearShot(attacker))
				return true;

	return false;
}

int CBasicTower::ChooseTarget() const
{
	//if possible, choose the first tank who stepped in range
	for(unsigned int i = 0; i < v_targets.size(); i++){
		if(v_targets[i]->AttackerType() == A_ADVANCED)
			return v_targets[i]->AttackerID();
	}

	//choose the first attacker who stepped in range
	if(!v_targets.empty())
		return v_targets[0]->AttackerID();

	return 0;
}

void CBasicTower::CheckBlindSpots(){
	bool top = false, bottom = false, left = false, right = false;

	for(unsigned int i = 0; i < v_borders.size(); i++)
	{
		if(v_borders[i].t_ypos == m_ypos - 1 && v_borders[i].t_xpos == m_xpos)
			top = true;

		if(v_borders[i].t_ypos == m_ypos + 1 && v_borders[i].t_xpos == m_xpos)
			bottom = true;
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos - 1)
			left = true;
		
		if(v_borders[i].t_ypos == m_ypos && v_borders[i].t_xpos == m_xpos + 1)
			right = true;
	}


	if(top && left)
		v_blind_spots.push_back(std::make_pair(m_ypos - 1, m_xpos - 1));

	if(top && right)
		v_blind_spots.push_back(std::make_pair(m_ypos - 1, m_xpos + 1));

	if(bottom && left)
		v_blind_spots.push_back(std::make_pair(m_ypos + 1, m_xpos - 1));

	if(bottom && right)
		v_blind_spots.push_back(std::make_pair(m_ypos + 1, m_xpos + 1));
}
