#ifndef GATE_H
#define GATE_H

#include <vector>
#include <iostream>
#include <cstdlib>
#include <curses.h>

#define G_EXIT '<'

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
	* Assign all information about this gate to another gate
	*/
	CGate & operator= (const CGate & src);

	/*!
	* Get the type of the gate
	* @return type of the gate
	*/
	char GateType() const;


	/*!
	* Get the vertical position of the gate
	* @return the vertical position of the gate
	*/
	int GateYpos() const;


	/*!
	* Get the horizontal position of the gate
	* @return the horizontal position of the gate
	*/
	int GateXpos() const;


	/*!
	* Get the ID of the gate
	* @return ID of the gate
	*/
	int GateID() const;


	/*!
	* Set the vertical position of the gate
	* @param ypos vertical position of the gate
	*/
	void SetGateYpos(const int & ypos);


	/*!
	* Set the horizontal position of the gate
	* @param xpos horizontal position of the gate
	*/
	void SetGateXpos(const int & xpos);
	

	/*!
	* Set the type of the gate
	* @param type type of the gate
	*/
	void SetGateType(const char & type);

	//! vector of coordinates which are followed by attackers coming from this gate
	std::vector<std::pair<int,int>> path;

private:

	//! the type of the gate (enter/exit)
	char m_gate_type;
	
	//! vertical position of the gate
	int m_ypos;
	
	//! horizontal position of the gate
	int m_xpos;
	
	//! ID number of the gate
	int m_gate_ID;
};


#endif