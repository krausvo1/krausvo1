#include "advancedA.h"

CAdvancedAttacker::CAdvancedAttacker(const int & ypos, const int & xpos, const int & number, const int & health):
							   CAttacker('@', ypos, xpos, number, health), m_stunned(false){} //LOAD


CAdvancedAttacker::CAdvancedAttacker(const CGate & start, const int & number):
							   CAttacker('@', start, number,  200), m_stunned(false){} //NEW


void CAdvancedAttacker::TakeHit(const int & damage, const bool & stun){
	m_hit = true;
	m_health -= damage;
}

bool CAdvancedAttacker::Move(){
	if(!(m_health > 0))
		m_attacker_type = 'X';

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;	

	move(m_ypos, m_xpos);
	addch(m_attacker_type);

	m_moves++; //kolik pohybů attacker udělal

	real_ypos = m_start.path[m_start.path.size() - m_moves].first;
	real_xpos = m_start.path[m_start.path.size() - m_moves].second;


	return m_health > 0;
}
