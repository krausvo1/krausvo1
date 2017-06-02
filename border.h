#ifndef BORDER_H
#define BORDER_H

	/*!
	* Structure representing a border (wall)
	*/
struct TBorder{
public:

	/*!
	* Constructor
	* @param ypos vertical position of the border
	* @param xpos horizontal position of border
	*/
	TBorder(const int & ypos, const int & xpos) : t_ypos(ypos), t_xpos(xpos) {}

private:
	//! Vertical position of the border
	int t_ypos;

	//! Horizontal position of the border
	int t_xpos;
};

#endif