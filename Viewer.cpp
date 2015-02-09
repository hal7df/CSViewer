/*
 * Viewer.cpp
 *
 *  Created on: Feb 8, 2015
 *      Author: paul
 */

#include "Viewer.h"

//public ****************
Viewer::Viewer(CSVReader* csv, argData args) {
	m_csv = csv;
	m_args = args;

	m_row = m_args.persist;
	m_col = 0;
}

Viewer::~Viewer() {
	// TODO Auto-generated destructor stub
}

void Viewer::view() {
	char c;
	int rows;

	do
	{
		draw();
		c = getch();
		rows = getmaxy(stdscr);

		switch (c)
		{
		case KEY_UP:
			move(0,-1);
			break;
		case KEY_DOWN:
			move(0,1);
			break;
		case KEY_LEFT:
			move(-1,0);
			break;
		case KEY_RIGHT:
			move(1,0);
			break;
		case 'j':
			jump_dialog();
		case KEY_PPAGE:
			move(0,-(rows-(2+m_args.persist)));
			break;
		case KEY_NPAGE:
			move(0,(rows-(2+m_args.persist)));
			break;
		}
	}while(c != 'q');
}

//private ***************
//** Curses functions ***
void Viewer::init_curses() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
}

//** Drawing functions **
void Viewer::draw() {
	unsigned maxCol;
	int cumul;
	unsigned x, startY;

	cumul = 0;

	for (x = m_col+1; x < m_csv->getCols(); x++)
	{
		if (startX(m_col,x) > getmaxx(stdscr))
			break;
	}

	maxCol = x-1;
	startY = 0;

	if (m_args.persist > 0)
	{
		for (int y = 0; y < m_args.persist; y++)
		{
			for (x = m_col; x < maxCol; x++)
			{
				attron(A_BOLD);
				mvprintw(y,startX(m_col,x)," %s",m_csv->getCell(y,x).c_str());
				attroff(A_BOLD);
				mvprintw(y,(startX(m_col,x+1)-2)," |");
			}
		}
		for (x = 0; x < (startX(m_col,maxCol+1)-1); x++)
			mvprintw(m_args.persist,x,"-");

		startY = m_args.persist+1;
	}
	for (unsigned y = m_row; y < (m_row+(LINES-(1+startY))); y++)
	{
		for (x = m_col; x < maxCol; x++)
		{
			mvprintw((startY+(y-m_row)),startX(m_col,x)," %s",m_csv->getCell(y,x).c_str());
			mvprintw((startY+(y-m_row)),(startX(m_col,x+1)-2)," |");
		}
	}

	attron(A_REVERSE);
	mvprintw(LINES,0,"Line %d/",(m_row+(LINES-(1+startY))));
	printw("%d ",m_csv->getRows());
	printw("(%d%%)",(int)((m_row+(LINES-(1+startY)))/m_csv->getRows()));
	attroff(A_REVERSE);

	refresh();
}

void Viewer::move(int deltaX, int deltaY) {
	m_row += deltaY;
	m_col += deltaX;
}

void Viewer::move(int percent) {

}

//** Other functions ****
unsigned Viewer::startX(unsigned startCol, unsigned col) {
	unsigned cumul;

	cumul = 0;
	for (unsigned x = startCol+1; x <= col; x++)
	{
		cumul += (m_csv->getColWidth(x-1)+3);
	}

	return cumul;
}
