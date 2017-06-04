#ifndef BORDER_H
#define BORDER_H

#define BORDER '#'

	/*!
	* Structure representing a border (piece of wall)
	*/
struct TBorder{

	/*!
	* Constructor
	* @param ypos vertical position of the border
	* @param xpos horizontal position of border
	*/
	TBorder(const int & ypos, const int & xpos) : t_ypos(ypos), t_xpos(xpos) {}

	//! vertical position of the border
	int t_ypos;

	//! horizontal position of the border
	int t_xpos;
};

#endif