#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <vector>
#include "attacker.h"
#include "tower.h"
#include "gate.h"
#include "map.h"

using namespace std;

class CGame{
public:
	CGame();
	~CGame(){};

	void NewGame();

	bool LoadGame(ifstream & file);

	void StartGame(CMap map);

	void SaveGame(const CMap & map);

	void SetExit();
	void SetGoal();
	bool CheckVictory(const int & attackers_won);

	bool CreateObject				(const char & object,   const char & type, 
					 				 const int & ypos,      const int & xpos,
					 				 						const int & health);

	bool CreateTower(const char & type, const int & ypos, const int & xpos);

	bool CreateAttacker(const char & type, const int & ypos, const int & xpos, const int & health);
	
	bool CreateGate(const char & type, const int & ypos, const int & xpos);

private:
	std::vector<CTower> v_towers;
	std::vector<CGate> v_gates;
	std::vector<CAttacker> v_attackers;

	CGate m_exit_gate;

	int m_goal;

	int m_maxheight;
	int m_maxwidth;
};

#endif