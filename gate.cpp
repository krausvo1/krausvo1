#include "gate.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>


CGate::CGate(const char & gate_type, const int & ypos, const int & xpos, const int & gate_ID):
		   m_gate_type(gate_type),m_xpos(xpos),m_ypos(ypos), m_gate_ID(gate_ID){}

// CGate::CGate(const int & gate_type, const int & ypos, const int & xpos):
// 		   m_gate_type(gate_type),m_xpos(xpos),m_ypos(ypos){}		   

