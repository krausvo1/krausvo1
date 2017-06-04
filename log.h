#ifndef LOG_H
#define LOG_H
	
	/*!
	* Structure representing a log containing information about 
	* collisions between a tower and an attacker
	*/
struct TLog{

	/*!
	* Constructor initialising a log
	* @param ID ID number of the attacker that was hit
	* @param health remaining health of the attacker that was shot
	*/
	TLog(const int & ID, const int & health) : t_ID(ID), t_health(health) {}

	//! ID number of an attacker which was shot
	int t_ID;

	//! amount of remaining health the hit attacker has
	int t_health;
};


#endif