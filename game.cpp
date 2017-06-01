#include <curses.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include "tower.h"
#include "gate.h"
#include "attacker.h"
#include "map.h"
#include "game.h"

CGame::CGame() : m_goal(0), m_maxheight(0), m_maxwidth(0), m_engine(m_seed()){
	srand(time(NULL));
}

CGame::~CGame(){
	for(unsigned int i = 0; i < v_towers.size(); i++)
          delete v_towers[i];

   for(unsigned int i = 0; i < v_attackers.size(); i++)
      delete v_attackers[i];
}

void CGame::SetExit(){
	for(unsigned int i = 0; i < v_gates.size(); i++){
		if(v_gates[i].m_gate_type == '<'){
			m_exit_gate.m_ypos = v_gates[i].m_ypos;
			m_exit_gate.m_xpos = v_gates[i].m_xpos;
			m_exit_gate.m_gate_ID = v_gates[i].m_gate_ID;
		}
	}
}

void CGame::SetGoal(){
	m_goal = rand() % 15 + 5;
}

void CGame::AssignBorders(){
	for(unsigned int i = 0; i < v_towers.size(); i++){
			v_towers[i]->v_borders = v_borders;
			v_towers[i]->CheckRange();
	}
}

bool CGame::CheckVictory(const int & attackers_won){
	if(attackers_won == m_goal){
		move((m_maxheight-3)/2, m_maxwidth/2 - 6);
		attron(A_BOLD);
		printw("VICTORY!");
		move(((m_maxheight-3)/2)+1, m_maxwidth/2 - 6);
		printw("Play again? [Y/N]");
		attroff(A_BOLD); 
		refresh();

		return true;
	}

	return false;
}

void CGame::StartGame(CMap map){
	nodelay(stdscr, true);

	while(1){

		map.NextFrame();

		move(m_maxheight-2,0);
		printw("Attackers alive: %d, attackes won: %d, goal: %d", 
			map.m_attackers_alive, map.m_attackers_won, m_goal);

		if(map.m_logs_on){
			printw(", logs: ON");
			map.PrintLogs();
		}
		else
			printw(", logs: OFF");

		
		if(CheckVictory(map.m_attackers_won)){
			nodelay(stdscr, false);
			while(1){
				switch(getch()){
					case 'Y':
					case 'y':
						NewGame();
						return;
					case 'N':
					case 'n':
						return;
					default:
						continue;
				};
			}
		}
		
		refresh();
		clear();
		usleep(300000);
		
		switch(getch()){
			case 'L':
			case 'l':
				map.SwitchLogs();
				continue;
			case 'Q':
			case 'q':
				return;
			case '1':
				map.AddAttacker(map.v_gates[0]);
				continue;
			case '2':
				if(map.v_gates.size() > 2)
					map.AddAttacker(map.v_gates[1]);
				continue;
			case '3':
				if(map.v_gates.size() > 3)
					map.AddAttacker(map.v_gates[2]);
				continue;
			case 'S':
			case 's':
				SaveGame(map);
				continue;
		}
	}	
}

void CGame::ClearMap(){
	m_map.resize(m_maxheight);
	for(int i = 0; i < m_maxheight; i++){
		m_map[i].resize(m_maxwidth);
	}

	for(unsigned int i = 0; i < v_gates.size(); i++){
		for(int i = 0; i < m_maxheight; i++)
			for(int j = 0; j < m_maxwidth; j++)
				m_map[i][j] = 0;
	}

	for(int i=0; i < m_maxwidth; i++){
		move(0,i);
		m_map[0][i] = -8;
		addch('#');
	}

	for(int i=0; i < m_maxwidth; i++){
		move(m_maxheight-3,i);
		m_map[m_maxheight-3][i] = -8;
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,0);
		m_map[i][0] = -8;
		addch('#');
	}

	for(int i=0; i < m_maxheight-3; i++){
		move(i,m_maxwidth-1);
		m_map[i][m_maxwidth-1] = -8;
		addch('#');
	}

	for(unsigned int i = 0; i < v_towers.size(); i++){
		m_map[v_towers[i]->m_ypos][v_towers[i]->m_xpos] = -8;
		move(v_towers[i]->m_ypos, v_towers[i]->m_xpos);
		addch(v_towers[i]->m_tower_type);
		refresh();
	}

	for(unsigned int i = 0; i < v_borders.size(); i++){
		m_map[v_borders[i].t_ypos][v_borders[i].t_xpos] = -8;
		move(v_borders[i].t_ypos, v_borders[i].t_xpos);
		addch('#');
		refresh();
	}

	for(unsigned int i = 0; i <v_gates.size(); i++){
		move(v_gates[i].m_ypos, v_gates[i].m_xpos);
		addch(v_gates[i].m_gate_type);
		refresh();
	}
}

