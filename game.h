#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include "basicT.h"
#include "advancedT.h"
#include "basicA.h"
#include "advancedA.h"
#include "gate.h"
#include "map.h"
#include "border.h"

using namespace std;

class CGame{
public:
	CGame();
	~CGame();

	void NewGame();

	bool LoadGame(ifstream & file);

	void StartGame(CMap map);

	void SaveGame(const CMap & map);

	void SetExit();
	void SetGoal();
	void CheckGoal();
	void CheckResolution(const int & maxheightLoaded, const int & maxwidthLoaded);

	bool CheckVictory(const int & attackers_won);
	void AssignBorders();


	bool CreateObject				(const char & object,   const char & type, 
					 				 const int & ypos,      const int & xpos,
					 				 const int & health, 	const bool & stunned);

	bool CreateTower(const char & type, const int & ypos, const int & xpos);

	bool CreateAttacker(const char & type, const int & ypos, const int & xpos, const int & health, const bool & stunned);
	
	bool CreateGate(const char & type, const int & ypos, const int & xpos);

	bool CreateBorder(const char & type, const int & ypos, const int & xpos);


private:
	std::vector<CTower*> v_towers;
	std::vector<CGate> v_gates;
	std::vector<CAttacker*> v_attackers;
	std::vector<TBorder> v_borders;

	CGate m_exit_gate;

	int m_goal;

	int m_maxheight;
	int m_maxwidth;
};

#endif