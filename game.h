#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
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

	void RestrictArea();

	void SetExit();
	void SetGoal();
	void CheckGoal();
	void CheckResolution(const int & maxheightLoaded, const int & maxwidthLoaded);

	bool CheckVictory(const int & attackers_won);
	void AssignBorders();
	bool AssignPaths();


	void ClearMap();

	vector<pair<int,int> > FindPath(CGate & start);

	void GenerateMap();

	void GenerateGates();

	void GenerateWall(std::pair<int,int> position);

	void GenerateTower(std::pair<int,int> position1, 
					   std::pair<int,int> position2, 
					   std::pair<int,int> position3, const int & choice);

	void GenerateBasicTower(int & placement, std::pair<int,int> position1, 
										     std::pair<int,int> position2);

	void GenerateAdvancedTower(int & placement, std::pair<int,int> position1, 
											    std::pair<int,int> position2, 
											    std::pair<int,int> position3);

	void PlaceObstacle(std::pair<int,int> position1, 
					    std::pair<int,int> position2, 
					    std::pair<int,int> position3, const int & eliminate);


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

	std::vector<std::vector<int>> m_map;

	int m_goal;

	int m_maxheight;
	int m_maxwidth;
	
	std::random_device m_seed;

	std::mt19937 m_engine;

	std::vector<std::pair<int,int>> v_restricted_area;

};

#endif