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
#include "log.h"

#ifndef MAP_H
#define MAP_H


class CMap{
public:
	CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates, const std::vector<TBorder> & borders, 
		 const int & maxheight, const int & maxwidth, const CGate & exit);
	
	CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates,
		 const std::vector<CAttacker*> & attackers, const std::vector<TBorder> & borders, const int & maxheight, const int & maxwidth,
		 const CGate & exit, const int & money);

	~CMap();

	std::vector<std::pair<int,int>> FindPath(CGate & start);

	int TestRound(const int & gate_number);

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

	int m_money;
};


#endif


