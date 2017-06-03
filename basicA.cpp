#include "basicA.h"

CBasicAttacker::CBasicAttacker(const int & ypos, const int & xpos, const int & ID, const int & health, const bool & stunned):
							   CAttacker(A_BASIC, ypos, xpos, ID, health), m_stunned(stunned), m_escorted(false){} //LOAD


CBasicAttacker::CBasicAttacker(const CGate & start, const int & ID):
							   CAttacker(A_BASIC, start, ID, 150), m_stunned(false), m_escorted(false){} //NEW


void CBasicAttacker::TakeHit(const int & damage, const bool & stun){
	m_hit = true;
	m_health -= damage;
	m_stunned = stun;
}

void CBasicAttacker::SetIsEscorted(const bool & escorted){
	if(escorted)
		m_escorted = true;
	else
		m_escorted = false;
}

bool CBasicAttacker::Move(){
	if(!(m_health > 0))
		m_attacker_type = A_DEAD;

	if(m_stunned){
		init_pair(2,COLOR_YELLOW, COLOR_BLACK);
		attron(COLOR_PAIR(2));
		move(m_start.path[m_start.path.size() - m_moves].first,  m_start.path[m_start.path.size() - m_moves].second);
		addch(m_attacker_type);
		attroff(COLOR_PAIR(2));
		return true;
	}

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;

	move(m_ypos, m_xpos);
	addch(m_attacker_type);

	if(!m_escorted)
		m_moves += 2; //kolik pohybů attacker udělal
	else
		m_moves++;

	real_ypos = m_start.path[m_start.path.size() - m_moves].first;
	real_xpos = m_start.path[m_start.path.size() - m_moves].second;

	return m_health > 0;
}