#include "advancedA.h"

CAdvancedAttacker::CAdvancedAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health):
							   CAttacker(type, ypos, xpos, number, health), m_attacker_type('@'){} //LOAD


CAdvancedAttacker::CAdvancedAttacker(const char & type, const CGate & start, const int & number):
							   CAttacker(type, start, number), m_attacker_type('@'), m_health(200){} //NEW


virtual void CAdvancedAttacker::TakeHit(const int & damage, const bool & stun){
	m_hit = true;
	m_health -= damage;
}

virtual bool CAdvancedAttacker::Move(){
	if(m_health == 0)
		m_attacker_type = 'X';

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;	

	move(m_ypos, m_xpos);
	addch(m_attacker_type);

	m_moves++; //kolik pohybů attacker udělal

	return m_health > 0;
}