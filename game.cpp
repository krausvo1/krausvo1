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

CGame::CGame() : m_goal(0)
{
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

bool CGame::CheckVictory(const int & maxheight, const int & maxwidth, const int & attackers_won){
	if(attackers_won == m_goal){
		move((maxheight-3)/2, maxwidth/2 - 6);
		attron(A_BOLD);
		printw("VICTORY!");
		move(((maxheight-3)/2)+1, maxwidth/2 - 6);
		printw("Play again? [Y/N]");
		attroff(A_BOLD); 
		refresh();

		return true;
	}

	return false;
}

void CGame::StartGame(const int & maxheight, const int & maxwidth, CMap map){
	nodelay(stdscr, true);
	srand(time(NULL));

	while(1){

		map.NextFrame(maxheight, maxwidth);

		move(maxheight-2,0);
		printw("Attackers alive: %d, attackes won: %d, goal: %d", 
			map.m_attackers_alive, map.m_attackers_won, m_goal);

		if(map.m_logs_on){
			printw(", logs: ON");
			map.PrintLogs(maxheight);
		}
		else
			printw(", logs: OFF");

		
		if(CheckVictory(maxheight, maxwidth, map.m_attackers_won)){
			nodelay(stdscr, false);
			while(1){
				switch(getchar()){
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
		
		// char choice = getch();
		switch(getch()){
			case 'L':
			case 'l':
				map.SwitchLogs();
				continue;
			case 'Q':
			case 'q':
				return;
			case '1':
				map.AddAttacker(map.gates[0]);
				continue;
			case '2':
				if(map.gates.size() > 2)
					map.AddAttacker(map.gates[1]);
				continue;
			case '3':
				if(map.gates.size() > 3)
					map.AddAttacker(map.gates[2]);
				continue;
			case 'S':
			case 's':
				SaveGame(map, maxheight, maxwidth);
				continue;
		}
	}	
}


void CGame::NewGame(){
	clear();
	v_towers.clear();
	v_attackers.clear();
	v_gates.clear();

	int maxheight,maxwidth;
	getmaxyx(stdscr,maxheight,maxwidth);

	CTower tower1 ('I', 10, 50);
	CTower tower2 ('I', 4, 45);
	CTower tower3 ('I', maxheight-2, 8);
	CTower tower7 ('T', maxheight-4, 7);
	
	CGate gate1 ('1', 4, maxwidth-2, 1);
	CGate gate2 ('2', maxheight-4, maxwidth-2, 2);
	CGate gate3 ('<', maxheight-4, 1, 3);

	v_towers.push_back(tower1);
	v_towers.push_back(tower2);
	v_towers.push_back(tower3);
	v_towers.push_back(tower7);

	v_gates.push_back(gate1);
	v_gates.push_back(gate2);
	v_gates.push_back(gate3);

	SetExit();
	SetGoal();

	CMap map(v_towers, v_gates, maxheight, maxwidth, m_exit_gate);
	
	StartGame(maxheight, maxwidth, map);////////////////////
	
	nodelay(stdscr, false);
	endwin();
}

bool CGame::LoadGame(ifstream & file){
	char object, type;
	int ypos, xpos;
	int maxheight, maxwidth, maxheightLoaded, maxwidthLoaded;
	int health = 0;

	getmaxyx(stdscr, maxheight, maxwidth);

	file >> maxheightLoaded >> maxwidthLoaded >> m_goal;

	if(maxheight < maxheightLoaded || maxwidth < maxwidthLoaded){
		resizeterm(maxwidthLoaded, maxwidthLoaded);
		clear();
		printw("Widen the terminal window, please.");
		refresh();
		usleep(2000000);
	}

	if(m_goal > 15 || m_goal < 5){
		clear();
		printw("Goal not in interval <5,15>, setting it to 15.");
		refresh();
		usleep(2000000);
		m_goal = 15;
	}

	int line = 0;
	clear();

	while(file >> object >> type >> ypos >> xpos){

		if(object == 'A')
			file >> health;

		if(!CreateObject(object, type, ypos, xpos, maxheightLoaded, maxwidthLoaded, health)){
			printw("Error occured while reading data from the file, line: %d", line++);
			refresh();
			usleep(1000000);
			return false;
		}
	}

	printw("Game loaded!");
	refresh();
	usleep(1900000);

	SetExit();

	CMap map(v_towers, v_gates, v_attackers, maxheightLoaded, maxwidthLoaded, m_exit_gate);
	
	StartGame(maxheightLoaded, maxwidthLoaded, map);

	nodelay(stdscr, false);
	endwin();

	return true;
}




bool CGame::CreateObject(const char & object,    const char & type, 
						 const int & ypos, 	     const int & xpos, 
						 const int & maxheight,  const int & maxwidth, const int & health){
	
	if(ypos > maxheight - 4 || ypos < 1 || xpos > maxwidth - 2 || xpos < 1){
		return false;
	}

	switch(object){
		case 'T':
			return CreateTower(type, ypos, xpos, maxwidth);
			break;
		case 'A':
			return CreateAttacker(type, ypos, xpos, maxwidth, health);
			break;
		case 'G':
			return CreateGate(type, ypos, xpos, maxwidth);
			break;
		default:
			return false;
	}
	return true;
}

bool CGame::CreateTower(const char & type, const int & ypos, 
						const int & xpos, const int & maxwidth){

	if(xpos == 1 || xpos == maxwidth - 2) return false;

	switch(type){
		case 'T':
			v_towers.push_back(CTower(type, ypos, xpos));
			break;
		case 'I':
			v_towers.push_back(CTower(type, ypos, xpos));
			break;
		default:
			return false;
	}

	return true;
}

bool CGame::CreateAttacker(const char & type, const int & ypos, 
						   const int & xpos, const int & maxwidth, const int & health){

	if(xpos == 1 || xpos == maxwidth - 2) return false;

	switch(type){
		case '@':
			v_attackers.push_back(CAttacker(type, ypos, xpos, v_attackers.size(), health));
			break;
		case '&':
			v_attackers.push_back(CAttacker(type, ypos, xpos, v_attackers.size(), health));
			break;
		default:
			return false;
	}

	return true;
}

bool CGame::CreateGate(const char & type, const int & ypos, const int & xpos,
					   										const int & maxwidth){

	if(xpos != 1 && xpos != maxwidth - 2) return false;

	v_gates.push_back(CGate(type, ypos, xpos, v_gates.size()+1));
	return true;
}

void CGame::SaveGame(const CMap & map,const int & maxheight, const int & maxwidth){
	time_t t = time(0);
	struct tm * now = localtime(&t);

	char date[80];

	strftime(date, 80, "%T %d-%m-%Y", now);
	
	ofstream outputFile;
	outputFile.open(date);
	
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

	outputFile << maxheight << " " << maxwidth << " " << m_goal << '\n';

	for(unsigned int i = 0; i < v_gates.size(); i++){
		outputFile << "G " << v_gates[i].m_gate_type << " " <<v_gates[i].m_ypos << " " << v_gates[i].m_xpos << '\n';
	}

	for(unsigned int i = 0; i < v_towers.size(); i++){
		outputFile << "T " << v_towers[i].m_tower_type << " " << v_towers[i].m_ypos << " " << v_towers[i].m_xpos << '\n';
	}

	for(unsigned int i = 0; i < map.attackers.size(); i++){
		outputFile << "A " << map.attackers[i].m_attacker_type << " " << map.attackers[i].m_ypos << " " << map.attackers[i].m_xpos << '\n';
		outputFile << map.attackers[i].m_health << '\n';
	}
}