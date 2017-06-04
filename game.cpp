#include "game.h"

CGame::CGame() : m_goal(0), m_maxheight(0), m_maxwidth(0), m_engine(m_seed()), m_attackers_won(0)
{}

CGame::~CGame()
{
	for(unsigned int i = 0; i < v_towers.size(); i++)
      delete v_towers[i];

   	for(unsigned int i = 0; i < v_attackers.size(); i++)
      delete v_attackers[i];
}

void CGame::RestrictArea()
{
	for(int i = m_maxwidth - 5; i < m_maxwidth - 2; i++)
		for(int j = 0; j < m_maxheight - 3; j++)
			v_restricted_area.push_back(make_pair(j,i));

	for(int j = 0; j < m_maxheight - 3; j++)
		v_restricted_area.push_back(make_pair(j,1));
}

void CGame::SetGoal()
{
	std::uniform_int_distribution<int> goal_range(5, 15);

	m_goal = goal_range(m_engine);
}

bool CGame::CheckVictory(const int & attackers_won)
{
	if(attackers_won >= m_goal){
		move( (m_maxheight - 3) / 2, m_maxwidth / 2 - 8);
		attron(A_BOLD);
		printw("VICTORY!");
		move( ((m_maxheight - 3) / 2) + 1, m_maxwidth / 2 - 8);
		printw("Play again? [Y/N]");
		attroff(A_BOLD); 
		refresh();

		return true;
	}

	return false;
}

bool CGame::CheckGameOver(const int & m_money, 
						  const int & attackers_alive)
{
	if(m_money == 0 && attackers_alive == 0)
	{
		move( (m_maxheight - 3) / 2, m_maxwidth / 2 - 8);
		attron(A_BOLD);
		printw("GAME OVER");
		move( ((m_maxheight - 3) / 2) + 1, m_maxwidth / 2 - 8);
		printw("Play again? [Y/N]");
		attroff(A_BOLD); 
		refresh();

		return true;
	}

	return false;
}

void CGame::CountMoney(CMap & map)
{
	int avg_damage = 0;

	for(unsigned int i = 0; i < v_gates.size(); i++)
	{
		if(v_gates[i].GateType() == G_EXIT)
			continue;

		avg_damage += map.TestRound(i);
	}

	avg_damage = avg_damage / (v_gates.size() - 1);
	map.SetMoney((avg_damage / 200 + 2) * 1000);

	if(m_goal > 8)
		map.AddMoney(1000);
}

