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

CSVReader::CSVReader(const char* fname) {
	m_delim = ',';
	m_fname = fname;

	read_file(fname);
}

CSVReader::~CSVReader() {
	// TODO Auto-generated destructor stub
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
	}
}
