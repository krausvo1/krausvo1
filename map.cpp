#include "map.h"
#include <iostream>
#include <cstdlib>
#include <curses.h>
#include <vector>
#include <unistd.h>

using namespace std;

CMap::CMap(const std::vector<CTower> & twrs, const std::vector<CGate> & gts, const int & maxheight, const int & maxwidth,
		   const CGate & exit):
		   towers(twrs), gates(gts), m_maxheight(maxheight), m_maxwidth(maxwidth),m_exit_gate(exit)
{
	// map = new int*[maxheight];
	// for(int i = 0; i < maxheight; i++){
	// 	map[i] = new int[maxwidth];
	// }

	map.resize(maxheight);
	for(int i = 0; i < maxheight; i++){
		map[i].resize(maxwidth);
	}

	for(unsigned int i = 0; i < gates.size(); i++){
		for(int i = 0; i < maxheight; i++)
			for(int j = 0; j < maxwidth; j++)
				map[i][j] = 0;

		for(unsigned int i = 0; i < towers.size(); i++){
			map[towers[i].m_ypos][towers[i].m_xpos] = -8;
		}

		PrintBorders(m_maxheight, m_maxwidth, 'm');

		if(gates[i].m_gate_type != '<'){
			gates[i].path = FindPath(gates[i]);
		}
	}
}

CMap::~CMap(){
	// for(int i = 0; i < m_maxheight; i++){
	// 	delete [] map[i];
	// }

	// delete [] map;
}