void CGame::StartGame(CMap & map)
{		
	if(map.Money() == 0)
		CountMoney(map);

	nodelay(stdscr, true);

	while(1)
	{
		map.NextRound();

		move(m_maxheight - 2, 0);
		printw("Attackers alive: %d, attackes won: %d, goal: %d, money: %d$", 
			map.AttackersAlive(), map.AttackersWon(), m_goal, map.Money());

		if(map.AttackersWon() > m_attackers_won){
			m_attackers_won = map.AttackersWon();
			map.AddMoney(600);
		}

		if(map.LogsON()){
			printw(", logs: ON");
			map.PrintLogs();
		}
		else
			printw(", logs: OFF");

		
		if(CheckVictory(map.AttackersWon()) || CheckGameOver(map.Money(), map.AttackersAlive()))
		{
			nodelay(stdscr, false);
			while(1)
			{
				switch(getch())
				{	
					//PLAY AGAIN
					case 'Y':
					case 'y':
						NewGame();
						return;

					//QUIT GAME
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
		
		switch(getch())
		{
			//TURN LOGS ON/OFF
			case 'L':
			case 'l':
				map.SwitchLogs();
				continue;

			//QUIT GAME
			case 'Q':
			case 'q':
				return;

			//ADD AN ATTACKER TO GATE NUMBER 1
			case '1':
				map.AddAttacker(v_gates[0]);
				continue;

			//ADD AN ATTACKER TO GATE NUMBER 2
			case '2':
				map.AddAttacker(v_gates[1]);
				continue;

			//ADD AN ATTACKER TO GATE NUMBER 3
			case '3':
				if(v_gates.size() > 3)
					map.AddAttacker(v_gates[2]);
				continue;

			//SAVE GAME
			case 'S':
			case 's':
				SaveGame(map);
				continue;
		}
	}	
}

void CGame::ClearMap()
{
	m_map.resize(m_maxheight);
	for(int i = 0; i < m_maxheight; i++)
		m_map[i].resize(m_maxwidth);


	for(int i = 0; i < m_maxheight; i++)
		for(int j = 0; j < m_maxwidth; j++)
			m_map[i][j] = 0;

	//Place borders:
	//from top left corner to top right corner
	for(int i = 0; i < m_maxwidth; i++)
		m_map[0][i] = -8;

	//from bottom left corner to bottom right corner
	for(int i = 0; i < m_maxwidth; i++)
		m_map[m_maxheight - 3][i] = -8;

	//from bottom left corner to top left corner
	for(int i = 0; i < m_maxheight - 3; i++)
		m_map[i][0] = -8;

	//from top right corner to bottom right corner
	for(int i = 0; i < m_maxheight - 3; i++)
		m_map[i][m_maxwidth - 1] = -8;
	
	//place generated borders
	for(unsigned int i = 0; i < v_borders.size(); i++)
		m_map[v_borders[i].t_ypos][v_borders[i].t_xpos] = -8;
	
	//place towers
	for(unsigned int i = 0; i < v_towers.size(); i++)
		m_map[v_towers[i]->TowerYpos()][v_towers[i]->TowerXpos()] = -8;

}

std::vector< std::pair<int,int> > CGame::FindPath(CGate & start)
{
	//stores coordinates of nodes
	std::vector< std::pair<int, int> > nodes;

	int y = start.GateYpos();
	int x = start.GateXpos();

	nodes.push_back(make_pair(y,x));
	
	//count the cost of every node
	while(nodes[0].first != m_exit_gate.GateYpos() || nodes[0].second != m_exit_gate.GateXpos())
	{
		if(y != 0 && m_map[y - 1][x] == 0){
			nodes.push_back(make_pair(y - 1, x));
			m_map[y - 1][x] = m_map[y][x] + 1;
		}
		
		if(x != 0 && m_map[y][x - 1] == 0){
			nodes.push_back(make_pair(y, x - 1));
			m_map[y][x - 1] = m_map[y][x] + 1;
		}

		if(y != m_maxheight - 4 && m_map[y + 1][x] == 0){
			nodes.push_back(make_pair(y + 1, x));
			m_map[y + 1][x] = m_map[y][x] + 1;
		}

		if(x != m_maxwidth - 1 && m_map[y][x + 1] == 0){
			nodes.push_back(make_pair(y, x + 1));
			m_map[y][x + 1] = m_map[y][x] + 1;
		}

		if(!nodes.empty())
			nodes.erase(nodes.begin());

		y = nodes[0].first;
		x = nodes[0].second;
	}

	m_map[start.GateYpos()][start.GateXpos()] = 0;

	 

	//stores coordinates representing the way form entry gate to exit gate
	std::vector< std::pair<int,int> > path;

	y = m_exit_gate.GateYpos();
	x = m_exit_gate.GateXpos();
	
	path.push_back(make_pair(y, x));
	

	int round = 0;

	//find the way with the lowest nodes values
	while((path[path.size() - 1].first != start.GateYpos() || path[path.size() - 1].second != start.GateXpos()))
	{	
		if(path.size() > 1){
			y = path[path.size() - 1].first;
			x = path[path.size() - 1].second;
		}

		if(y != 0 && m_map[y - 1][x] == m_map[y][x] - 1){
			path.push_back(make_pair(y - 1, x));
			continue;
		}

		if(x != 0 && m_map[y][x - 1] == m_map[y][x] - 1){
			path.push_back(make_pair(y, x - 1));
			continue;
		}

		if(y != m_maxheight - 4 && m_map[y + 1][x] == m_map[y][x] - 1){
			path.push_back(make_pair(y + 1, x));
			continue;
		}
		
		if(x != m_maxwidth - 1 && m_map[y][x + 1] == m_map[y][x] - 1){
			path.push_back(make_pair(y, x + 1));
			continue;
		}

		
		round++;
		//in case this while loop 
		if(round > 100000){
			std::vector< std::pair<int,int> > error;
			return error;
		}
	}

	return path;
}

void CGame::GenerateMap()
{
	std::uniform_int_distribution<int> placement_range(7, 10);

	//choose which object (1 - wall, 2 - tower) eliminate
	for(int choice = 2; choice > 0; choice--)
	{
		for(unsigned int i = 0; i < v_gates.size() - 1; i++)
		{
			ClearMap();
			v_gates[i].path = FindPath(v_gates[i]);

			//choose a coordinate from the path, place an obstacle somewhere in its area
			//and find new shortest path
			for(int rnd_pos = v_gates[i].path.size() - 4; rnd_pos > 3; rnd_pos -= placement_range(m_engine))
			{
				if(rnd_pos < 3)
					break;

				ClearMap();
				
				//in case a path cannot be found, destroy the last inserted border
				while(v_gates[i].path.size() == 0)
				{
					v_gates[i].path = FindPath(v_gates[i]);
					v_borders.pop_back();
				}

				PlaceObstacle(v_gates[i].path[rnd_pos], v_gates[i].path[rnd_pos - 1], 
							  v_gates[i].path[rnd_pos - 2], choice);
			}
		}
	}

	//find the shortest paths with all obstacles in place
	for(unsigned int i = 0; i < v_gates.size() - 1; i++)
	{
		ClearMap();
		v_gates[i].path = FindPath(v_gates[i]);
		
		//in case a path cannot be found, destroy the last inserted border
		while(v_gates[i].path.size() == 0)
		{
			v_gates[i].path = FindPath(v_gates[i]);
			v_borders.pop_back();
		}
	}
}


void CGame::PlaceObstacle(std::pair<int,int> position1, 
						  std::pair<int,int> position2, 
						  std::pair<int,int> position3, 
						  const int & eliminate)
{
	for(unsigned int i = 0; i < v_restricted_area.size(); i++)
		if(position1 == v_restricted_area[i] ||
		   position2 == v_restricted_area[i] ||
		   position3 == v_restricted_area[i]  ) return;
	
	std::uniform_int_distribution<int> dist(1, 2);

	if(position1.second < 2)
		position1.second = 2;


	int choice = dist(m_engine);

	//second chance to choose an object
	if(choice == eliminate)
		choice = dist(m_engine);

	if(choice != eliminate)
		switch(choice){
			// WALL
			case 1:
				GenerateWall(position1);
				break;
			
			// TOWER
			case 2:
				GenerateTower(position1, position2, position3, dist(m_engine));
		}
}

void CGame::GenerateWall(std::pair<int,int> position)
{
	std::uniform_int_distribution<int> wall_length_range(3, 8);
	int wall_length = wall_length_range(m_engine);

	std::uniform_int_distribution<int> start_ypos_range(position.first - wall_length, position.first);
	int start_ypos = start_ypos_range(m_engine);

	for(int i = 0; i < wall_length; i++)
	{	
		if(start_ypos + i > 0 && start_ypos + i < m_maxheight - 4){
			m_map[start_ypos + i][position.second] = -8;
			v_borders.push_back(TBorder(start_ypos + i, position.second));
		}
	}
}

void CGame::GenerateTower(std::pair<int,int> position1, 
					   	  std::pair<int,int> position2, 
					      std::pair<int,int> position3, 
					      const int & 		 choice)
{
	std::uniform_int_distribution<int> basic_range(-1, 1);
	int basic_pos = basic_range(m_engine);
	
	// the tower should not be placed directly in the path
	while(basic_pos == 0)
		basic_pos = basic_range(m_engine);


	std::uniform_int_distribution<int> advanced_range(-2, 2);
	int advanced_pos = advanced_range(m_engine);

	// the tower should not be placed directly in the path
	while(advanced_pos == 0)
		advanced_pos = advanced_range(m_engine);


	switch(choice){
		//ADVANCED TOWER
		case 1:
			GenerateAdvancedTower(advanced_pos, position1, position3);
			break;

		//BASIC TOWER (higher chance to be chosen)
		case 2:
		case 3:
			GenerateBasicTower(basic_pos, position1, position2);
			break;
	}
}

void CGame::GenerateBasicTower(int & placement, 
							   std::pair<int,int> position1, 
							   std::pair<int,int> position2)
{
	//tower cannot be placed at 0 vertical position
	while(position1.first + placement < 1)
		placement++;
	
	//tower cannot be placed above maximum height - 4
	while(position1.first + placement > m_maxheight - 4)
		placement--;

	//if the next step on the path has the same horizontal position,
	//place the tower one horizontal position to the right 
	if(position2.second == position1.second)
		position1.second++;

	m_map[position1.first + placement][position1.second] = T_BASIC;
	v_towers.push_back(new CBasicTower (position1.first + placement, position1.second));
}

void CGame::GenerateAdvancedTower(int & placement, 
								  std::pair<int,int> position1, 
								  std::pair<int,int> position3)
{	
	//tower cannot be placed at 0 vertical position	
	while(position1.first + placement < 1)
		placement++;
	
	//tower cannot be placed above maximum height - 4	
	while(position1.first + placement > m_maxheight - 4)
		placement--;

	//if the second next step has the same horizontal position,
	//place the tower one horizontal position to the right 
	if(position3.second == position1.second)
		position1.second++;

	m_map[position1.first + placement][position1.second] = T_ADVANCED;
	v_towers.push_back(new CAdvancedTower (position1.first + placement, position1.second));
}

void CGame::GenerateGates()
{
	std::uniform_int_distribution<int> height_range(1, m_maxheight - 4);
	std::uniform_int_distribution<int> num_of_gates(2, 3);

	//two gates
	if(num_of_gates(m_engine) == 2){
		std::uniform_int_distribution<int> first_half(1, (m_maxheight - 4) / 2);
		std::uniform_int_distribution<int> second_half((m_maxheight - 4) / 2 + 1, m_maxheight - 4);

		CGate gate1 ('1', first_half(m_engine), m_maxwidth - 2, 1);
		CGate gate2 ('2', second_half(m_engine), m_maxwidth - 2, 2);
		CGate gate3 (G_EXIT, height_range(m_engine), 1, 3);

		v_gates.push_back(gate1);
		v_gates.push_back(gate2);
		v_gates.push_back(gate3);

		m_exit_gate = gate3;
	}
	//three gates
	else{
		std::uniform_int_distribution<int> first_third(1, (m_maxheight - 4) / 3);
		std::uniform_int_distribution<int> second_third((m_maxheight - 4) / 3 + 1, (m_maxheight - 4) * 2/3);
		std::uniform_int_distribution<int> third_third((m_maxheight - 4) * 2/3 + 1, m_maxheight - 4);

		CGate gate1 ('1', first_third(m_engine), m_maxwidth - 2, 1);
		CGate gate2 ('2', second_third(m_engine), m_maxwidth - 2, 2);
		CGate gate3 ('3', third_third(m_engine), m_maxwidth - 2, 3);
		CGate gate4 (G_EXIT, height_range(m_engine), 1, 3);

		v_gates.push_back(gate1);
		v_gates.push_back(gate2);
		v_gates.push_back(gate3);
		v_gates.push_back(gate4);

		m_exit_gate = gate4;
	}
}

void CGame::NewGame()
{
	getmaxyx(stdscr, m_maxheight, m_maxwidth);

	v_towers.clear();
	v_attackers.clear();
	v_gates.clear();
	v_borders.clear();
	v_restricted_area.clear();
	
	RestrictArea();

	GenerateGates();

	GenerateMap();

	SetGoal();

	AssignBorders();

	CMap map(v_towers, v_gates, v_borders, m_maxheight, m_maxwidth, m_exit_gate);
	
	StartGame(map);
	
	nodelay(stdscr, false);
	endwin();
}

bool CGame::LoadGame(ifstream & file)
{
	v_towers.clear();
	v_attackers.clear();
	v_gates.clear();
	v_borders.clear();

	getmaxyx(stdscr, m_maxheight, m_maxwidth);

	int maxheightLoaded, maxwidthLoaded;
	int money = 0;

	//read the header
	if(!(file >> maxheightLoaded >> maxwidthLoaded >> m_attackers_won >> m_goal >> money)){
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
	int health = 0, stunned = 0;
	int line_number = 1;

	//read the objects
	while(file >> object >> type >> ypos >> xpos)
	{
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
	
	if(!AssignPaths())
		return false;

	AssignBorders();
	
	printw("Game successfully loaded!");
	refresh();
	usleep(190000);
	clear();

	CMap map(v_attackers, v_towers, v_gates, v_borders, m_maxheight, m_maxwidth, m_exit_gate, m_attackers_won, money);
	
	StartGame(map);

	nodelay(stdscr, false);
	endwin();

	return true;
}

void CGame::AssignBorders()
{
	for(unsigned int i = 0; i < v_towers.size(); i++)
	{
			v_towers[i]->AssignBorders(v_borders);
			v_towers[i]->CheckBlindSpots();
	}
}

bool CGame::AssignPaths()
{
	for(unsigned int i = 0; i < v_gates.size(); i++)
	{
		if(v_gates[i].GateType() != G_EXIT){
			ClearMap();
			v_gates[i].path = FindPath(v_gates[i]);
		}
	}


	for(unsigned int i = 0; i < v_gates.size(); i++)
		for(unsigned int j = 0; j < v_attackers.size(); j++)
		{
			if(!v_attackers[j]->IsAssigned() && v_gates[i].GateType() != G_EXIT)
			    v_attackers[j]->AssignPath(v_gates[i]);
		}


	for(unsigned int i = 0; i < v_attackers.size(); i++)
		if(!v_attackers[i]->IsAssigned()){
			printw("Error occured while placing attackers on their paths - %d. attacker is off the path!", i + 1);
			refresh();
			usleep(2400000);		
			return false;
		}

	return true;
}

bool CGame::CreateObject(const char & object,    
						 const char & type, 
						 const int & ypos, 	     
						 const int & xpos, 
						 const int & health, 	 
						 const bool & stunned)
{	
	if(ypos > m_maxheight - 4 || ypos < 1 || xpos > m_maxwidth - 2 || xpos < 1){
		return false;
	}

	switch(object)
	{
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

bool CGame::CreateTower(const char & type, 
						const int & ypos, 
						const int & xpos)
{
	if(xpos < 3 || xpos > m_maxwidth - 5) return false;

	switch(type)
	{
		case T_BASIC:
			v_towers.push_back(new CBasicTower(ypos, xpos));
			break;
		
		case T_ADVANCED:
			v_towers.push_back(new CAdvancedTower(ypos, xpos));
			break;

		default:
			return false;
	}

	return true;
}

bool CGame::CreateAttacker(const char & type, 
						   const int & ypos, 
						   const int & xpos,  
						   const int & health, 
	   					   const bool & stunned)
{
	if(xpos == 1 || xpos == m_maxwidth - 2) return false;

	switch(type){
		case A_BASIC:
			v_attackers.push_back(new CBasicAttacker(ypos, xpos, v_attackers.size(), health, stunned));
			break;
		case A_ADVANCED:
			v_attackers.push_back(new CAdvancedAttacker(ypos, xpos, v_attackers.size(), health));
			break;
		default:
			return false;
	}

	return true;
}

bool CGame::CreateGate(const char & type, 
					   const int & ypos, 
					   const int & xpos)
{   										
	if((xpos != 1 && xpos != m_maxwidth - 2) || 
	   (type != G_EXIT && type != '1' && type != '2' && type != '3'))
		return false;

	v_gates.push_back(CGate(type, ypos, xpos, v_gates.size()+1));

	if(type == G_EXIT)
		m_exit_gate = v_gates[v_gates.size() - 1];

	return true;
}

bool CGame::CreateBorder(const char & type, 
						 const int & ypos, 
						 const int & xpos)
{
	if(xpos < 3 || xpos > m_maxwidth - 5 || type != BORDER) return false;

	v_borders.push_back(TBorder(ypos, xpos));
	return true;
}

void CGame::SaveGame(const CMap & map)
{
	//name of the file with saved game consists of time and date
	//of its saving
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

	//write the header
	outputFile << m_maxheight << " " << m_maxwidth << " " << map.AttackersWon() << " " << m_goal << " " << map.Money() << '\n';

	//write gates
	for(unsigned int i = 0; i < v_gates.size(); i++)
		outputFile << "G " << v_gates[i].GateType() << " " <<v_gates[i].GateYpos() << " " << v_gates[i].GateXpos() << '\n';

	//write attackers
	for(unsigned int i = 0; i < map.v_attackers.size(); i++)
	{
		if(map.v_attackers[i]->AttackerType() == A_DEAD || map.v_attackers[i]->HasWon())
			continue;

		outputFile << "A " << map.v_attackers[i]->AttackerType() << " " << map.v_attackers[i]->AttackerYpos() << " " 
		<< map.v_attackers[i]->AttackerXpos() << '\n';
		
		outputFile << map.v_attackers[i]->AttackerHealth() << " " << map.v_attackers[i]->IsStunned() << '\n';
	}

	//write towers
	for(unsigned int i = 0; i < v_towers.size(); i++)
		outputFile << "T " << v_towers[i]->TowerType() << " " << v_towers[i]->TowerYpos() << " " << v_towers[i]->TowerXpos() << '\n';

	//write borders
	for(unsigned int i = 0; i < v_borders.size(); i++)
		outputFile << "B #" << v_borders[i].t_ypos << " " << v_borders[i].t_xpos << '\n';
}

void CGame::CheckGoal()
{
	if(m_goal > 15 || m_goal < 5){
		clear();
		printw("Goal out of interval <5,15>, setting it to 15.");
		refresh();
		usleep(2000000);

		m_goal = 15;
	}
}

void CGame::CheckResolution(const int & maxheightLoaded, 
							const int & maxwidthLoaded)
{
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