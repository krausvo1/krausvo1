#include "attacker.h"

CAttacker::CAttacker(const char & type, const CGate & start, const int & number, const int & health):
					m_attacker_type(type), m_ypos(start.m_ypos), m_xpos(start.m_xpos), //NEW
					m_hit(false), m_health(health), m_number(number), m_start(start), m_moves(2), m_attacker_won(false)
{
	real_ypos = m_start.path[m_start.path.size() - (m_moves)].first;
	real_xpos = m_start.path[m_start.path.size() - (m_moves)].second;
}


CAttacker::CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const int & health): //LOAD 
					m_attacker_type(type), m_ypos(ypos), m_xpos(xpos), 
					m_hit(false), m_health(health), m_number(number), m_start(CGate()),
					m_moves(0), m_attacker_won(false){}


bool CAttacker::AssignPath(const CGate & gate){
	for(unsigned int i = 0; i < gate.path.size(); i++){
		if(std::make_pair(m_ypos,m_xpos) == gate.path[i]){
			m_start = gate;
			m_moves = m_start.path.size() - i;
			real_ypos = m_start.path[m_start.path.size() - m_moves].first;
			real_xpos = m_start.path[m_start.path.size() - m_moves].second;
			return true;
		}
	}
	return false;
}

bool CAttacker::CheckWin(){
	if(m_ypos == m_start.path[0].first && m_xpos == m_start.path[0].second){
		m_attacker_won = true;
		return true;
	}

	return false;
}

bool CAttacker::IsInGame() const{
	return m_health > 0 && !m_attacker_won;
}
