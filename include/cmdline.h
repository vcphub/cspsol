#ifndef CMDLINE_H
#define CMDLINE_H

#include <fstream>

enum SearchStrategy {DFS, BFS};

class CmdOption
{
	public:
		SearchStrategy search; 

		/* Input data file name */
		char * data_file; 

		/* Subproblem glp_intopt. */
		bool subintopt; 

		/* Workaround for multiple solutions. */
		bool workaround; 

		/* No progress output to standard terminal. */
		bool silent; 
		std::ofstream tout; /* Terminal outout redirection. */

		CmdOption();

};

/* Global functions. */
CmdOption * process_arguments(int argc, char * argv[]);
void print_usage();

#endif
