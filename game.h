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
	CGame(){};
	~CGame(){};

	void NewGame();

	bool LoadGame(ifstream & file);

	void StartGame(const int & maxheight, const int & maxwidth, CMap map);

	void SetExit();

	bool CreateObject				(const char & object, const char & type, 
					 				 const int & ypos,    const int & xpos,
					 									  const int & maxwidth);

	bool CreateTower(const char & type, const int & ypos, const int & xpos);

	bool CreateAttacker(const char & type, const int & ypos, const int & xpos);
	
	bool CreateGate(const char & type, const int & ypos, const int & xpos,
					const int & maxwidth);

private:
	std::vector<CTower> v_towers;
	std::vector<CGate> v_gates;
	std::vector<CAttacker> v_attackers;

	CGate m_exit_gate;
};

#endif