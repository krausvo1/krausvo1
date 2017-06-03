#include "map.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>

using namespace std;

CMap::CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates, 
		   const std::vector<TBorder> & borders, const int & maxheight, const int & maxwidth, 
		   const CGate & exit):
		   v_towers(towers), v_gates(gates), v_borders(borders), m_attackers_alive(0), m_first_not_loaded(0), 
		   m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), m_attackers_won(0), m_logs_on(true), m_money(0)
{
}

CMap::CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates,
		   const std::vector<CAttacker*> & attackers, const std::vector<TBorder> & borders, 
		   const int & maxheight, const int & maxwidth, const CGate & exit, const int & money):
		   v_towers(towers), v_gates(gates), v_attackers(attackers), v_borders(borders), m_attackers_alive(0), 
		   m_first_not_loaded(attackers.size()), m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), 
		   m_attackers_won(0), m_logs_on(true), m_money(money)
{
}

CMap::~CMap(){
	for(unsigned int i = m_first_not_loaded; i < v_attackers.size(); i++)
          delete v_attackers[i];  
}

void CMap::NextFrame (){
	PrintBorders();
	PrintTowers();
	PrintGates();
	CheckCollisions();
	PrintAttackers();
}

void CMap::PrintBorders() const{
	for(int i=0; i < m_maxwidth; i++){
		move(0,i);
		// if(choice != 'f')
		// 	map[0][i] = BORDER;
		addch(BORDER);
	}

	for(int i=0; i < m_maxwidth; i++){
		move(m_maxheight-3,i);
		// if(choice != 'f')
		// 	map[m_maxheight-3][i] = BORDER;
		addch(BORDER);
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,0);
		// if(choice != 'f')
		// 	map[i][0] = BORDER;
		addch(BORDER);
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,m_maxwidth-1);
		// if(choice != 'f')
		// 	map[i][m_maxwidth-1] = BORDER;
		addch(BORDER);
	}

	for(unsigned int i = 0; i < v_borders.size(); i++){
		move(v_borders[i].t_ypos,v_borders[i].t_xpos);
		// if(choice != 'f')
		// 	map[v_borders[i].t_ypos][v_borders[i].t_xpos] = BORDER;
		addch(BORDER);
	}
}

void CMap::PrintTowers() const{
	for(unsigned int i = 0; i < v_towers.size(); i++){
		move(v_towers[i]->TowerYpos(), v_towers[i]->TowerXpos());
		addch(v_towers[i]->TowerType());
	}
}

void CMap::PrintGates() const{
	for(unsigned int i = 0; i < v_gates.size(); i++){
		move(v_gates[i].GateYpos(), v_gates[i].GateXpos());
		addch(v_gates[i].GateType());
	}
}

void CMap::CheckCollisions(){
	for(unsigned int t = 0; t < v_towers.size(); t++){
		for(unsigned int a = 0; a < v_attackers.size(); a++)
			if(v_attackers[a]->IsInGame() && v_towers[t]->InRange(*v_attackers[a]))
				v_towers[t]->AddTarget(v_attackers[a]);

		if(v_towers[t]->TargetsCount())
			v_towers[t]->Shoot(*v_attackers[v_towers[t]->ChooseTarget()]);
		else
			if(v_towers[t]->TowerType() == T_ADVANCED)
				v_towers[t]->ChargeStun();

		v_towers[t]->ClearTargets();
	}
}

void CMap::PrintAttackers(){
	start_color();
	init_pair(1,COLOR_RED, COLOR_BLACK);
	
	m_attackers_alive = 0;
	
	for(unsigned int i = 0; i < v_attackers.size(); i++){
		if(v_attackers[i]->IsHit()){
			if(m_logs_on)
				v_logs.push_back(TLog(v_attackers[i]->AttackerID(), v_attackers[i]->AttackerHealth()));
			
			v_attackers[i]->SetHit(false);
			attron(COLOR_PAIR(1));
		}

		if(v_attackers[i]->AttackerType() == A_BASIC)
			CheckEscorts(*v_attackers[i]);

		if(!v_attackers[i]->HasWon() && v_attackers[i]->AttackerType() != A_DEAD && v_attackers[i]->Move()){
			if(v_attackers[i]->CheckWin()){
				m_attackers_won++;
			}
			else
				m_attackers_alive++;

		}

		attroff(COLOR_PAIR(1));
	}
}

void CMap::CheckEscorts(CAttacker & attacker){
	for(unsigned int i = 0; i < v_attackers.size() - 1; i++){
		if((v_attackers[i]->AttackerType() == A_ADVANCED) && 
		   (v_attackers[i]->m_start.path.size() - (attacker.AttackerMoves() + 2) > 0) &&
		   (v_attackers[i]->AttackerRealYpos() == attacker.m_start.path[attacker.m_start.path.size() - (attacker.AttackerMoves() + 2)].first) &&
		   (v_attackers[i]->AttackerRealXpos() == attacker.m_start.path[attacker.m_start.path.size() - (attacker.AttackerMoves() + 2)].second))
		{
			attacker.SetIsEscorted(true);
			break;
		}
		else
			attacker.SetIsEscorted(false);
	}
}

void CMap::AddAttacker (const CGate & start){
	move(0,0);

	if(m_money == 0){
		printw("Not enough money for more attackers! (%d$)", m_money);
		refresh();
		usleep(2000000);
		return;
	}

	printw("Gate %d, select attacker type:\t\t%d$\nPress B for basic attacker (-100$)\nPress A for advanced attacker (-200$)", start.GateID(), m_money);
	refresh();

	nodelay(stdscr, false);

	switch(getch()){
		case 'B':
		case 'b':
			if(m_money < 100){
				move(3,0);
				printw("Not enough money for a basic attacker! (%d$)", m_money);
				refresh();
				usleep(2000000);
				break;
			}
			v_attackers.push_back(new CBasicAttacker(start, v_attackers.size()));
			m_attackers_alive++;
			m_money -= 100;
			break;
		case 'A':
		case 'a':
			if(m_money < 200){
				move(3,0);
				printw("Not enough money for an advanced attacker! (%d)", m_money);
				refresh();
				usleep(2000000);
				break;
			}
			v_attackers.push_back(new CAdvancedAttacker(start, v_attackers.size()));
			m_attackers_alive++;
			m_money -= 200;
			break;
		default:
			break;
	}

	nodelay(stdscr, true);
	clear();
}

void CMap::PrintLogs(){
	int health;

	for(unsigned int i = 0; i < v_logs.size(); i++){
		if(!(v_logs[i].t_health > 0))
			health = 0;
		else
			health = v_logs[i].t_health;

		move(m_maxheight - 1,0);
		printw("Attacker %d hit, remaining health: %d", v_logs[i].t_number, health);
		refresh();
		usleep(3000000);
	}

	v_logs.clear();
}


void CMap::SwitchLogs(){
	if(m_logs_on)
		m_logs_on = false;
	else
		m_logs_on = true;
}

int CMap::TestRound(const int & gate_number){
	CAdvancedAttacker test(v_gates[gate_number], 0);

	while(!test.CheckWin()){
		for(unsigned int t = 0; t < v_towers.size(); t++){
			if(v_towers[t]->InRange(test))
				v_towers[t]->Shoot(test);
		}

		test.Move();
	}

	return 200 - test.AttackerHealth();
}