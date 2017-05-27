#ifndef BORDER_H
#define BORDER_H

struct TBorder{
	TBorder(const int & ypos, const int & xpos):t_ypos(ypos), t_xpos(xpos){};
	int t_ypos;
	int t_xpos;
};

#endif