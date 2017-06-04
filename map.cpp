#include "map.h"

CMap::CMap(const std::vector<CTower*> & towers, 
		   const std::vector<CGate> & gates, 
		   const std::vector<TBorder> & borders, 
		   const unsigned int & maxheight,
		   const unsigned int & maxwidth, 
		   const CGate & exit) :

		   v_towers(towers), v_gates(gates), v_borders(borders), m_attackers_alive(0), m_first_not_loaded(0), 
		   m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), m_attackers_won(0), m_logs_on(true), m_money(0)
{
}

CMap::CMap(const std::vector<CAttacker*> & attackers, 
		   const std::vector<CTower*> & towers, 
		   const std::vector<CGate> & gates,
		   const std::vector<TBorder> & borders, 
		   const unsigned int & maxheight, 
		   const unsigned int & maxwidth, 
		   const CGate & exit, 
		   const int & attackers_won, 
		   const int & money) :

		   v_attackers(attackers), v_towers(towers), v_gates(gates), v_borders(borders), m_attackers_alive(0), 
		   m_first_not_loaded(attackers.size()), m_maxheight(maxheight), m_maxwidth(maxwidth), m_exit_gate(exit), 
		   m_attackers_won(attackers_won), m_logs_on(true), m_money(money)
{
}

CMap::~CMap()
{
	for(unsigned int i = m_first_not_loaded; i < v_attackers.size(); i++)
          delete v_attackers[i];  
}

void CMap::NextFrame()
{
	PrintBorders();
	PrintTowers();
	PrintGates();
	CheckCollisions();
	PrintAttackers();
}

void CMap::PrintBorders() const
{
	//Place borders:
	//from top left corner to top right corner
	for(unsigned int i = 0; i < m_maxwidth; i++){
		move(0, i);
		addch(BORDER);
	}

	//from bottom left corner to bottom right corner
	for(unsigned int i = 0; i < m_maxwidth; i++){
		move(m_maxheight - 3, i);
		addch(BORDER);
	}

	//from bottom left corner to top left corner
	for(unsigned int i = 0; i < m_maxheight - 3; i++){
		move(i, 0);
		addch(BORDER);
	}

	//from top right corner to bottom right corner
	for(unsigned int i = 0; i < m_maxheight - 3; i++){
		move(i, m_maxwidth - 1);
		addch(BORDER);
	}

	//place generated borders
	for(unsigned int i = 0; i < v_borders.size(); i++){
		move(v_borders[i].t_ypos, v_borders[i].t_xpos);
		addch(BORDER);
	}
}

void CMap::PrintTowers() const
{
	for(unsigned int i = 0; i < v_towers.size(); i++){
		move(v_towers[i]->TowerYpos(), v_towers[i]->TowerXpos());
		addch(v_towers[i]->TowerType());
	}
}

void CMap::PrintGates() const
{
	for(unsigned int i = 0; i < v_gates.size(); i++){
		move(v_gates[i].GateYpos(), v_gates[i].GateXpos());
		addch(v_gates[i].GateType());
	}
}

void CMap::CheckCollisions()
{
	for(unsigned int t = 0; t < v_towers.size(); t++)
	{
		for(unsigned int a = 0; a < v_attackers.size(); a++)
			if(v_attackers[a]->IsInGame() && v_towers[t]->InRange(*v_attackers[a]))
				v_towers[t]->AddTarget(v_attackers[a]);

		if(v_towers[t]->TargetsCount())
			v_towers[t]->Shoot(*v_attackers[v_towers[t]->ChooseTarget()]);
		else
			if(v_towers[t]->TowerType() == T_ADVANCED)
				v_towers[t]->ChargeStun();

		v_towers[t]->ClearTargets();
	}
}