vector<pair<int,int> > CGame::FindPath(CGate & start){
	std::vector<pair<int, int> > temp; //uchovává souřadnice
	// clear();
	int y = start.m_ypos;
	int x = start.m_xpos;

	// mvprintw(0,0,"START: %d,%d vs. max: %d, %d", y, x, m_maxheight, m_maxwidth);
	// refresh();
	// usleep(2000000);

	temp.push_back(make_pair(y,x));
	// clear();
	// move(y,x);
	// addch('S');
	// move(m_exit_gate.m_ypos,m_exit_gate.m_xpos);
	// addch('E');
	// refresh();
	// usleep(100000);

	// printw("start pos: %d,%d", y,x);
	// printw("\nend pos: %d,%d", m_exit_gate.m_ypos,m_exit_gate.m_xpos);
	// refresh();
	// usleep(1000000);

	//očísluj
	while(temp[0].first != m_exit_gate.m_ypos || temp[0].second != m_exit_gate.m_xpos){ 

		if(y != 0 && m_map[y - 1][x] == 0){//y != 0 kvůli pozici -1
			temp.push_back(make_pair(y - 1, x));
			m_map[y - 1][x] = m_map[y][x] + 1;
			// move(y-1,x);
			// addch(m_map[y-1][x]);
			// refresh();
			// usleep(1900);
		}
		
		if(x != 0 && m_map[y][x - 1] == 0){//x != 0 kvůli pozici -1
			temp.push_back(make_pair(y, x - 1));
			m_map[y][x - 1] = m_map[y][x] + 1;
			// move(y,x-1);
			// addch(m_map[y][x-1]);
			// refresh();
			// usleep(1900);
		}

		if(y != m_maxheight - 4 && m_map[y + 1][x] == 0){
			temp.push_back(make_pair(y + 1, x));
			m_map[y + 1][x] = m_map[y][x] + 1;
			// move(y+1,x);
			// addch(m_map[y+1][x]);
			// refresh();
			// usleep(1900);
		}

		if(x != m_maxwidth - 1 && m_map[y][x + 1] == 0){
			temp.push_back(make_pair(y, x + 1));
			m_map[y][x + 1] = m_map[y][x] + 1;
			// move(y,x+1);
			// addch(m_map[y][x+1]);
			// refresh();
			// usleep(1900);
		}

		if(!temp.empty())
			temp.erase(temp.begin());

		y = temp[0].first;
		x = temp[0].second;
		// clear();
		// mvprintw(0,0,"PRVNI, %dvs%d, %dvs%d", temp[0].first,m_exit_gate.m_ypos, temp[0].second,m_exit_gate.m_xpos);
		// refresh();
		// usleep(190000);
	}

	m_map[start.m_ypos][start.m_xpos] = 0;

	 
	//najdi cestu zpět, z cíle do počátku, path[0] = cíl, path[max] = počátek
	vector<pair<int,int> > path;

	y = m_exit_gate.m_ypos;
	x = m_exit_gate.m_xpos;
	
	path.push_back(make_pair(y, x));
	// clear();
	// printw("POnd pos: %d,%d", y,x);
	// refresh();
	// usleep(1000000);
	
	int i = 0;

	// for(int i = 0; i < m_m_maxheight-4; i++)
	// 	for(int j = 0; j < m_maxwidth; j++){
	// 		mvprintw(i,j,"%c",map[i][j]);
	// 		refresh();
	// 	}

	// usleep(190000);
	// clear();
	// refresh();
	while(!(path[path.size() - 1].first == start.m_ypos && path[path.size() - 1].second == start.m_xpos)){ 
		if(path.size() > 1){
			i++;
			// y = path[path.size() - 1].first;
			// x = path[path.size() - 1].second;
			y = path[path.size() - 1].first;
			x = path[path.size() - 1].second;
			// mvprintw(0,0,"%d,%d",y,x);
			// refresh();
			// usleep(10000);

		}

		if(y != 0 && m_map[y - 1][x] == m_map[y][x] - 1){
			path.push_back(make_pair(y - 1, x));
			// move(y-1,x);
			// addch(m_map[y-1][x]);
			// refresh();
			// usleep(10000);
			continue;
		}

		if(x != 0 && m_map[y][x - 1] == m_map[y][x] - 1){
			path.push_back(make_pair(y, x - 1));
			// move(y,x-1);
			// addch(m_map[y][x-1]);
			// refresh();
			// usleep(10000);
			continue;
		}

		if(y != m_maxheight - 4 && m_map[y + 1][x] == m_map[y][x] - 1){
			path.push_back(make_pair(y + 1, x));
			// move(y+1,x);
			// addch(m_map[y+1][x]);
			// refresh();
			// usleep(10000);
			continue;
		}
		
		if(x != m_maxwidth - 1 && m_map[y][x + 1] == m_map[y][x] - 1){
			path.push_back(make_pair(y, x + 1));
			// move(y,x+1);
			// addch(m_map[y][x+1]);
			// refresh();
			// usleep(10000);
			continue;
		}
	}

	return path;
}

