#include<iostream>
using namespace std;
#include<ncurses.h>

//g++ test_window1.cc -lncurses
int main()
{
	int x = 0,y = 0;
	int max_x = 0,max_y = 0;

	initscr();
	noecho();
	curs_set(0);

	return 0;
}
