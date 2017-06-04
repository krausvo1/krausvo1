#ifndef LOG_H
#define LOG_H
	
struct TLog{
	TLog(const int & number, const int & health):t_number(number), t_health(health){}

	int t_number;
	int t_health;
};

#endif