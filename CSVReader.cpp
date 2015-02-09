/*
 * CSVReader.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: paul
 */

#include "CSVReader.h"

CSVReader::CSVReader() {
	m_delim = ',';
}

CSVReader::CSVReader(const char* fname, char delim) {
	m_delim = delim;
	m_fname = fname;

	read_file(fname);
}

CSVReader::~CSVReader() {
	clear();
}

void CSVReader::read_file(const char* fname) {
	m_fin.open(fname);

	if (m_fin.is_open())
	{
		stringstream ss;
		vector<string> row;
		string rawRow, rawCell;

		while (!m_fin.eof())
		{
			if (!getline(m_fin,rawRow)) break;
			ss << rawRow;

			while (ss)
			{
				if (!getline(ss, rawCell, m_delim)) break;
				row.push_back(rawCell);
			}

			m_data.push_back(row);

			row.clear();
			ss.clear();
			ss.str("");
		}

		m_fin.close();
		countCols();
	}
}

void CSVReader::countCols()
{
	unsigned tmp;
	unsigned max;

	max = 0;
	for (unsigned x = 0; x < m_data.size(); x++)
	{
		if (m_data.at(x).size() > max)
			max = m_data.at(x).size();
	}

	for (unsigned x = 0; x < max; x++)
	{
		tmp = 0;
		for (unsigned y = 0; y < m_data.size(); y++)
		{
			if (x < m_data.at(y).size())
			{
				if (m_data.at(y).at(x).size() > tmp)
					tmp = m_data.at(y).at(x).size();
			}
		}
		m_colWidth.push_back(tmp);
	}
}

unsigned CSVReader::getColWidth (unsigned col)
{
	if (col < m_colWidth.size())
		return m_colWidth.at(col);
	else
		return 0;
}

string CSVReader::getCell (unsigned row, unsigned col)
{
	if (row < m_data.size())
	{
		if (col < m_data.at(row).size())
			return m_data.at(row).at(col);
		else
			return string("");
	}
	else
		return string("");
}