void CGame::GenerateMap(){
	ClearMap();

	std::uniform_int_distribution<int> gen(5, 7); //n-tá souřadnice
	
	for(unsigned int i = 0; i < v_gates.size() - 1; i++)
		for(int j = 1; j < m_maxwidth - 1; j++){
			v_gates[i].path.push_back(std::make_pair(v_gates[i].m_ypos, j)); //přiřadí cestu zleva doprava, rovně
		}

	for(int choice = 2; choice > 0; choice--){
		for(unsigned int i = 0; i < v_gates.size() - 1; i++){
			for(int position_n = v_gates[i].path.size() - 4; position_n > 3; position_n -= gen(m_engine)){
				if(position_n < 3)
					break;
				mvprintw(0,0,"krok: %d",position_n);
				refresh();
				ClearMap();
				v_gates[i].path = FindPath(v_gates[i]);
				// InsertObstacle(v_gates[i].path[position_n], v_gates[i].path[position_n - 1], 
				// 			   v_gates[i].path[position_n - 2], choice);//na jakou souřadnici
				clear();
			}

			printw("Dalsi brana");
			refresh();
			// usleep(1000000);
		}
	}

	for(unsigned int i = 0; i < v_gates.size() - 1; i++){
		ClearMap();
		v_gates[i].path = FindPath(v_gates[i]);
	}

}

void CGame::GenerateWall(const int & dylka_zdi, const int & start_y, std::pair<int,int> position){
	mvprintw(0,0,"WALL INSERTED: %d,%d",start_y, position.second);
	refresh();
	// usleep(1000000);
	for(int i = 0; i < dylka_zdi; i++){
		if(start_y + i > 0 && start_y + i < m_maxheight - 4){
			m_map[start_y + i][position.second] = -8;
			v_borders.push_back(TBorder(start_y + i, position.second));

		}
	}
}

void CGame::GenerateBasicTower(int & placement, std::pair<int,int> position1, 
												std::pair<int,int> position2){
	for(;position1.first + placement < 1; placement++);
	for(;position1.first + placement > m_maxheight - 4; placement--);

	if(position2.second == position1.second)
		position1.second++;

	printw("BASIC TOWER INSERTED: %d,%d", position1.first + placement, position1.second);
	refresh();
	// usleep(1000000);

	m_map[position1.first + placement][position1.second] = 'T';
	v_towers.push_back(new CBasicTower (position1.first + placement, position1.second));
}

void CGame::GenerateAdvancedTower(int & placement, std::pair<int,int> position1, 
												   std::pair<int,int> position2, 
												   std::pair<int,int> position3){
	for(;position1.first + placement < 1; placement++);
	for(;position1.first + placement > m_maxheight - 4; placement--);

	if(position3.second == position1.second)
		position1.second++;

	printw("ADV TOWER INSERTED: %d,%d", position1.first + placement, position1.second);
	refresh();
	// usleep(1000000);
	
	m_map[position1.first + placement][position1.second] = 'I';
	v_towers.push_back(new CAdvancedTower (position1.first + placement, position1.second));
}

