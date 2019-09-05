#include <vector>
#include <iostream>
#include <cstdlib>
#include <sys/stat.h>
#include <cctype>
#include <cstring>
#include <string>
#include <getopt.h>
#include "CSVReader.h"
#include "Viewer.h"
using namespace std;

//COMMANDLINE INTERFACE
void usageMessage();
void version();
void notFound(argData args);
argData parseArgs (int argc, char* argv[]);

int main (int argc, char* argv[])
{
	if (argc < 2) usageMessage();
	else
	{
		struct stat buf;

		argData args = parseArgs(argc, argv);

		if (stat(args.fName.c_str(), &buf) == 0)
		{
			cout << "Loading (delimiter '" << args.delim << "')..." << endl;

			CSVReader csv (args.fName.c_str(), args.delim);
			Viewer viewer (&csv, args);

			viewer.view();
		}
		else notFound(args);
	}

	return 0;
}


void usageMessage()
{
	cout << "USAGE: csviewer [OPTIONS] FILE" << endl;
	cout << "csviewer is a small, commandline CSV file viewer using ncurses." << endl << endl;
	cout << "Options:" << endl;
	cout << " -p, --persist <n>\tMake the first n rows persist at top as a header." << endl;
	//cout << " -c, --column <n> [...]\tOnly display column(s) n,..." << endl;
	cout << " -d, --delim <c>\tUse 'c' as the field delimiter." << endl;
	cout << " -v, --version\t\tPrint version number and exit." << endl;
	cout << " -h, --help\t\tDisplay this help message and exit." << endl << endl;
	cout << "Arguments:" << endl;
	cout << " FILE\t\t\tPath to csv file to open." << endl;

	exit(0);
}

void version()
{
	cout << "csviewer 0.2-alpha0" << endl;
	cout << "Author: Paul Bonnen" << endl;

	exit(0);
}

void notFound (argData args)
{
	cout << "csviewer: error: " << args.fName << ": file not found" << endl;
}

argData parseArgs (int argc, char* argv[])
{
	argData args;

	args.persist = 0;
	args.delim = ',';

        const static struct option opts[] = {
            {"persist", required_argument, nullptr, 'p'},
            {"delim", required_argument, nullptr, 'd'},
            {"version", no_argument, nullptr, 'v'},
            {"help", no_argument, nullptr, 'h'},
            {nullptr, 0, nullptr, '\0'}
        };

        int idx = 0, c;
        int argStart = 1;

        while ((c = getopt_long(argc, argv, "p:d:vh", opts, &idx)) != -1) {
            switch (c) {
            case 'p':
                if (!optarg) {
                    cerr << "Error: must specify a number of rows to persist" << endl;
                    exit(EXIT_FAILURE);
                }

                args.persist = atoi(optarg);
                argStart += 2;
                break;
            case 'd':
                if (!optarg) {
                    cerr << "Error: must specify a delimiter" << endl;
                    exit(EXIT_FAILURE);
                }

                args.delim = *optarg;
                argStart += 2;
                break;
            case 'v':
                version();
                break;
            case 'h':
                usageMessage();
                break;
            default:
                cerr << "Error: Unknown option '-" << static_cast<char>(c) << '\'' << endl;
                exit(EXIT_FAILURE);
                break;
            }
        }

        args.fName = argv[argStart];
	return args;
}
