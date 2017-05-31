#include "basicA.h"

CBasicAttacker::CBasicAttacker(const int & ypos, const int & xpos, const int & number, const int & health, const bool & stunned):
							   CAttacker('&', ypos, xpos, number, health), m_stunned(stunned){} //LOAD


CBasicAttacker::CBasicAttacker(const CGate & start, const int & number):
							   CAttacker('&', start, number, 150), m_stunned(false){} //NEW


void CBasicAttacker::TakeHit(const int & damage, const bool & stun){
	m_hit = true;
	m_health -= damage;
	m_stunned = stun;
}

bool CBasicAttacker::Move(){
	if(!(m_health > 0))
		m_attacker_type = 'X';

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

	m_moves++; //kolik pohybů attacker udělal

	real_ypos = m_start.path[m_start.path.size() - m_moves].first;
	real_xpos = m_start.path[m_start.path.size() - m_moves].second;

	return m_health > 0;
}