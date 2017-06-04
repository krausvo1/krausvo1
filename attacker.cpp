#include "attacker.h"

CAttacker::CAttacker(const char & type, 
					 const CGate & start, 
					 const int & ID, 
					 const int & health) :

					 m_start(start), m_attacker_type(type), m_ypos(start.GateYpos()), 
					 m_xpos(start.GateXpos()), m_hit(false), m_health(health), m_attacker_ID(ID), 
					 m_moves(2), m_attacker_won(false), m_assigned(false), m_giving_way(false)
{
	real_ypos = m_start.path[m_start.path.size() - (m_moves)].first;
	real_xpos = m_start.path[m_start.path.size() - (m_moves)].second;
}


CAttacker::CAttacker(const char & type,
					 const int & ypos,
					 const int & xpos,
					 const int & ID,
					 const int & health) :

					 m_start(CGate()), m_attacker_type(type), m_ypos(ypos), m_xpos(xpos), 
					 m_hit(false), m_health(health), m_attacker_ID(ID), m_moves(1), 
					 m_attacker_won(false), m_assigned(false), m_giving_way(false)
{
}


void CAttacker::AssignPath(const CGate & gate)
{
	for(unsigned int i = 0; i < gate.path.size() && !m_assigned; i++)
	{
		if(std::make_pair(m_ypos, m_xpos) == gate.path[i])
		{
			m_start = gate;
			m_moves = m_start.path.size() - i;
			real_ypos = m_start.path[m_start.path.size() - m_moves].first;
			real_xpos = m_start.path[m_start.path.size() - m_moves].second;
			m_assigned = true;
			
			return;
		}
	}
}

bool CAttacker::CheckWin()
{
	if((m_ypos == m_start.path[0].first && m_xpos == m_start.path[0].second)   ||
	   (m_ypos == m_start.path[0].first && m_xpos == m_start.path[0].second + 1))
	{
		m_attacker_won = true;
		return true;
	}

	return false;
}

void CAttacker::GiveWay(const bool & give_way)
{
	if(give_way)
		m_giving_way = true;
	else
		m_giving_way = false;
}

bool CAttacker::IsInGame() const
{
	return m_health > 0 && !m_attacker_won;
}

char CAttacker::AttackerType() const
{
	return m_attacker_type;
}

int CAttacker::AttackerYpos() const
{
	return m_ypos;
}

int CAttacker::AttackerXpos() const
{
	return m_xpos;
}

int CAttacker::AttackerRealYpos() const
{
	return real_ypos;
}

int CAttacker::AttackerRealXpos() const
{
	return real_xpos;
}

bool CAttacker::IsHit() const
{
	return m_hit;
}

void CAttacker::SetHit(const int & hit)
{
	m_hit = hit;
}


int CAttacker::AttackerHealth() const
{
	return m_health;
}

int CAttacker::AttackerID() const
{
	return m_attacker_ID;
}

unsigned int CAttacker::AttackerMoves() const
{
	return m_moves;
}

bool CAttacker::HasWon() const
{
	return m_attacker_won;
}

bool CAttacker::IsAssigned() const
{
	return m_assigned;
}

bool CAttacker::IsStunned() const
{
	return m_stunned;
}
