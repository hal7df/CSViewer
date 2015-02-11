/*
 * Viewer.h
 *
 *  Created on: Feb 8, 2015
 *      Author: paul
 */

#ifndef VIEWER_H_
#define VIEWER_H_

#include <ncurses.h>
#include <vector>
#include <cmath>
#include "CSVReader.h"

struct argData {
	string fName;
	int persist = 0;
	vector<int> cols;
};

class Viewer {
public:
	Viewer(CSVReader* csv, argData args);
	virtual ~Viewer();

	void view();
private:
	//Curses functions
	void init_curses();
	void end_curses() { endwin(); }

	//Display functions
	void draw();
	void jump_dialog();
	void move(int deltaX, int deltaY);
	void move(int percent);

	//Other functions
	unsigned startX(unsigned startCol, unsigned col);
	bool lastColOnScreen(unsigned col);

	CSVReader* m_csv;
	argData m_args;
	int m_row, m_col;
	int f_moved;
};

#endif /* VIEWER_H_ */
