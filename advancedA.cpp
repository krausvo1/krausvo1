#include "advancedA.h"

CAdvancedAttacker::CAdvancedAttacker(const int & ypos,
									 const int & xpos,
									 const int & ID,
									 const int & health) :

							   		 CAttacker(A_ADVANCED, ypos, xpos, ID, health)
{
}


CAdvancedAttacker::CAdvancedAttacker(const CGate & start, 
									 const int & ID) :

							 		 CAttacker(A_ADVANCED, start, ID, 200)
{ 
}

void CAdvancedAttacker::TakeHit(const int & damage, 
							    const bool & stun)
{
	m_hit = true;
	m_health -= damage;
}

bool CAdvancedAttacker::Move()
{
	if(!(m_health > 0))
		m_attacker_type = A_DEAD;

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;	

	move(m_ypos, m_xpos);
	addch(m_attacker_type);
	
	if(m_start.path.size() != m_moves && !m_giving_way)
		m_moves++;

	real_ypos = m_start.path[m_start.path.size() - m_moves].first;
	real_xpos = m_start.path[m_start.path.size() - m_moves].second;


	return m_health > 0;
}
