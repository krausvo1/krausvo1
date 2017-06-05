CXX=g++
CXXFLAGS=-Wall -pedantic -g -std=c++11 -Wno-long-long -O0 -ggdb
CURSES=-lncurses
RM=rm -rf
EXECUTABLE=krausvo1
SOURCES=./src/main.cpp ./src/map.cpp ./src/menu.cpp ./src/game.cpp ./src/tower.cpp ./src/advancedT.cpp ./src/basicT.cpp ./src/attacker.cpp ./src/advancedA.cpp ./src/basicA.cpp ./src/log.cpp ./src/border.cpp ./src/gate.cpp

all:$(SOURCES:.cpp=.o)
	$(CXX) $(SOURCES:.cpp=.o) -o $(EXECUTABLE) $(CURSES)
	doxygen ./src/doc.cpp

compile:$(SOURCES:.cpp=.o)
	$(CXX) $(SOURCES:.cpp=.o) -o $(EXECUTABLE) $(CURSES)

clean:
	$(RM) $(SOURCES:.cpp=.o) $(EXECUTABLE)
	$(RM) doc

doc:
	mkdir doc
	doxygen ./src/doc.cpp

run:
	./$(EXECUTABLE)

