#ifndef LOG_H
#define LOG_H

struct TLog{
	TLog(const int & number, const int & health, const int & y, const int & x):t_number(number), t_health(health), ypos(y), xpos(x){};
	int t_number;
	int t_health;
	int ypos;
	int xpos;
};

#endif