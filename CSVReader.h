/*
 * CSVReader.h
 *
 *  Created on: Feb 3, 2015
 *      Author: paul
 *
 *  Compiled with reference to http://www.cplusplus.com/forum/general/17771/#msg89650
 */

#ifndef CSVREADER_H_
#define CSVREADER_H_

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;

class CSVReader {
public:
	CSVReader();
	CSVReader(const char* fname);
	virtual ~CSVReader();

	//File management functions
	void read_file (const char* fname);
	void refresh () { clear(); read_file(m_fname); }
	void clear () { m_data.clear(); }

	//Data access functions
	string getCell (int row, int col) { return m_data.at(row).at(col); }
	bool good () { return m_data.size() != 0; }

	//Settings
	void  setDelim (char delim) { m_delim = delim; refresh(); }
private:
	ifstream m_fin;
	vector<vector<string> > m_data;
	const char* m_fname;
	char m_delim;
};

#endif /* CSVREADER_H_ */
