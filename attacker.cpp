#include "attacker.h"
#include <curses.h>
#include <unistd.h>

CAttacker::CAttacker(const char & type, const CGate & start, const int & number):
					m_attacker_type(type), m_ypos(start.m_ypos), m_xpos(start.m_xpos), //NEW
					m_hit(false), m_health(60), m_number(number), m_start(start), m_moves(2){}

CAttacker::CAttacker(const char & type, const int & ypos, const int & xpos, const int & number, const CGate & exit): //LOAD 
					m_attacker_type(type), m_ypos(ypos), m_xpos(xpos), 
					m_hit(false), m_health(60), m_number(number), m_exit_gate(exit), m_start(CGate()),
					m_moves(0){}

void CAttacker::Move(){
	move(m_ypos, m_xpos - 1);
	addch(m_attacker_type);
	m_xpos = m_xpos - 1;
}

void CAttacker::NewMove(){

	m_ypos = m_start.path[m_start.path.size() - m_moves].first;
	m_xpos = m_start.path[m_start.path.size() - m_moves].second;	

	move(m_ypos, m_xpos);
	addch(m_attacker_type);

	m_moves++; //kolik pohybů attacker udělal
}

bool CAttacker::AssignPath(const CGate & gate){
	for(unsigned int i = 0; i < gate.path.size(); i++){
		if(std::make_pair(m_ypos,m_xpos)==gate.path[i]){
			m_start = gate;
			m_moves = m_start.path.size() - i;
			return true;
		}
	}
	return false;
}
