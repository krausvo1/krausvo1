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
		   m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), m_attackers_won(0), m_logs_on(true){}

CMap::CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates,
		   const std::vector<CAttacker*> & attackers, const std::vector<TBorder> & borders, 
		   const int & maxheight, const int & maxwidth, const CGate & exit):
		   v_towers(towers), v_gates(gates), v_attackers(attackers), v_borders(borders), m_attackers_alive(0), 
		   m_first_not_loaded(attackers.size()), m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), 
		   m_attackers_won(0), m_logs_on(true)
{
	for(unsigned int i = 0; i < v_gates.size(); i++){
		for(unsigned int j = 0; j < v_attackers.size(); j++){
			if(v_attackers[j]->m_start.path.empty() && v_gates[i].m_gate_type != '<')
				v_attackers[j]->AssignPath(v_gates[i]);
		}
	}
	
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
		// 	map[0][i] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxwidth; i++){
		move(m_maxheight-3,i);
		// if(choice != 'f')
		// 	map[m_maxheight-3][i] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,0);
		// if(choice != 'f')
		// 	map[i][0] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,m_maxwidth-1);
		// if(choice != 'f')
		// 	map[i][m_maxwidth-1] = '#';
		addch('#');
	}

	for(unsigned int i = 0; i < v_borders.size(); i++){
		move(v_borders[i].t_ypos,v_borders[i].t_xpos);
		// if(choice != 'f')
		// 	map[v_borders[i].t_ypos][v_borders[i].t_xpos] = '#';
		addch('#');
	}
}

void CMap::PrintTowers() const{
	for(unsigned int i = 0; i < v_towers.size(); i++){
		move(v_towers[i]->m_ypos, v_towers[i]->m_xpos);
		addch(v_towers[i]->m_tower_type);
	}
}

void CMap::PrintGates() const{
	for(unsigned int i = 0; i < v_gates.size(); i++){
		move(v_gates[i].m_ypos, v_gates[i].m_xpos);
		addch(v_gates[i].m_gate_type);
	}
}

void CMap::CheckCollisions(){
	for(unsigned int t = 0; t < v_towers.size(); t++){
		for(unsigned int a = 0; a < v_attackers.size(); a++)
			if(v_attackers[a]->IsInGame() && v_towers[t]->InRange(*v_attackers[a]))
				v_towers[t]->v_targets.push_back(v_attackers[a]);

		if(!v_towers[t]->v_targets.empty())
			v_towers[t]->Shoot(*v_attackers[v_towers[t]->ChooseTarget()]);
		else
			if(v_towers[t]->m_tower_type == 'I')
				v_towers[t]->ChargeStun();

		v_towers[t]->v_targets.clear();
	}
}


void CMap::PrintAttackers(){
	start_color();
	init_pair(1,COLOR_RED, COLOR_BLACK);
	
	m_attackers_alive = 0;
	
	for(unsigned int i=0; i < v_attackers.size(); i++){
		if(v_attackers[i]->m_hit){
			if(m_logs_on)
				v_logs.push_back(TLog(v_attackers[i]->m_number, v_attackers[i]->m_health, 
									v_attackers[i]->real_ypos, v_attackers[i]->real_xpos));
			
			v_attackers[i]->m_hit = false;
			attron(COLOR_PAIR(1));
		}

		if(v_attackers[i]->m_attacker_type == '&')
			CheckEscorts(*v_attackers[i]);

		if(!v_attackers[i]->m_attacker_won && v_attackers[i]->m_attacker_type != 'X' && v_attackers[i]->Move()){
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
		if((v_attackers[i]->m_attacker_type == '@') && 
		   (v_attackers[i]->real_ypos == attacker.m_start.path[attacker.m_start.path.size() - (attacker.m_moves+2)].first) &&
		   (v_attackers[i]->real_xpos == attacker.m_start.path[attacker.m_start.path.size() - (attacker.m_moves+2)].second))
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
	printw("Gate %d, select attacker type:\nPress B for basic attacker\nPress A for advanced attacker", start.m_gate_ID);
	refresh();

	nodelay(stdscr, false);

	switch(getch()){
		case 'B':
		case 'b':
			v_attackers.push_back(new CBasicAttacker(start, v_attackers.size()));
			m_attackers_alive++;
			break;
		case 'A':
		case 'a':
			v_attackers.push_back(new CAdvancedAttacker(start, v_attackers.size()));
			m_attackers_alive++;
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

		move(m_maxheight-1,0);
		printw("Attacker %d hit, remaining health: %d, pos: %d,%d", v_logs[i].t_number, health, v_logs[i].ypos, v_logs[i].xpos);
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