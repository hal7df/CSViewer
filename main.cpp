#include <ncurses.h>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include "CSVReader.h"
using namespace std;

struct argData {
	string fName;
	int persist = 0;
	vector<int> cols;
};

void init_curses();
void usageMessage();
void version();
argData parseArgs (int argc, char* argv[]);

int main (int argc, char* argv[])
{
	if (argc < 2)
		usageMessage();
	else
	{
		argData args = parseArgs(argc, argv);
		cout << "Filename: " << args.fName << endl;
		cout << "Persist rows: " << args.persist << endl;
		cout << "Columns: ";
		for (unsigned x = 0; x < args.cols.size(); x++)
			cout << args.cols.at(x) << ' ';
		cout << endl;
		exit(0);
	}
	init_curses();
	return 0;
}

void init_curses ()
{
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);
}

void usageMessage()
{
	cout << "USAGE: csviewer [OPTIONS] FILE" << endl;
	cout << "csviewer is a small, commandline CSV file viewer using ncurses." << endl << endl;
	cout << "Options:" << endl;
	cout << " -p, --persist <n>\tMake the first n rows persist at top as a header." << endl;
	cout << " -c, --column <n> [...]\tOnly display column(s) n,..." << endl;
	cout << " -v, --version\t\tPrint version number and exit." << endl;
	cout << " -h, --help\t\tDisplay this help message and exit." << endl << endl;
	cout << "Arguments:" << endl;
	cout << " FILE\t\t\tPath to csv file to open." << endl;

	exit(0);
}

void version()
{
	cout << "csviewer 0.1" << endl;
	cout << "Author: Paul Bonnen" << endl;

	exit(0);
}

argData parseArgs (int argc, char* argv[])
{
	string tmp;
	int offset;
	argData args;
	bool flag;

	for (int x = 1; x < argc; x++)
	{
		tmp = string(argv[x]);
		flag = false;

		if (tmp.find("--") != string::npos)
		{
			if (tmp.find("persist") != string::npos)
			{
				if (argc <= x+1)
				{
					cout << "Error: must specify number of rows to persist." << endl;
					exit(EXIT_FAILURE);
				}
				for (unsigned y = 0; y < strlen(argv[x+1]); y++)
				{
					if (!isdigit(argv[x+1][y]))
						flag = true;
				}

				if (!flag)
					args.persist = atoi(argv[++x]);
				else
				{
					cout << "Error: must specify number of rows to persist." << endl;
					exit(EXIT_FAILURE);
				}
			}
			else if (tmp.find("column") != string::npos)
			{
				for (int y = x+1; y < argc; y++)
				{
					flag = false;
					for (unsigned z = 0; z < strlen(argv[y]); z++)
					{
						if (!isdigit(argv[y][z]))
							flag = true;
					}

					if (!flag)
						args.cols.push_back(atoi(argv[y]));
					else
					{
						if (y == x+1)
						{
							cout << "Error: must specify column(s)." << endl;
							exit(EXIT_FAILURE);
						}
						break;
					}
				}

				x += args.cols.size();
			}
			else if (tmp.find("version") != string::npos)
				version();
			else if (tmp.find("help") != string::npos)
				usageMessage();
		}
		else if (tmp.find('-') != string::npos)
		{
			for (unsigned y = 1; y < tmp.size(); y++)
			{
				if (tmp.at(y) == 'v')
					version();
				else if (tmp.at(y) == 'h')
					usageMessage();
				else if (tmp.at(y) == 'p')
				{
					if (args.persist == 0)
					{
						if (args.cols.size() != 0)
						{
							cout << "Error: Must specify rows to persist before columns." << endl;
							exit(EXIT_FAILURE);
						}
						else
						{
							if (argc <= x+1)
							{
								cout << "Error: must specify number of rows to persist." << endl;
								exit(EXIT_FAILURE);
							}
							for (unsigned z = 0; y < strlen(argv[x+1]); z++)
							{
								if (!isdigit(argv[x+1][y]))
									flag = true;
							}

							if (!flag)
								args.persist = atoi(argv[x+1]);
							else
							{
								cout << "Error: must specify number of rows to persist." << endl;
								exit(EXIT_FAILURE);
							}
						}
					}
				}
				else if (tmp.at(y) == 'c')
				{
					if (args.cols.size() == 0)
					{
						if (tmp.find('p') != string::npos && tmp.find('p') < tmp.find('c'))
							offset = x+2;
						else
							offset = x+1;

						for (int z = offset; z < argc; z++)
						{
							flag = false;
							for (unsigned a = 0; a < strlen(argv[z]); a++)
							{
								if (!isdigit(argv[z][a]))
									flag = true;
							}

							if (!flag)
								args.cols.push_back(atoi(argv[z]));
							else
							{
								if (z == x+1)
								{
									cout << "Error: must specify column(s)." << endl;
									exit(EXIT_FAILURE);
								}
								break;
							}
						}
					}
				}
			}

			if ((tmp.find('p') != string::npos) || (tmp.find('c') != string::npos))
			{
				x += args.cols.size();

				if (args.persist != 0)
					x++;
			}
		}
		else
			args.fName = tmp;
	}

	return args;
}