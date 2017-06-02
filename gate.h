#ifndef GATE_H
#define GATE_H

#include <vector>

	/*!
	* Class representing a gate which attackers use to enter/exit the game
	*/
class CGate{
public:

	/*!
	* Constructor initialising a gate
	* @param gate_type type of the gate, enter/exit 
	* @param ypos vertical position of the gate 
	* @param xpos horizontal position of the gate
	* @param gate_ID ID number of the gate 
	*/
	CGate(const char & gate_type, const int & ypos, const int & xpos, const int & gate_ID);

	/*!
	* Constructor of not specified gate
	*/
	CGate(){};

	/*!
	* Implicit destructor
	*/
	~CGate(){};

	/*!
	* Find a path from this gate to the exit gate
	* @param exit exit gate
	* @return vector of coordinates which are followed by attackers coming from this gate
	*/
	std::vector<std::pair<int,int>> FindPath(const CGate & exit);

	/*!
	* Assign all information of this gate to another gate
	*/
	CGate & operator= (const CGate & src);

	//! the type of the gate (enter/exit)
	char m_gate_type;
	
	//! vertical position of the gate
	int m_ypos;
	
	//! horizontal position of the gate
	int m_xpos;
	
	//! ID number of the gate
	int m_gate_ID;

	//! vector of coordinates which are followed by attackers coming from this gat
	std::vector<std::pair<int,int>> path;
};

#endif