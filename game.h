#ifndef GAME_H
#define GAME_H

#include <fstream>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>
#include <cstring>

#include "map.h"

	/*!
	* Class representing most of the processes ran before the game starts
	* and some processess that run in the background  
	*/
class CGame{
public:

	/*!
	* Implicit constructor initialising few private variables of the class 
	*/
	CGame();

	/*!
	* Implicit destructor deleting towers and attackers
	*/
	~CGame();

	/*!
	* Prepare new game
	*/
	void NewGame();

	/*!
	* Load a saved game from a file
	* @param file the file from which all the necessary data needed to load a game are read
	* @return true when the game was successfully loaded, false when otherwise
	*/
	bool LoadGame						(ifstream & file);

	/*!
	* Start and run the game till the end, receive and execute players commands
	* @param map an object containing all the necessary data and methods needed to run the game
	*/
	void StartGame						(CMap & map);

	/*!
	* Write all the necessary data to a file so the exact same game could be loaded later
	* @param map an object containing few necessary data that have to be written in the save file
	*/
	void SaveGame						(const CMap & map);

	/*!
	* Store all coordinations at which a border or a tower cannot be placed
	*/
	void RestrictArea();

	/*!
	* Count the amount of money the player will start the game with
	* @param map an object to which the counted amount of money will be stored
	*/
	void CountMoney						(CMap & map);

	/*!
	* Generate the number of attackers that need to reach the exit to win the game
	*/
	void SetGoal();
	
	/*!
	* Check whether the goal loaded from a file has a valid value,
	* set it to the maximum valid number if not
	*/
	void CheckGoal();
	
	/*!
	* Compare the loaded values of maximum height and maximum width of the window
	* with current resolution
	* @param maxheightLoaded the loaded value of maximum height of the window
	* @param maxwidthLoaded the loaded value of maximum width of the window
	*/
	void CheckResolution				(const int & maxheightLoaded, 
						 				 const int & maxwidthLoaded);
	/*!
	* Check whether the player has won the game or not
	* @param attackers_won number of attackers that have reached the exit gate so far
	* @return true when the needed number of attackers have reached the exit gate,
	* false when otherwise
	*/
	bool CheckVictory					(const int & attackers_won);
	
	/*!
	* Check whether the player has lost the game or not
	* @param money amount of money the player has left to spend
	* @param attackers_alive number of alive attackers
	* @return true when the player has ran out of money and attackers, false when otherwise
	*/
	bool CheckGameOver					(const int & money, 
					 					 const int & attackers_alive);

	/*!
	* Assign all borders to towers so their blind spots can be marked afterwards
	*/
	void AssignBorders();
	
	/*!
	* Assign attackers loaded from a file to a path which they will follow
	* @return true when all attackers have been successfully assigned to their paths, 
	* false when otherwise
	*/
	bool AssignPaths();

	/*!
	* Empty the 2D vector conatining values of each node so that new values for new paths
	* can be count and mark the coordinates of already created borders and towers
	*/
	void ClearMap();

	/*!
	* Find the shortest path from the given gate to the exit gate considering already created
	* borders and towers (using Dijkstra's algorithm)
	* @param start the gate from which the shortest path will be calculated
	* @return a vector of pairs containing coordinates of the shortest path from the exit gate to the 
	* given gate
	*/
	std::vector< std::pair<int,int> > FindPath		(CGate & start);

	/*!
	* Generate a new map with obstacles
	*/
	void GenerateMap();

	/*!
	* Generate the gates to be placed in a new map
	*/
	void GenerateGates();

	/*!
	* Generate a wall (consisting of borders) to be placed in a new map
	* @param position a certain position of the path from which the placement position
	* of the wall will be calculated
	*/
	void GenerateWall					(std::pair<int,int> position);

	/*!
	* Generate a tower to be placed in a new map
	* @param position1 a certain position of the path from which the placement position
	* of the tower will be calculated
	* @param position2 another position of the path which is used to check 
	* whether the placement position of the basic tower would alter the path
	* @param position3 another position of the path which is used to check 
	* whether the placement position of the advanced tower would alter the path
	* @param choice number deciding which type of tower will be generated
	*/
	void GenerateTower					(std::pair<int,int> position1, 
					  					 std::pair<int,int> position2, 
					  					 std::pair<int,int> position3, 
					  					 const int & choice);

	/*!
	* Generate a basic tower to be placed in a new map
	* @param placement generated placement position for new basic tower
	* @param position1 a certain position of the path from which the placement position
	* of the tower will be calculated
	* @param position2 another position of the path which is used to check 
	* whether the placement position of the basic tower would alter the path
	*/
	void GenerateBasicTower				(int & placement, 
										 std::pair<int,int> position1, 
			    		   				 std::pair<int,int> position2);

