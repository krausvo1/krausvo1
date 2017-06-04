#ifndef MAP_H
#define MAP_H

#include "basicT.h"
#include "advancedT.h"
#include "basicA.h"
#include "advancedA.h"
#include "log.h"

	/*!
	* Class representing a map and some processes of the game
	*/
class CMap{
public:

	/*!
	* Constructor initialising a new map
	* @param towers vector of towers which are in the game
	* @param gates vector of gates which are in the game
	* @param borders vector of borders which are int the game
	* @param maxheight maximum height of the window
	* @param maxwidth maximum width of the window
	* @param exit the exit gate
	*/
	CMap								(const std::vector<CTower*> & towers, 
										 const std::vector<CGate> & gates, 
										 const std::vector<TBorder> & borders, 
										 const unsigned int & maxheight, 
										 const unsigned int & maxwidth, 
										 const CGate & exit);
	

	/*!
	* Constructor initialising a map loaded from a file
	* @param attackers vector of alive attackers in the game
	* @param towers vector of towers which are in the game
	* @param gates vector of gates which are in the game
	* @param borders vector of borders which are int the game
	* @param maxheight maximum height of the window
	* @param maxwidth maximum width of the window
	* @param exit the exit gate
	* @param attackers_won number of attackers that have already reached the exit gate
	* @param money amount of money the player has left to spend
	*/							
	CMap								(const std::vector<CAttacker*> & attackers, 
										 const std::vector<CTower*> & towers, 
										 const std::vector<CGate> & gates,
										 const std::vector<TBorder> & borders, 
										 const unsigned int & maxheight, 
										 const unsigned int & maxwidth,
										 const CGate & exit, 
										 const int & attackers_won, 
										 const int & money);

	/*!
	* Implicit destructor that deletes attackers which the player has added to the game
	*/
	~CMap();


	/*!
	* Run a test round to gather information about damage taken 
	* along the way from specific gate towards the exit
	* @param gate_ID ID number of the gate through which the testing attacker enters the game
	* @return amount of damage taken along the way towards the exit
	*/
	int TestRound						(const int & gate_ID);


	/*!
	* Run next round of the game
	*/
	void NextRound();


	/*!
	* Check for all collisions happening between towers and attackers
	*/
	void CheckCollisions();

	/*!
	* Check whether the given attacker gives a way to another attacker or not
	* @param attacker attacker that is being check on
	*/
	void CheckPriorities				(CAttacker & attacker);

	/*!
	* Print all borders on the screen
	*/
	void PrintBorders() const;
	
	/*!
	* Print all towers on the screen
	*/
	void PrintTowers() const;
	
	/*!
	* Print all gates on the screen
	*/
	void PrintGates() const;
	
	/*!
	* Print alive or now dying attackers on the screen
	*/
	void PrintAttackers();
	
	/*!
	* Print all logs of happening collisions on the screen
	*/
	void PrintLogs();
	
	/*!
	* Switch the decision whether logs are to be displayed or not
	*/
	void SwitchLogs();

	/*!
	* Add an attacker in the game
	* @param gate the gate through which the attacker enters the game
	*/
	void AddAttacker 					(const CGate & gate);

	/*!
	* Get number of attackers that have already escaped the map through the exit gate
	* @return number of attackers that have already escaped the map through the exit gate
	*/
	int AttackersWon() const;
	
	/*!
	* Get number of alive attackers
	* @return number of alive attackers
	*/
	int AttackersAlive() const;
	
	/*!
	* Get the amount of money the player has left to spend
	* @return the amount of money the player has left to spend
	*/
	int Money() const;
	
	/*!
	* Give the player given amount of money
	* @param money the amount of money being given to the player
	*/
	void AddMoney						(const int & money);
	
	/*!
	* Set the amount of players money to certain value 
	* @param money the amount of money the player will have
	*/
	void SetMoney						(const int & money);
	
	/*!
	* Tell whether the logs will be displayed next round or not
	* @return true when the logs will be displayed next round, false when otherwise
	*/
	bool LogsON() const;

	//! vector containing all attackers in the game
	std::vector<CAttacker*> v_attackers;

private:

	//! vector containing all attackers in the game
	std::vector<CTower*> v_towers;

	//! vector containing all towers in the game
	std::vector<CGate> v_gates;
	
	//! vector containing all logs created this round
	std::vector<TLog> v_logs;

	//! vector containing all borders in the game
	std::vector<TBorder> v_borders;

	//! number of attackers alive
	int m_attackers_alive;

	//! number of the first attacker that was added by the player
	int m_first_not_loaded;

	//! maximum height of the window
	unsigned int m_maxheight;

	//! maximum width of the window
	unsigned int m_maxwidth;

	//! the exit gate to which all attackers are heading
	CGate m_exit_gate;

	//! 2D vector containing valued nodes used to find the shortest path for the attackers
	std::vector<std::vector<int>> map;

	//! number of attackers that have already escaped the map through the exit gate
	int m_attackers_won;

	//! decides whether the logs will be displayed next round or not
	bool m_logs_on;

	//! amount of money the player has left to spend on attackers
	int m_money;
};


#endif


