#include <curses.h>
#include <ctime>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>


using namespace std;

int main (){
	int m_range = 3, m_xpos = 45, m_ypos = 5;
	for(int j = - 2; j < m_range; j++){
		for(int k = - 2; k < m_range; k++)
			if()
			cout << m_ypos + j << "," << m_xpos + k <<"  ";
	cout << endl;
	}

	cout<<endl;
	m_range = 2;
	for(int j = - 1; j < m_range; j++){
		for(int k = - 1; k < m_range; k++)
			cout << m_ypos + j << "," << m_xpos + k <<"  ";
	cout << endl;
	}
	return 0;
}