	/*!
	* Generate an advanced tower to be placed in a new map
	* @param placement generated placement position for new advanced tower
	* @param position1 a certain position of the path from which the placement position
	* of the tower will be calculated
	* @param position3 another position of the path which is used to check 
	* whether the placement position of the advanced tower would alter the path
	*/
	void GenerateAdvancedTower			(int & placement, 
							   			 std::pair<int,int> position1, 
							   			 std::pair<int,int> position3);

	/*!
	* Choose which type of obstacle (wall/tower) will be placed in a new map
	* @param position1 a certain position of the path from which the placement position
	* of the tower will be calculated
	* @param position2 another position of the path which is used to check 
	* whether the placement position of a basic tower would alter the path
	* @param position3 another position of the path which is used to check 
	* whether the placement position of a advanced tower would alter the path
	* @param eliminate decides which type of obstacle (wall/tower) will not be created
	* during this call
	*/
	void PlaceObstacle					(std::pair<int,int> position1, 
					  					 std::pair<int,int> position2, 
					  					 std::pair<int,int> position3, 
					  					 const int & eliminate);
	
	/*!
	* Choose which kind of object (tower/attacker/gate/border) was loaded from a file and 
	* proceed on creating it 
	* @param object decides which kind of object will be created
	* @param type decides of which type the newly created object will be
	* @param ypos vertical position of the newly created object
	* @param xpos horizontal position of the newly created object
	* @param health amount of health the newly created attacker will have (if the created 
	* object is actually an attacker)
	* @param stunned decides whether the newly created attacker is stunned for the next round or not
	* (if the created object is actually an attacker)
	* @return true when the object was created successfully, false when otherwise
	*/
	bool CreateObject					(const char & object,   
					 					 const char & type, 
	 				 					 const int & ypos,      
	 				 					 const int & xpos,
	 				 					 const int & health, 	
	 				 					 const bool & stunned);

	/*!
	* Create a new tower based on data gathered from a file
	* @param type decides of which type the newly created tower will be
	* @param ypos vertical position of the newly created tower
	* @param xpos horizontal position of the newly created tower
	* @return true when the tower was created successfully, false when otherwise
	*/
	bool CreateTower					(const char & type, 
										 const int & ypos, 
										 const int & xpos);
	/*!
	* Create a new attacker based on data loaded from a file
	* @param type decides of which type the newly created attacker will be
	* @param ypos vertical position of the newly created attacker
	* @param xpos horizontal position of the newly created attacker
	* @param health amount of health the newly created attacker will have 
	* @param stunned decides whether the newly created attacker is stunned for the next round or not
	* @return true when the attacker was created successfully, false when otherwise
	*/
	bool CreateAttacker					(const char & type, 
										 const int & ypos, 
									 	 const int & xpos, 
										 const int & health, 
										 const bool & stunned);
	/*!
	* Create a new gate based on data gathered from a file
	* @param type decides of which type the newly created gate will be
	* @param ypos vertical position of the newly created gate
	* @param xpos horizontal position of the newly created gate
	* @return true when the gate was created successfully, false when otherwise
	*/
	bool CreateGate						(const char & type, 
										 const int & ypos, 
										 const int & xpos);
	/*!
	* Create a new border based on data gathered from a file
	* @param type decides of which type the newly created border will be (only one type of border currently)
	* @param ypos vertical position of the newly created border
	* @param xpos horizontal position of the newly created border
	* @return true when the border was created successfully, false when otherwise
	*/
	bool CreateBorder					(const char & type, 
					 					 const int & ypos,
					 					 const int & xpos);

private:
	
	//! vector containing all attackers in the game
	std::vector<CTower*> v_towers;

	//! vector containing all towers in the game
	std::vector<CGate> v_gates;

	//! vector containing all attackers in the game
	std::vector<CAttacker*> v_attackers;

	//! vector containing all borders in the game
	std::vector<TBorder> v_borders;

	//! the exit gate to which all attackers are heading
	CGate m_exit_gate;

	//! 2D vector containing valued nodes used to find the shortest path for the attackers
	std::vector<std::vector<int>> m_map;

	//! number of attackers that need to reach the exit gate in order to win the game
	int m_goal;

	//! maximum height of the window
	int m_maxheight;

	//! maximum width of the window
	int m_maxwidth;

	//! seed used initialise random number engine
	std::random_device m_seed;
	
	//! random number engine
	std::mt19937 m_engine;

	//! vector containing coordinates at which towers or borders cannot be placed
	std::vector< std::pair<int,int> > v_restricted_area;

	//! number of attackers that have already escaped the map through the exit gate
	int m_attackers_won;
};

#endif