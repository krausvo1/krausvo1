#include <iostream>
#include <cstdlib>
#include <vector>
#include <curses.h>
#include "tower.h"
#include "gate.h"
#include "attacker.h"

#ifndef MAP_H
#define MAP_H

struct TLog{
	TLog(const int & number, const int & health):t_number(number), t_health(health){};
	int t_number;
	int t_health;
};

class CMap{
public:
	CMap(const std::vector<CTower> & twrs, const std::vector<CGate> & gts, 
		 const int & maxheight, const int & maxwidth, const CGate & exit);
	
	CMap(const std::vector<CTower> & twrs, const std::vector<CGate> & gts,
		 const std::vector<CAttacker> & attcks, const int & maxheight, const int & maxwidth,
		 const CGate & exit);

	~CMap(){}

	void NextFrame (const int & maxheight, const int & maxwidth);

	void AddAttacker (const CGate & gate);

	std::vector<std::pair<int,int>> FindPath(CGate & start);

	void PrintBorders (const int & maxheight, const int & maxwidth, const char & choice);
	void PrintTowers();
	void PrintGates();
	void PrintAttackers(const int & maxheight, const int & maxwidth);
	void PrintLogs(const int & maxheight);
	void SwitchLogs();


	std::vector<CTower> towers;
	std::vector<CGate> gates;
	std::vector<CAttacker> attackers;
	std::vector<TLog> logs;

	int m_attackers_alive;

	const int m_maxheight;
	const int m_maxwidth;

	CGate m_exit_gate;

	std::vector<std::vector<int>> map;

	int m_attackers_won;
	bool m_logs_on;

};


#endif