void CMap::PrintAttackers()
{
	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	
	m_attackers_alive = 0;
	
	for(unsigned int i = 0; i < v_attackers.size(); i++)
	{
		if(v_attackers[i]->IsHit())
		{
			if(m_logs_on)
				v_logs.push_back(TLog(v_attackers[i]->AttackerID(), v_attackers[i]->AttackerHealth()));
			
			v_attackers[i]->SetHit(false);
			attron(COLOR_PAIR(1));
		}

		CheckPriorities(*v_attackers[i]);

		if(!v_attackers[i]->HasWon() && v_attackers[i]->AttackerType() != A_DEAD && v_attackers[i]->Move())
		{
			if(v_attackers[i]->CheckWin())
				m_attackers_won++;
			else
				m_attackers_alive++;

		}

		attroff(COLOR_PAIR(1));
	}
}

void CMap::CheckPriorities(CAttacker & attacker)
{
	int difference;

	if(attacker.AttackerType() == A_ADVANCED)
		difference = 1;
	else
		difference = 2;


	for(unsigned int i = 0; i < v_attackers.size() - 1; i++)
	{
		if((v_attackers[i]->m_start.path.size() - (attacker.AttackerMoves() + difference) > 0) && 
		   (v_attackers[i]->IsInGame()) &&
		   (v_attackers[i]->AttackerRealYpos() == attacker.m_start.path[attacker.m_start.path.size() - (attacker.AttackerMoves() + difference)].first) &&
		   (v_attackers[i]->AttackerRealXpos() == attacker.m_start.path[attacker.m_start.path.size() - (attacker.AttackerMoves() + difference)].second))
		{
			attacker.GiveWay(true);
			break;
		}
		else
			attacker.GiveWay(false);
	}
}

void CMap::AddAttacker(const CGate & start)
{
	move(0,0);

	if(m_money == 0){
		printw("Not enough money for more attackers! (%d$)", m_money);
		refresh();
		usleep(2000000);
		return;
	}

	printw("Gate %d, select attacker type:\t\t%d$\nPress B for basic attacker (-100$)\nPress A for advanced attacker (-200$)", start.GateID(), m_money);
	refresh();

	nodelay(stdscr, false);

	switch(getch())
	{
		//BASIC ATTACKER
		case 'B':
		case 'b':
			if(m_money < 100){
				move(3,0);
				printw("Not enough money for a basic attacker! (%d$)", m_money);
				refresh();
				usleep(2000000);
				break;
			}

			v_attackers.push_back(new CBasicAttacker(start, v_attackers.size()));
			m_attackers_alive++;
			m_money -= 100;
			break;

		//ADVANCED ATTACKER
		case 'A':
		case 'a':
			if(m_money < 200){
				move(3,0);
				printw("Not enough money for an advanced attacker! (%d)", m_money);
				refresh();
				usleep(2000000);
				break;
			}

			v_attackers.push_back(new CAdvancedAttacker(start, v_attackers.size()));
			m_attackers_alive++;
			m_money -= 200;
			break;
		
		default:
			break;
	}

	nodelay(stdscr, true);
	clear();
}

void CMap::PrintLogs()
{
	int health;

	for(unsigned int i = 0; i < v_logs.size(); i++)
	{
		if(v_logs[i].t_health <= 0)
			health = 0;
		else
			health = v_logs[i].t_health;

		move(m_maxheight - 1,0);
		printw("Attacker %d hit, remaining health: %d", v_logs[i].t_number, health);
		refresh();
		usleep(3000000);
	}

	v_logs.clear();
}


void CMap::SwitchLogs()
{
	if(m_logs_on)
		m_logs_on = false;
	else
		m_logs_on = true;
}

int CMap::TestRound(const int & gate_number)
{
	CAdvancedAttacker test(v_gates[gate_number], 0);

	while(!test.CheckWin())
	{
		for(unsigned int t = 0; t < v_towers.size(); t++)
		{
			if(v_towers[t]->InRange(test))
				v_towers[t]->Shoot(test);
		}

		test.Move();
	}

	clear();

	return 200 - test.AttackerHealth();
}

int CMap::AttackersWon() const
{
	return m_attackers_won;
}

int CMap::AttackersAlive() const
{
	return m_attackers_alive;
}

int CMap::Money() const
{
	return m_money;
}

void CMap::AddMoney(const int & money)
{
	m_money += money;
}

void CMap::SetMoney(const int & money)
{
	m_money = money;
}

bool CMap::LogsON() const
{
	return m_logs_on;
}