void CGame::InsertObstacle(std::pair<int,int> position1, 
						   std::pair<int,int> position2, 
						   std::pair<int,int> position3, const int & eliminate){
	std::uniform_int_distribution<int> dist(1, 2); //jakej objekt

	std::uniform_int_distribution<int> rozmezi_zed(3, 8);
	int dylka_zdi = rozmezi_zed(m_engine);

	std::uniform_int_distribution<int> rozmezi_start_y(position1.first - dylka_zdi, position1.first);
	int start_y = rozmezi_start_y(m_engine);

	if(position1.second < 2)
		position1.second = 2;

	move(0,0);
	int choice = dist(m_engine);

	if(choice == eliminate)
		choice = dist(m_engine);

	if(choice != eliminate)
		switch(choice){
			case 1://WALL
				GenerateWall(dylka_zdi, start_y, position1);
				break;
			case 2:// T
				GenerateTower(position1, position2, position3, dist(m_engine));
	}

}

void CGame::GenerateTower(std::pair<int,int> position1, 
					   	  std::pair<int,int> position2, 
					      std::pair<int,int> position3, const int & choice){
	std::uniform_int_distribution<int> rozmezi_basic(-1, 1);
	int basic_pos = rozmezi_basic(m_engine);
	while(basic_pos == 0)
		basic_pos = rozmezi_basic(m_engine);

	std::uniform_int_distribution<int> rozmezi_advanced(-2, 2);
	int advanced_pos = rozmezi_advanced(m_engine);
	while(advanced_pos == 0)
		advanced_pos = rozmezi_advanced(m_engine);

	switch(choice){
		case 1:
			GenerateAdvancedTower(advanced_pos, position1, position2, position3);
			break;
		case 2:
		case 3:
			GenerateBasicTower(basic_pos, position1, position2);
			break;
	}
}

void CGame::NewGame(){
	clear();
	v_towers.clear();
	v_attackers.clear();
	v_gates.clear();
	v_borders.clear();

	getmaxyx(stdscr, m_maxheight, m_maxwidth);

	std::uniform_int_distribution<int> rozmezi_vyska(m_maxheight - 20,m_maxheight - 13);
	std::uniform_int_distribution<int> vyska(1, m_maxheight - 4);
	std::uniform_int_distribution<int> rozmezi_sirka(1, m_maxwidth-4);
	std::uniform_int_distribution<int> rozmezi(6, 13);
	std::uniform_int_distribution<int> rozsudek(1, 10);
	std::uniform_int_distribution<int> pocet_bran(2, 3);

	if(pocet_bran(m_engine) == 2){
		std::uniform_int_distribution<int> prvni_pulka(1, (m_maxheight - 4) / 2);
		std::uniform_int_distribution<int> druha_pulka((m_maxheight - 4) / 2, m_maxheight - 4);

		CGate gate1 ('1', prvni_pulka(m_engine), m_maxwidth - 2, 1);
		CGate gate2 ('2', druha_pulka(m_engine), m_maxwidth - 2, 2);
		CGate gate3 ('<', vyska(m_engine), 1, 3);

		v_gates.push_back(gate1);
		v_gates.push_back(gate2);
		v_gates.push_back(gate3);

		m_exit_gate = gate3;
	}
	else{
		std::uniform_int_distribution<int> prvni_tretina(1, (m_maxheight - 4) / 3);
		std::uniform_int_distribution<int> druha_tretina((m_maxheight - 4) / 3, (m_maxheight - 4) * 2/3);
		std::uniform_int_distribution<int> treti_tretina((m_maxheight - 4) * 2/3, m_maxheight - 4);

		CGate gate1 ('1', prvni_tretina(m_engine), m_maxwidth - 2, 1);
		CGate gate2 ('2', druha_tretina(m_engine), m_maxwidth - 2, 2);
		CGate gate3 ('3', treti_tretina(m_engine), m_maxwidth - 2, 3);
		CGate gate4 ('<', vyska(m_engine), 1, 3);

		v_gates.push_back(gate1);
		v_gates.push_back(gate2);
		v_gates.push_back(gate3);
		v_gates.push_back(gate4);

		m_exit_gate = gate4;
	}


	// v_gates.push_back(gate1);
	// v_gates.push_back(gate2);
	// v_gates.push_back(gate3);

	// m_exit_gate = gate3;

	// v_towers.push_back(new CAdvancedTower (8, 50));
	// v_towers.push_back(new CAdvancedTower (4, 45));
	// v_towers.push_back(new CBasicTower (3, 25));
	// v_towers.push_back(new CAdvancedTower (m_maxheight-4, 8));
	// v_towers.push_back(new CBasicTower (m_maxheight-4, 7));

	// v_borders.push_back(TBorder(3,62));
	// v_borders.push_back(TBorder(1,63));
	// v_borders.push_back(TBorder(2,60));


	// int random = rozmezi(m_engine);
	// for(int i = 1; i < m_maxwidth-6;i++){
	// 	if(i == random){
	// 		int dylka_zdi = rozmezi_vyska(m_engine);
	// 		if(rozsudek(m_engine) % 2 == 0){
	// 			for(int j = 0; j < dylka_zdi; j++){
	// 				v_borders.push_back(TBorder(j, i));
	// 			}
	// 		}
	// 		else
	// 			for(int j = m_maxheight - 4; j > m_maxheight - 4 - dylka_zdi; j--)
	// 				v_borders.push_back(TBorder(j, i));

			
			
	// 		random += 1 + rozmezi(m_engine);
	// 	}
	// }
	GenerateMap();

	SetExit();
	SetGoal();
	AssignBorders();


	CMap map(v_towers, v_gates, v_borders, m_maxheight, m_maxwidth, m_exit_gate);
	
	StartGame(map);
	
	nodelay(stdscr, false);
	endwin();
}

