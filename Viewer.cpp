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

	f_moved = 0;
}

Viewer::~Viewer() {
	// TODO Auto-generated destructor stub
}

void Viewer::view() {
	int c;
	init_curses();

	do
	{
		draw();
		c = getch();
		f_moved = 0;

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
			move(0,-(LINES-(2+m_args.persist)));
			break;
		case KEY_NPAGE:
			move(0,(LINES-(2+m_args.persist)));
			break;
		}
	}while(c != 'q');

	end_curses();
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
	unsigned maxCol, x, startY;

	for (x = m_col+1; x < m_csv->getCols(); x++)
	{
		if (startX(m_col,x) > getmaxx(stdscr))
			break;
	}

	maxCol = x-1;
	startY = 0;

	erase();

	if (m_args.persist > 0)
	{
		for (int y = 0; y < m_args.persist; y++)
		{
			for (x = m_col; x < maxCol+1; x++)
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
		for (x = m_col; x < maxCol+1; x++)
		{
			mvprintw((startY+(y-m_row)),startX(m_col,x)," %s",m_csv->getCell(y,x).c_str());
			mvprintw((startY+(y-m_row)),(startX(m_col,x+1)-2)," |");
		}
	}

	attron(A_REVERSE);
	mvprintw(LINES-1,0,"Row %d/",(m_row+(LINES-(1+startY))));
	printw("%d ",m_csv->getRows());
	printw("(%d%%)",(int)(100*(m_row+(LINES-(1+startY)))/m_csv->getRows()));
	printw(" Column %d/",(maxCol+1));
	printw("%d ",m_csv->getCols());
	printw("(%d%%)",(int)(100*(maxCol+1)/m_csv->getCols()));
	attroff(A_REVERSE);

	refresh();
}

void Viewer::move(int deltaX, int deltaY) {
	int nonData;
	if (m_args.persist > 0)
		nonData = 3;
	else
		nonData = 2;

	if ((unsigned)(m_row+deltaY) < (m_csv->getRows()-(LINES-(m_args.persist+nonData))) && (m_row+deltaY) >= (m_args.persist))
		m_row += deltaY;
	else if (abs(deltaY) > 1)
	{
		if (deltaY < 0)
			m_row=m_args.persist;
		else
			m_row=(m_csv->getRows()-(LINES-(m_args.persist+(nonData-1))));
	}

	if (((unsigned)(m_col+deltaX) < m_csv->getCols() && !lastColOnScreen(m_col+deltaX)) && (m_col+deltaX) >= 0)
		m_col += deltaX;
	else if (abs(deltaX) > 1)
	{
		if (deltaX < 0)
			m_col = 0;
		else
			m_col=(m_csv->getCols()-1);
	}

	f_moved = 1;
}

void Viewer::move(int percent) {
	double decimal = ((double)(percent))/100.;
	unsigned line = (unsigned)100*(decimal*((double)(m_csv->getRows())));

	move(0,line-m_row);
}

void Viewer::jump_dialog() {
	WINDOW* dialog;

	dialog = newwin(7,40,(LINES/2)-3,(COLS/2)-20);
	box(dialog,0,0);

	mvwprintw(dialog,2,2,"Jump to line (Enter % at end for percent:");

	delwin(dialog);
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

bool Viewer::lastColOnScreen (unsigned col) {
	return COLS > (startX(col-1,m_csv->getCols()-1)) + (m_csv->getColWidth(m_csv->getCols()-1));
}
