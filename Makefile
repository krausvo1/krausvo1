compile:
	g++ -Wall -pedantic -Wno-long-long -O0 -ggdb main.cpp menu.cpp map.cpp tower.cpp gate.cpp attacker.cpp game.cpp advancedA.cpp basicA.cpp advancedT.cpp basicT.cpp -lncurses
run:
	./a.out
