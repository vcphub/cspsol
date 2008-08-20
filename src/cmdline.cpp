
/* File: cmdline.cpp
 * Description: Command line options processing.
 *
 */

#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstdlib>
#include "cmdline.h"

using namespace std;
#define parse_cla(str) (strcmp(argv[i], str) == 0)

/* Constructor. */
CmdOption::CmdOption()
{
	/* Set default values. */
	search = DFS;
	data_file = NULL;

	workaround = false;
	subintopt = false;

	silent = false;
}

/* Print program usage. */
void print_usage() 
{
	cout << endl;
	cout << "Usage: cspsol [options...]" << " --data filename" << endl << endl;
	cout << "Where filename contains orders data in following format." << endl;
	cout << "max_pattern_width" << endl;
	cout << "order_width_1 demand_1" << endl;
	cout << "order_width_2 demand_2" << endl;
	cout << "order_width_n demand_n" << endl << endl;

	cout << "All order widths must be <= max_pattern_width." << endl;
	cout << "All demands, order widths and max_pattern_width must be INTEGERs." << endl;
	cout << endl;

	cout << "Options:"<<endl;
	cout << "--dfs		Process branch and bound tree in depth first manner (default)."<<endl;
	cout << "--bfs		Process branch and bound tree in breadth first manner."<<endl;
	cout << "--silent,	No output printed to terminal."<<endl;
	cout << "--si,		Use glp_intopt to solve subproblem (knapsack)."<<endl;
	cout << "		By default dynamic programming is used."<<endl;

	cout << "--wa		Use workaround to get alternate opt. int. sol."<<endl;
	cout << "		To be used with --si. Needs changes/patch to GLPK lib."<<endl;

	cout << "-h, --help	Display this help information and exit."<<endl;
	cout << endl;

	exit(-1);
}

/* Process command line options. */
CmdOption * process_arguments(int argc, char * argv[])
{
	CmdOption * option = new CmdOption();

	/* Parse the argument */
	if(argc == 1)
		print_usage();
	int i;
	for(i = 1; i < argc; i++) {

		if(parse_cla("--help") || parse_cla("-h"))
			print_usage();

		if(parse_cla("--dfs"))
			option->search = DFS;
		else if(parse_cla("--bfs"))
			option->search = BFS;
		else if(parse_cla("--silent"))
			option->silent = true;
		else if(parse_cla("--wa")) {
			cout<<endl;
			cout<<"IMP: Must use patched GLPK lib to allow -ve tol_obj."<<endl;
			cout<<endl;
			option->workaround = true;

		} else if(parse_cla("--subintopt") || parse_cla("--si")) {
			option->subintopt = true;
		} else if (parse_cla("-d") || parse_cla("--data")) {
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				cerr << "cspsol error: Orders data file NOT specified." << endl;
				print_usage();
			}    
			option->data_file = argv[i];
		} 
	}

	if((i == argc)  && (option->data_file == NULL)) {
		cerr << "cspsol error: Orders data file NOT specified." << endl;
		print_usage();
	}

	if(option->silent == true) {
		/* Redirect terminal output to file cout.txt opened using tout.*/
		option->tout.open("cout.txt");
		streambuf * tout_buf = option->tout.rdbuf();
		cout.rdbuf(tout_buf);
	}

	return option;
}

