#include "basicA.h"

CBasicAttacker::CBasicAttacker(const int & ypos,
							   const int & xpos,
							   const int & ID,
							   const int & health,
							   const bool & stunned) :

							   CAttacker(A_BASIC, ypos, xpos, ID, health), m_stunned(stunned)
{
}


CBasicAttacker::CBasicAttacker(const CGate & start, 
							   const int & ID) :

							   CAttacker(A_BASIC, start, ID, 150), m_stunned(false)
{
}


void CBasicAttacker::TakeHit(const int & damage, 
							 const bool & stun)
{
	m_hit = true;
	m_health -= damage;
	m_stunned = stun;
}

bool CBasicAttacker::Move()
{
	if(m_health <= 0)
		m_attacker_type = A_DEAD;

	//when the attacker is stunned, it cannot move
	if(m_stunned)
	{
		init_pair(2,COLOR_YELLOW, COLOR_BLACK);
		attron(COLOR_PAIR(2));
		move(m_start.path[m_start.path.size() - m_moves].first,  m_start.path[m_start.path.size() - m_moves].second);
		addch(m_attacker_type);
		attroff(COLOR_PAIR(2));
		return true;
	}

	//when the attacker is only one step before the exit gate
	//it move only by one step
	if(m_start.path.size() - m_moves == 1)
		m_moves++;


	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;

	move(m_ypos, m_xpos);
	addch(m_attacker_type);


	if(m_start.path.size() != m_moves)
	{
		if(!m_giving_way)
			m_moves += 2;
		else
			m_moves++;
	}

	real_ypos = m_start.path[m_start.path.size() - m_moves].first;
	real_xpos = m_start.path[m_start.path.size() - m_moves].second;

	return m_health > 0;
}