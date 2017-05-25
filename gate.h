#ifndef GATE_H
#define GATE_H

#include <vector>

class CGate{
public:
	CGate(const char & gate_type, const int & ypos, const int & xpos, const int & gate_ID);
	// CGate(const int & gate_type, const int & ypos, const int & xpos);
	
	CGate(){};
	~CGate(){};

	char m_gate_type;
	int m_xpos, m_ypos, m_gate_ID;

	std::vector<std::pair<int,int>> path;
};

#endif