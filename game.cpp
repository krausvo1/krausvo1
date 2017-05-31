#include <curses.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <random>
#include "tower.h"
#include "gate.h"
#include "attacker.h"
#include "map.h"
#include "game.h"

CGame::CGame() : m_goal(0), m_maxheight(0), m_maxwidth(0){
	srand(time(NULL));
}

CGame::~CGame(){
	for(unsigned int i = 0; i < v_towers.size(); i++)
          delete v_towers[i];
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

	AssignBorders();

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


void CGame::NewGame(){
	clear();
	v_towers.clear();
	v_attackers.clear();
	v_gates.clear();
	v_borders.clear();

	getmaxyx(stdscr, m_maxheight, m_maxwidth);
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<int> rozmezi_vyska(m_maxheight - 10,m_maxheight - 6);
	std::uniform_int_distribution<int> rozmezi_sirka(1, m_maxwidth-4);
	std::uniform_int_distribution<int> rozmezi(6, 13);
	std::uniform_int_distribution<int> rozsudek(1, 10);

	
	CGate gate1 ('1', 4, m_maxwidth-2, 1);
	CGate gate2 ('2', 8, m_maxwidth-2, 2);
	CGate gate3 ('<', 4, 1, 3);

	v_towers.push_back(new CAdvancedTower (8, 50));
	v_towers.push_back(new CAdvancedTower (4, 45));
	v_towers.push_back(new CBasicTower (3, 25));
	v_towers.push_back(new CAdvancedTower (m_maxheight-4, 8));
	v_towers.push_back(new CBasicTower (m_maxheight-4, 7));

	v_gates.push_back(gate1);
	v_gates.push_back(gate2);
	v_gates.push_back(gate3);

	// v_borders.push_back(TBorder(4,25));
	// v_borders.push_back(TBorder(3,26));
	// v_borders.push_back(TBorder(4,46));


	int random = rozmezi(eng);
	for(int i = 1; i < m_maxwidth-6;i++){
		if(i == random){
			int dylka_zdi = rozmezi_vyska(eng);
			if(rozsudek(eng) % 2 == 0){
				for(int j = 1; j < dylka_zdi; j++){
					v_borders.push_back(TBorder(j, i));
				}
			}
			else
				for(int j = m_maxheight - 4; j > m_maxheight - 4 - dylka_zdi; j--)
					v_borders.push_back(TBorder(j, i));

			
			
			random += rozmezi(eng);
		}
	}


	SetExit();
	SetGoal();

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
	usleep(1900000);

	SetExit();

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