vector<pair<int,int> > CMap::FindPath(CGate & start){
	std::vector<pair<int, int> > temp; //uchovává souřadnice

	int y = start.m_ypos;
	int x = start.m_xpos;

	temp.push_back(make_pair(y,x));
	clear();
	printw("start pos: %d,%d", y,x);
	printw("\n end pos: %d,%d", m_exit_gate.m_ypos,m_exit_gate.m_xpos);
	refresh();
	usleep(1000000);

	//očísluj
	while(temp[0].first != m_exit_gate.m_ypos || temp[0].second != m_exit_gate.m_xpos){ 

		if(y!=0)
		if(y != 0 && map[y - 1][x] == 0){//y != 0 kvůli pozici -1
			temp.push_back(make_pair(y-1,x));
			map[y-1][x] = map[y][x] + 1;
			move(y-1,x);
			addch(map[y-1][x]);
			refresh();
			// usleep(19000);
		}
		
		if(x!=0)
		if(x != 0 && map[y][x - 1] == 0){//x != 0 kvůli pozici -1
			temp.push_back(make_pair(y,x-1));
			map[y][x-1] = map[y][x] + 1;
			move(y,x-1);
			addch(map[y][x-1]);
			refresh();
			// usleep(19000);
		}

		if(y!=m_maxheight-4)
		if(map[y + 1][x] == 0){
			temp.push_back(make_pair(y+1,x));
			map[y+1][x] = map[y][x] + 1;
			move(y+1,x);
			addch(map[y+1][x]);
			refresh();
			// usleep(19000);
		}

		if(x!=m_maxwidth-1)		
		if(map[y][x+1] == 0){
			temp.push_back(make_pair(y,x+1));
			map[y][x+1] = map[y][x] + 1;
			move(y,x+1);
			addch(map[y][x+1]);
			refresh();
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
	printw("POnd pos: %d,%d", y,x);
	refresh();
	usleep(1000000);
	
	int i = 0;

	for(int i = 0; i < m_maxheight-4; i++)
		for(int j = 0; j < m_maxwidth; j++){
			mvprintw(i,j,"%c",map[i][j]);
			refresh();
		}
	usleep(1900000);
	clear();
	refresh();
	while(!(path[path.size()-1].first == start.m_ypos && path[path.size()-1].second == start.m_xpos)){ 
		if(path.size()>1){
			i++;
			// y = path[path.size() - 1].first;
			// x = path[path.size() - 1].second;
			y = path[path.size()-1].first;
			x = path[path.size()-1].second;
			mvprintw(0,0,"%d,%d",y,x);
			refresh();
			usleep(1000);

		}

		if(y!=0)
		if(map[y - 1][x] == map[y][x] - 1){
			path.push_back(make_pair(y-1,x));
			move(y-1,x);
			addch(map[y-1][x]);
			refresh();
			usleep(1000);
			continue;
		}

		if(x!=0)
		if(map[y][x - 1] == map[y][x] - 1){
			path.push_back(make_pair(y,x-1));
			move(y,x-1);
			addch(map[y][x-1]);
			refresh();
			usleep(1000);
			continue;
		}

		if(y!=m_maxheight-4)
		if(map[y + 1][x] == map[y][x] - 1){
			path.push_back(make_pair(y+1,x));
			move(y+1,x);
			addch(map[y+1][x]);
			refresh();
			usleep(1000);
			continue;
		}
		
		if(x!=m_maxwidth-1)		
		if(map[y][x+1] == map[y][x] - 1){
			path.push_back(make_pair(y,x+1));
			move(y,x+1);
			addch(map[y][x+1]);
			refresh();
			usleep(1000);
			continue;
		}

	// 	// if(!(i > path.size()) ){
	// 	// 	// y = path[path.size() - 1].first;
	// 	// 	// x = path[path.size() - 1].second;
	// 	// 	y = path[i].first;
	// 	// 	x = path[i].second;
	// 	// }
		printw("%d", i);
		refresh();
	}

	return path;
}



CMap::CMap(const std::vector<CTower> & twrs, const std::vector<CGate> & gts,
		   const std::vector<CAttacker> & attcks, const int & maxheight, const int & maxwidth, const CGate & exit):
		   towers(twrs), gates(gts), attackers(attcks), number_of_attacker(attcks.size()),
		   m_maxheight(maxheight), m_maxwidth(maxwidth),m_exit_gate(exit){}

void CMap::PrintBorders (const int & maxheight, const int & maxwidth, const char & choice){
	for(int i=0; i < maxwidth; i++){
		move(0,i);
		if(choice != 'f')
			map[0][i] = '#';
		addch('#');
	}

	for(int i=0; i < maxwidth; i++){
		move(maxheight-3,i);
		if(choice != 'f')
			map[maxheight-3][i] = '#';
		addch('#');
	}

	for(int i=0; i < maxheight-3; i++){
		move(i,0);
		if(choice != 'f')
			map[i][0] = '#';
		addch('#');
	}

	for(int i=0; i < maxheight-3; i++){
		move(i,maxwidth-1);
		if(choice != 'f')
			map[i][maxwidth-1] = '#';
		addch('#');
	}
}

void CMap::PrintTowers(){
	for(unsigned int i = 0; i < towers.size(); i++){
		move(towers[i].m_ypos, towers[i].m_xpos);
		addch(towers[i].m_tower_type);
	}
}

void CMap::PrintGates(){
	for(unsigned int i = 0; i < gates.size(); i++){
		move(gates[i].m_ypos, gates[i].m_xpos);
		addch(gates[i].m_gate_type);
	}
}


void CMap::PrintAttackers(const int & maxheight, const int & maxwidth){
	// use_default_colors();
	start_color();
	init_pair(1,COLOR_RED, COLOR_BLACK);
	m_attackers_alive = 0;
	for(unsigned int i=0; i < attackers.size(); i++){
		if(attackers[i].m_hit){
			logs.push_back(TLog(attackers[i].m_number, attackers[i].m_health));
			attackers[i].m_hit = false;
			attron(COLOR_PAIR(1));
		}

		if(attackers[i].m_health > 0){
			attackers[i].NewMove();
			m_attackers_alive++;
		}

		attroff(COLOR_PAIR(1));
	}
}

void CMap::NextFrame (const int & maxheight, const int & maxwidth){
	PrintBorders(maxheight, maxwidth, 'f');
	PrintTowers();
	PrintGates();

	for(unsigned int t = 0; t < towers.size(); t++){
		for(unsigned int a = 0; a < attackers.size(); a++){
			if(towers[t].InRange(attackers[a]) && attackers[a].m_health != 0)
				towers[t].v_targets.push_back(attackers[a]);
		}

		if(!towers[t].v_targets.empty()) //&& attackers[towers[t].ChooseTarget()].m_health != 0)
			towers[t].ShootAt(attackers[towers[t].ChooseTarget()]);

		towers[t].v_targets.clear();
	}

	PrintAttackers(maxheight, maxwidth);
}

void CMap::AddAttacker (const CGate & start){
	move(0,0);
	printw("Gate %d, select attacker type - press B for basic attacker or press A for advanced attacker", start.m_gate_ID);
	refresh();

	nodelay(stdscr, false);
	char attacker_type = getch();

	switch(attacker_type){
		case 'B':
		case 'b':
			attackers.push_back(CAttacker('&', start, number_of_attacker));
			number_of_attacker++;
			m_attackers_alive++;
			break;
		case 'A':
		case 'a':
			attackers.push_back(CAttacker('@', start, number_of_attacker));
			number_of_attacker++;
			m_attackers_alive++;
			break;
		default:
			break;
	}

	nodelay(stdscr, true);
	clear();
}

void CMap::PrintLogs(const int & maxheight){
	for(unsigned int i = 0; i < logs.size(); i++){
		move(maxheight-1,0);
		printw("Attacker %d hit, remaining health: %d", logs[i].t_number, logs[i].t_health);
		refresh();
		usleep(3000000);
	}
}
