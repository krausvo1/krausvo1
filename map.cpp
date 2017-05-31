#include "map.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>

using namespace std;

CMap::CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates, const std::vector<TBorder> & borders, const int & maxheight, const int & maxwidth,
		   const CGate & exit):
		   v_towers(towers), v_gates(gates), v_borders(borders), m_attackers_alive(0), m_maxheight(maxheight), m_maxwidth(maxwidth), 
		   m_exit_gate(exit), m_attackers_won(0), m_logs_on(true)
{
	map.resize(m_maxheight);
	for(int i = 0; i < m_maxheight; i++){
		map[i].resize(m_maxwidth);
	}

	for(unsigned int i = 0; i < v_gates.size(); i++){
		for(int i = 0; i < m_maxheight; i++)
			for(int j = 0; j < m_maxwidth; j++)
				map[i][j] = 0;

		for(unsigned int i = 0; i < v_towers.size(); i++){
			map[v_towers[i]->m_ypos][v_towers[i]->m_xpos] = -8;
		}

		PrintBorders('m');

		if(v_gates[i].m_gate_type != '<'){
			v_gates[i].path = FindPath(v_gates[i]);
		}
	}
}

CMap::CMap(const std::vector<CTower*> & towers, const std::vector<CGate> & gates,
		   const std::vector<CAttacker*> & attackers, const std::vector<TBorder> & borders, 
		   const int & maxheight, const int & maxwidth, const CGate & exit):
		   v_towers(towers), v_gates(gates), v_attackers(attackers), v_borders(borders), m_attackers_alive(0),
		   m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), m_attackers_won(0), m_logs_on(true)
{
	map.resize(m_maxheight);
	for(int i = 0; i < m_maxheight; i++){
		map[i].resize(m_maxwidth);
	}

	for(unsigned int i = 0; i < v_gates.size(); i++){
		for(int i = 0; i < m_maxheight; i++)
			for(int j = 0; j < m_maxwidth; j++)
				map[i][j] = 0;

		for(unsigned int i = 0; i < v_towers.size(); i++){
			map[v_towers[i]->m_ypos][v_towers[i]->m_xpos] = -8;
		}

		PrintBorders('m');

		if(v_gates[i].m_gate_type != '<'){
			v_gates[i].path = FindPath(v_gates[i]);
		}

		for(unsigned int j = 0; j < v_attackers.size(); j++){
			if(v_attackers[j]->m_start.path.empty())
				v_attackers[j]->AssignPath(v_gates[i]);
		}
	}	
}

CMap::~CMap(){
	for(unsigned int i = 0; i < v_towers.size(); i++)
          delete v_towers[i];

    v_towers.clear();

    for(unsigned int i = 0; i < v_attackers.size(); i++)
          delete v_attackers[i];

    v_attackers.clear();
}