bool CGame::LoadGame(ifstream & file){
	getmaxyx(stdscr, m_maxheight, m_maxwidth);

	int maxheightLoaded, maxwidthLoaded;
	if(!(file >> maxheightLoaded >> maxwidthLoaded >> m_goal)){
		printw("Header of the file is invalid, please check the first line.");
		refresh();
		usleep(2000000);
		return false;
	}

	CheckResolution(maxheightLoaded, maxwidthLoaded);
	CheckGoal();

	clear();

	char object, type;
	int ypos, xpos;
	int health = 0, stunned = false;
	int line_number = 1;
	while(file >> object >> type >> ypos >> xpos){
		if(object == 'A' && !(file >> health >> stunned)){
			line_number++;
			break;
		}

		line_number++;

		if(!CreateObject(object, type, ypos, xpos, health, stunned)){
			break;
		}
	}

	if(!file.eof()){
		printw("Error occured while reading data from the file, line: %d ", line_number);
		refresh();
		usleep(2400000);
		return false;
	}

	printw("Game loaded!");
	refresh();
	usleep(190000);

	SetExit();

	ClearMap();

	for(unsigned int i = 0; i < v_towers.size(); i++){
		m_map[v_towers[i]->m_ypos][v_towers[i]->m_xpos] = -8;
	}

	for(unsigned int i = 0; i < v_borders.size(); i++){
		m_map[v_borders[i].t_ypos][v_borders[i].t_xpos] = -8;
	}

	for(unsigned int i = 0; i < v_gates.size(); i++){
		if(v_gates[i].m_gate_type != '<')
			v_gates[i].path = FindPath(m_exit_gate);
	}

	AssignBorders();


	CMap map(v_towers, v_gates, v_attackers, v_borders, m_maxheight, m_maxwidth, m_exit_gate);
	
	StartGame(map);

	nodelay(stdscr, false);
	endwin();

	return true;
}

bool CGame::CreateObject(const char & object,    const char & type, 
						 const int & ypos, 	     const int & xpos, 
						 const int & health, 	 const bool & stunned){
	
	if(ypos > m_maxheight - 4 || ypos < 1 || xpos > m_maxwidth - 2 || xpos < 1){
		return false;
	}

	switch(object){
		case 'T':
			return CreateTower(type, ypos, xpos);
			break;
		case 'A':
			return CreateAttacker(type, ypos, xpos, health, stunned);
			break;
		case 'G':
			return CreateGate(type, ypos, xpos);
			break;
		case 'B':
			return CreateBorder(type, ypos, xpos);
			break;
		default:
			return false;
	}
	return true;
}

