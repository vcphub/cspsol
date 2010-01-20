
/* File: cmdline.cpp
 * Description: Command line options processing.
 *
 */

#include<iostream>
#include<fstream>
#include<iomanip>
#include<string>
#include<cstdlib>
#include<cstring>
#include "cmdline.h"

using namespace std;
// Macro
#define parse_cla(str) (strcmp(argv[i], str) == 0)
#define VERSION_NUMBER "1.0"

/*-------------------------------------------------------------------
Constructor. 
Initialize properties to default values.
-------------------------------------------------------------------*/
CmdOption::CmdOption()
{
        /* BB tree can be processed by DFS or BFS. Default = DFS. */
	search = DFS;
	data_file = NULL;

        /* batch testing mode is off. */
	test = false;           
	tc_file = NULL;

        /* input data is CSP format. */
	bpp = false;

        /* Solve subproblem by dynamic programming (DP). */
	subintopt = false;      
	workaround = false;

	/* CG is done at every BB node, including root node. */
	cg_root_only = false;    

        /* Progress is displayed on standard output. */
	silent = false;
	opt_level = 1.0;
}

/*-------------------------------------------------------------------
Print program usage.
-------------------------------------------------------------------*/
void CmdOption::print_usage() 
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

	cout << "Options :"<<endl<<endl;

	cout << "--test filename"<<endl;
        cout << "                Solve all test cases input data file ."<<endl;
	cout << "--dfs		Process branch and bound tree in depth first manner (default)."<<endl;
	cout << "--bfs		Process branch and bound tree in breadth first manner."<<endl;
	cout << "-Olevel		Optimization level (1 to 3). Default value is 1. ";
	cout << "Higher level may take more time and memory."<<endl;

	cout << "--cgroot	Perform column generation only at root node."<<endl;
	cout << "--bpp		Assume input data file in bin packing problem (BPP) format."<<endl;
	cout << "--silent	No output printed to terminal."<<endl;

	cout << "--otext filename"<<endl;	
	cout << "		Write solution to specified file in text format."<<endl;
	cout << "		If the filename is 'stdout' then solution is printed \
to output terminal."<<endl<<endl;

	cout << "--oxml filename"<<endl;	
	cout << "		Write solution to specified file in XML format."<<endl;
	cout << "		If the filename is 'stdout' then solution is printed \
to output terminal."<<endl<<endl;

	cout << "--si		Use glp_intopt to solve subproblem (knapsack)."<<endl;
	cout << "		By default dynamic programming is used."<<endl;

	cout << "--wa		Use workaround to get alternate opt. int. sol."<<endl;
	cout << "		To be used with --si. Needs changes/patch to GLPK lib."<<endl;

	cout << "-h, --help	Display this help information and exit."<<endl;
	cout << "-v, --version   Display version information and exit."<<endl;

	cout << endl;

	exit(-1);
}

/*-------------------------------------------------------------------
Print program usage.
-------------------------------------------------------------------*/
void CmdOption::print_version() 
{
        cout<<"cspsol: Cutting Stock Problem Solver, version "<< VERSION_NUMBER <<endl<<endl;

        cout<<"Copyright (C) 2008, 2009, 2010 Vijay C. Patil."<<endl<<endl;
        cout<<"This program is free software; you may redistribute it under the terms of "<<endl;
        cout<<"the GNU General Public License, version 3, This program has absolutely no warranty."<<endl;

	cout << endl;
	exit(-1);
}

/*-------------------------------------------------------------------
Process command line options.
-------------------------------------------------------------------*/
void CmdOption::process_arguments(int argc, char * argv[])
{
	/* Parse and process the arguments. */
	if(argc == 1)
		print_usage();

	int i = 0;
	for(i = 1; i < argc; i++) {

		if(parse_cla("--help") || parse_cla("-h"))
			print_usage();
		else if(parse_cla("--version") || parse_cla("-v"))
			print_version();

		else if(parse_cla("--test")) {
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				cerr << "cspsol error: Testcases file NOT specified." << endl;
				print_usage();
			}    
			this->tc_file = argv[i];
			this->test = true;

		} else if(parse_cla("--dfs"))
			this->search = DFS;
		else if(parse_cla("--bfs"))
			this->search = BFS;
		else if(parse_cla("--cgroot"))
			this->cg_root_only = true;
		else if(parse_cla("--bpp"))
			this->bpp = true;
		else if(parse_cla("-O1"))
			this->opt_level = 1.0;
		else if(parse_cla("-O2"))
			this->opt_level = 2.0;
		else if(parse_cla("-O3"))
			this->opt_level = 3.0;
		else if(parse_cla("--silent"))
			this->silent = true;
		else if(parse_cla("--wa")) {
			cout<<endl;
			cout<<"IMP: Must use patched GLPK lib to allow -ve tol_obj."<<endl;
			cout<<endl;
			this->workaround = true;

		} else if(parse_cla("--subintopt") || parse_cla("--si")) {
			this->subintopt = true;

		} else if (parse_cla("-d") || parse_cla("--data")) {
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				cerr << "cspsol error: Orders data file NOT specified." << endl;
				print_usage();
			}    
			this->data_file = argv[i];

		} else if (parse_cla("--oxml")) {
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				cerr << "cspsol error: Output solution report file NOT specified." << endl;
				print_usage();
			}    
			this->rformats.push_back(XML);
			this->rfilenames.push_back(argv[i]);

		} else if (parse_cla("--otext")) {
			i++; 
			if (i == argc || argv[i][0] == '\0' || argv[i][0] == '-')  {
				cerr << "cspsol error: Output solution report file NOT specified." << endl;
				print_usage();
			}    
			this->rformats.push_back(TEXT);
			this->rfilenames.push_back(argv[i]);
		} 
	}

	if((i == argc) && (this->data_file == NULL) && 
					(this->tc_file == NULL)) {
		cerr << "cspsol error: Orders data file NOT specified." << endl;
		print_usage();
	}

	if(this->silent == true) {
		/* Redirect terminal output to file cout.txt opened using tout.*/
		this->redirect_cout();
	}
}

/* Redirect terminal output to file cout.txt opened using tout.*/
void CmdOption::redirect_cout()
{
	this->tout.open("cout.txt");
	this->cout_buf = cout.rdbuf();
	streambuf * tout_buf = this->tout.rdbuf();
	cout.rdbuf(tout_buf);
}

void CmdOption::restore_cout()
{
	if(this->silent == true)
		cout.rdbuf(this->cout_buf);
}
