#include <iostream>
#include <cstdlib>
#include <vector>
#include <curses.h>
#include "gate.h"
#include "border.h"
#include "basicT.h"
#include "advancedT.h"
#include "basicA.h"
#include "advancedA.h"

#ifndef MAP_H
#define MAP_H

struct TLog{
	TLog(const int & number, const int & health, const int & y, const int & x):t_number(number), t_health(health), ypos(y), xpos(x){};
	int t_number;
	int t_health;
	int ypos;
	int xpos;
};

class CMap{
public:
	CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates, const std::vector<TBorder> & borders, 
		 const int & maxheight, const int & maxwidth, const CGate & exit);
	
	CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates,
		 const std::vector<CAttacker*> & attackers, const std::vector<TBorder> & borders, const int & maxheight, const int & maxwidth,
		 const CGate & exit);

	~CMap();

	std::vector<std::pair<int,int>> FindPath(CGate & start);

	void NextFrame();

	void CheckCollisions();

	void CheckEscorts(CAttacker & attacker);

	void PrintBorders() const;
	void PrintTowers() const;
	void PrintGates() const;
	void PrintAttackers();
	
	void PrintLogs();
	void SwitchLogs();

	void AddAttacker (const CGate & gate);


	std::vector<CTower*> v_towers;
	std::vector<CGate> v_gates;
	std::vector<CAttacker*> v_attackers;
	std::vector<TLog> v_logs;
	std::vector<TBorder> v_borders;

	int m_attackers_alive;
	int m_first_not_loaded;

	const int m_maxheight;
	const int m_maxwidth;

	CGate m_exit_gate;

	std::vector<std::vector<int>> map;

	int m_attackers_won;
	bool m_logs_on;

};


#endif


