#include "basicA.h"

CBasicAttacker::CBasicAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health):
							   CAttacker(type, ypos, xpos, number, health), m_attacker_type('&'), m_stunned(false){} //LOAD


CBasicAttacker::CBasicAttacker(const char & type, const CGate & start, const int & number):
							   CAttacker(type, start, number), m_attacker_type('&'), m_health(100), m_stunned(false){} //NEW


virtual void CBasicAttacker::TakeHit(const int & damage, const bool & stun){
	m_hit = true;
	m_health -= damage;
	m_stunned = stun;
}


virtual bool CBasicAttacker::Move(){
	if(m_health == 0)
		m_attacker_type = 'X';

	if(m_stunned){
		move(m_ypos, m_xpos);
		addch(m_attacker_type);
		return 1;
	}

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;	

	move(m_ypos, m_xpos);
	addch(m_attacker_type);

	m_moves++; //kolik pohybů attacker udělal

	return m_health > 0;
}