bool CGame::CreateTower(const char & type, const int & ypos, const int & xpos){

	if(xpos == 1 || xpos == m_maxwidth - 2) return false;

	switch(type){
		case 'T':
			v_towers.push_back(new CBasicTower(ypos, xpos));
			break;
		case 'I':
			v_towers.push_back(new CAdvancedTower(ypos, xpos));
			break;
		default:
			return false;
	}

	return true;
}

bool CGame::CreateAttacker(const char & type, const int & ypos, const int & xpos,  const int & health, const bool & stunned){

	if(xpos == 1 || xpos == m_maxwidth - 2) return false;

	switch(type){
		case '&':
			v_attackers.push_back(new CBasicAttacker(ypos, xpos, v_attackers.size(), health, stunned));
			break;
		case '@':
			v_attackers.push_back(new CAdvancedAttacker(ypos, xpos, v_attackers.size(), health));
			break;
		default:
			return false;
	}

	return true;
}

bool CGame::CreateGate(const char & type, const int & ypos, const int & xpos){   										
	if((xpos != 1 && xpos != m_maxwidth - 2) || 
	   (type != '<' && type != '1' && type != '2' && type != '3'))
		return false;

	v_gates.push_back(CGate(type, ypos, xpos, v_gates.size()+1));

	if(type != '<')
		m_exit_gate = v_gates[v_gates.size() - 1];

	return true;
}

bool CGame::CreateBorder(const char & type, const int & ypos, const int & xpos){
	if(xpos == 1 || xpos == m_maxwidth - 2 || type != '#') return false;

	v_borders.push_back(TBorder(ypos, xpos));
	return true;
}

void CGame::SaveGame(const CMap & map){
	time_t t = time(0);
	struct tm * now = localtime(&t);

	char date[80];

	strftime(date, 80, "%H%M%S-%d%m%Y", now);
	
	ofstream outputFile(date);
	
	clear();
	move(0,0);
	
	if(outputFile.is_open())
		printw("Game saved! File name is \"%s\"", date);
	else
		printw("Error occured while saving the game!");

	move(1,0);
	printw("Press ANY key to continue");
	refresh();

	nodelay(stdscr, false);
	getch();
	nodelay(stdscr, true);
	clear();

	outputFile << m_maxheight << " " << m_maxwidth << " " << m_goal << '\n';

	for(unsigned int i = 0; i < v_gates.size(); i++){
		outputFile << "G " << v_gates[i].m_gate_type << " " <<v_gates[i].m_ypos << " " << v_gates[i].m_xpos << '\n';
	}

	for(unsigned int i = 0; i < v_towers.size(); i++){
		outputFile << "T " << v_towers[i]->m_tower_type << " " << v_towers[i]->m_ypos << " " << v_towers[i]->m_xpos << '\n';
	}

	for(unsigned int i = 0; i < map.v_attackers.size(); i++){
		if(map.v_attackers[i]->m_attacker_type == 'X')
			continue;

		outputFile << "A " << map.v_attackers[i]->m_attacker_type << " " << map.v_attackers[i]->m_ypos << " " << map.v_attackers[i]->m_xpos << '\n';
		outputFile << map.v_attackers[i]->m_health << " " << map.v_attackers[i]->m_stunned << '\n';

	}

	for(unsigned int i = 0; i < map.v_borders.size(); i++){
		outputFile << "B # " << map.v_borders[i].t_ypos << " " << map.v_borders[i].t_xpos << '\n';
	}
}

void CGame::CheckGoal(){
	if(m_goal > 19 || m_goal < 5){
		clear();
		printw("Goal out of interval <5,19>, setting it to 19.");
		refresh();
		usleep(2000000);
		m_goal = 19;
	}
}

void CGame::CheckResolution(const int & maxheightLoaded, const int & maxwidthLoaded){
		if(m_maxheight < maxheightLoaded || m_maxwidth < maxwidthLoaded){
		resizeterm(maxwidthLoaded, maxwidthLoaded);
		clear();
		printw("Widen the terminal window, please.");
		refresh();
		usleep(2000000);
	}

	m_maxheight = maxheightLoaded;
	m_maxwidth = maxwidthLoaded;
}