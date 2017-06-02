#include "gate.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>


CGate::CGate(const char & gate_type, const int & ypos, const int & xpos, const int & gate_ID):
		   m_gate_type(gate_type), m_ypos(ypos), m_xpos(xpos), m_gate_ID(gate_ID), path(){}

CGate & CGate::operator= (const CGate & src){
	m_gate_type = src.m_gate_type;
	m_xpos = src.m_xpos;
	m_ypos = src.m_ypos;
	m_gate_ID = src.m_gate_ID;
	path = src.path;

	return *this;
}