vector<pair<int,int> > CMap::FindPath(CGate & start){
	std::vector<pair<int, int> > temp; //uchovává souřadnice

	int y = start.m_ypos;
	int x = start.m_xpos;

	temp.push_back(make_pair(y,x));
	clear();
	// printw("start pos: %d,%d", y,x);
	// printw("\nend pos: %d,%d", m_exit_gate.m_ypos,m_exit_gate.m_xpos);
	// refresh();
	// usleep(1000000);

	//očísluj
	while(temp[0].first != m_exit_gate.m_ypos || temp[0].second != m_exit_gate.m_xpos){ 

		if(y!=0)
		if(y != 0 && map[y - 1][x] == 0){//y != 0 kvůli pozici -1
			temp.push_back(make_pair(y-1,x));
			map[y-1][x] = map[y][x] + 1;
			// move(y-1,x);
			// addch(map[y-1][x]);
			// refresh();
			// usleep(19000);
		}
		
		if(x!=0)
		if(x != 0 && map[y][x - 1] == 0){//x != 0 kvůli pozici -1
			temp.push_back(make_pair(y,x-1));
			map[y][x-1] = map[y][x] + 1;
			// move(y,x-1);
			// addch(map[y][x-1]);
			// refresh();
			// usleep(19000);
		}

		if(y!=m_maxheight-4)
		if(map[y + 1][x] == 0){
			temp.push_back(make_pair(y+1,x));
			map[y+1][x] = map[y][x] + 1;
			// move(y+1,x);
			// addch(map[y+1][x]);
			// refresh();
			// usleep(19000);
		}

		if(x!=m_maxwidth-1)		
		if(map[y][x+1] == 0){
			temp.push_back(make_pair(y,x+1));
			map[y][x+1] = map[y][x] + 1;
			// move(y,x+1);
			// addch(map[y][x+1]);
			// refresh();
			// usleep(19000);
		}

		if(!temp.empty())
			temp.erase(temp.begin());

		y = temp[0].first;
		x = temp[0].second;
	}

	map[start.m_ypos][start.m_xpos] = 0;

	 
	//najdi cestu zpět, z cíle do počátku, path[0] = cíl, path[max] = počátek
	vector<pair<int,int> > path;

	y = m_exit_gate.m_ypos;
	x = m_exit_gate.m_xpos;
	
	path.push_back(make_pair(y,x));
	clear();
	// printw("POnd pos: %d,%d", y,x);
	// refresh();
	// usleep(1000000);
	
	int i = 0;

	// for(int i = 0; i < m_m_maxheight-4; i++)
	// 	for(int j = 0; j < m_maxwidth; j++){
	// 		mvprintw(i,j,"%c",map[i][j]);
	// 		refresh();
	// 	}

	// usleep(1900000);
	// clear();
	// refresh();
	while(!(path[path.size()-1].first == start.m_ypos && path[path.size()-1].second == start.m_xpos)){ 
		if(path.size()>1){
			i++;
			// y = path[path.size() - 1].first;
			// x = path[path.size() - 1].second;
			y = path[path.size()-1].first;
			x = path[path.size()-1].second;
			// mvprintw(0,0,"%d,%d",y,x);
			// refresh();
			// usleep(10000);

		}

		if(y!=0)
		if(map[y - 1][x] == map[y][x] - 1){
			path.push_back(make_pair(y-1,x));
			// move(y-1,x);
			// addch(map[y-1][x]);
			// refresh();
			// usleep(10000);
			continue;
		}

		if(x!=0)
		if(map[y][x - 1] == map[y][x] - 1){
			path.push_back(make_pair(y,x-1));
			// move(y,x-1);
			// addch(map[y][x-1]);
			// refresh();
			// usleep(10000);
			continue;
		}

		if(y!=m_maxheight-4)
		if(map[y + 1][x] == map[y][x] - 1){
			path.push_back(make_pair(y+1,x));
			// move(y+1,x);
			// addch(map[y+1][x]);
			// refresh();
			// usleep(10000);
			continue;
		}
		
		if(x!=m_maxwidth-1)		
		if(map[y][x+1] == map[y][x] - 1){
			path.push_back(make_pair(y,x+1));
			// move(y,x+1);
			// addch(map[y][x+1]);
			// refresh();
			// usleep(10000);
			continue;
		}
	}

	return path;
}

void CMap::NextFrame (){
	PrintBorders('f');
	PrintTowers();
	PrintGates();
	CheckCollisions();
	PrintAttackers();
}

void CMap::PrintBorders (const char & choice){
	for(int i=0; i < m_maxwidth; i++){
		move(0,i);
		if(choice != 'f')
			map[0][i] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxwidth; i++){
		move(m_maxheight-3,i);
		if(choice != 'f')
			map[m_maxheight-3][i] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,0);
		if(choice != 'f')
			map[i][0] = '#';
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,m_maxwidth-1);
		if(choice != 'f')
			map[i][m_maxwidth-1] = '#';
		addch('#');
	}

	for(unsigned int i = 0; i < v_borders.size(); i++){
		move(v_borders[i].t_ypos,v_borders[i].t_xpos);
		if(choice != 'f')
			map[v_borders[i].t_ypos][v_borders[i].t_xpos] = '#';
		addch('#');
	}
}

void CMap::PrintTowers(){
	for(unsigned int i = 0; i < v_towers.size(); i++){
		move(v_towers[i]->m_ypos, v_towers[i]->m_xpos);
		addch(v_towers[i]->m_tower_type);
	}
}

void CMap::PrintGates(){
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

void CMap::AddAttacker (const CGate & start){
	move(0,0);
	printw("Gate %d, select attacker type:\nPress B for basic attacker\nPress A for advanced attacker", start.m_gate_ID);
	refresh();

	nodelay(stdscr, false);
	// char attacker_type = getch();

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