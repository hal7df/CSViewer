#include <ncurses.h>
#include <vector>
#include "CSVReader.h"
using namespace std;

void init_curses ()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
}
int main (int argc, char* argv[])
{
	init_curses();
	return 0;
}
